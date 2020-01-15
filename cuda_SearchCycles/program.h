// стандартные заголовочные файлы
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits.h>

// специальные заголовочные файлы CUDA

#include <cuda_runtime_api.h>
#include <cuda_runtime.h>

// подключение набора стандартных математических констант

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

// используемые константы

const string pyscriptname = "parser.py"; // название запускаемого питоновского скрипта - парсера конфигурационного файла
const int MAX_DIMENSION = 16; // максимально возможная размерность многомерного кубика
const int MAX_DISCR = 1024; // максимально возможное значение параметра дискретизации
const int MAX_ARRAY_SIZE = 16384; // максимально возможная длина массива
const int LIM_MAX_CYCLE_LENGTH = 128; // предельное возможное максимальноезначение длины  цикла

//struct Decision { // последовательность решения
//	float *x; // float x[MAX_DIMENSION]; // координаты решения
//	Decision *next;
//};

// функции и отображения, используемые в программе

// отображение, задаваемое дифференциальным уравнением x'=F(t,x), где F - матрица поворота на заданный период, угол поворота t=T
__device__ float2 Rotation(float2 p, int num_iter, float math_mapping_params[MAX_ARRAY_SIZE]) { 
	float2 result;
	// метод Рунге-Кутты четвертого порядка
	float step = math_mapping_params[0];
	float tn = num_iter*step;
	float k1, k2, k3, k4; // вспомогательные слагаемые для метода Рунге-Кутты
	k1 = step*(p.x*cos(tn) + p.y*sin(tn));
	k2 = step*((p.x+step*k1/2)*cos(tn+step/2) + (p.y+step*k1/2)*sin(tn+step/2));
	k3 = step*((p.x+step*k2/2)*cos(tn+step/2) + (p.y+step*k2/2)*sin(tn+step/2));
	k4 = step*((p.x+step*k3)*cos(tn+step) + (p.y+step*k3)*sin(tn+step));
	result.x = p.x + (1./6)*(k1+2*k2+2*k3+k4);
	k1 = step*(-p.x*sin(tn) + p.y*cos(tn));
	k2 = step*(-(p.x+step*k1/2)*sin(tn+step/2) + (p.y+step*k1/2)*cos(tn+step/2));
	k3 = step*(-(p.x+step*k2/2)*sin(tn+step/2) + (p.y+step*k2/2)*cos(tn+step/2));
	k4 = step*(-(p.x+step*k3)*sin(tn+step) + (p.y+step*k3)*cos(tn+step));
	result.y = p.y+(1./6)*(k1+2*k2+2*k3+k4);
	return result;
}

__device__ float2 BernulliMapping(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // обобщенное отображение Бернулли 
	float2 result;
	result.x = math_mapping_params[0]*p.x;
	result.y = math_mapping_params[0]*p.y;
	return result;
}

__device__ float2 Inversion(float2 p) { // инверсия координат точки
	float2 result;
	result.x = -p.x;
	result.y = -p.y;
	return result;
}

__device__ float2 Translation(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // параллельный перенос
	float2 result;
	result.x = p.x + math_mapping_params[0];
	result.y = p.y + math_mapping_params[1];
	return result;
}

__device__ float2 TwoAgePopulation(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // модель двухвозрастной популяции
	float2 result;
	result.x = math_mapping_params[0]*p.y;
	result.y = p.x*(1-p.x) + math_mapping_params[1]*p.y;
	return result;
}

__device__ float2 VolterraLotka(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // модель Вольтерра-Лотки
	float2 result;
	result.x = p.x*(-math_mapping_params[0] + math_mapping_params[2]*p.y);
	result.y = p.y*(-math_mapping_params[1] + math_mapping_params[3]*p.x);
	return result;
}

__device__ float2 LogicalMapping(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // логистическое отображение
	float2 result;
	result.x = math_mapping_params[0]*p.x*(1-p.x);
	result.y = math_mapping_params[0]*p.y*(1-p.y);
	return result;
}

__device__ float2 FilteringMapping(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // фильтрующее отображение
	float2 result;
	result.y = math_mapping_params[0]*p.y + math_mapping_params[1]*p.x;
	result.x = p.y;
	if (result.y > 1)
		result.y = 1;
	if (result.y < -1)
		result.y = -1;
	return result;
}
__device__ void InitPointCoordinates(float *solution) {
	for (int i = 0; i < 4; i++)
		solution[i] = 0.0;
}
__device__ float2 Relax(float2 p, float math_mapping_params[MAX_ARRAY_SIZE]) { // relax отображение
	float sln[MAX_DIMENSION], sln_plus_zero[MAX_DIMENSION], sln_alpha_plus_zero[MAX_DIMENSION];
	for (int i = 0; i < MAX_DIMENSION; i++){
		sln[i] = 0.0;
		sln_plus_zero[i]=0;
		sln_alpha_plus_zero[i]= 0;
	}
	sln[1]=p.x;
	sln[2]=p.y;
	float d=math_mapping_params[0];
	float alpha = math_mapping_params[1];
	float beta = math_mapping_params[2];
	float period =alpha + 1.0 + (beta+1.0)/(alpha-beta-1.0);
	float calc_acc= math_mapping_params[3];
	int num_iter = 0;
	float phase = -calc_acc/2;
	num_iter++;
	phase += calc_acc;
	while ((period - phase) > 0.0) {
		if ((phase - period) >= 0.0) // ограничение, чтобы при расчёте не выйти за пределы рассматриваемого периода
			phase = period;
	    if ((phase-calc_acc <= 0.0 && phase >= 0.0) || (phase-calc_acc <= 1.0 && phase >= 1.0) || (phase-calc_acc <= alpha && phase >= alpha) || (phase-calc_acc <= alpha+1 && phase >= alpha+1)) 
			for (int j = 1; j <= 2; j++) {
				if (phase-calc_acc <= 0.0 && phase >= 0.0) { // phase = +0
					sln_plus_zero[j] = (alpha-1)/(alpha-beta-1)*sln[j];
					sln[j] = sln_plus_zero[j];
				}
				if (phase-calc_acc <= 1.0 && phase >= 1.0) { // phase = 1+0
					sln[j] = sln[j] - alpha/(alpha-1)*sln_plus_zero[j];
				}
				if (phase-calc_acc <= alpha && phase >= alpha) { // phase = alpha+0
					sln_alpha_plus_zero[j] = (1+beta)*sln[j];
					sln[j] = sln_alpha_plus_zero[j];
				}
				if (phase-calc_acc <= alpha+1 && phase >= alpha+1) { // phase = alpha+1+0
					sln[j] = sln[j] - alpha/(beta+1)*sln_alpha_plus_zero[j];
				}
			}
		else {
			float k1, k2, k3, k4; // вспомогательные слагаемые для метода Рунге-Кутты
			for (int j = 1; j <= 2; j++) {
				k1 = calc_acc*d*(exp(sln[j+1])+exp(-sln[j])-exp(sln[j])-exp(-sln[j-1]));
				k2 = calc_acc*d*(exp(sln[j+1]+calc_acc*k1/2)+exp(-sln[j]+calc_acc*k1/2)-exp(sln[j]+calc_acc*k1/2)-exp(-sln[j-1]+calc_acc*k1/2));
				k3 = calc_acc*d*(exp(sln[j+1]+calc_acc*k2/2)+exp(-sln[j]+calc_acc*k2/2)-exp(sln[j]+calc_acc*k2/2)-exp(-sln[j-1]+calc_acc*k2/2));
				k4 = calc_acc*d*(exp(sln[j+1]+calc_acc*k3)+exp(-sln[j]+calc_acc*k3)-exp(sln[j]+calc_acc*k3)-exp(-sln[j-1]+calc_acc*k3));
				sln[j] = sln[j] + (1./6)*(k1+2*k2+2*k3+k4);
			}
		}
		
		num_iter++;
		phase += calc_acc;
	}	
	return make_float2(sln[1], sln[2]);
}

__device__ float2 MathMapping(float2 p, int num_iter, int num_math_mapping, float math_mapping_params[MAX_ARRAY_SIZE]) {
	switch (num_math_mapping) {
		case 1: return Rotation(p, num_iter, math_mapping_params);
		case 2: return BernulliMapping(p, math_mapping_params);
		case 3: return LogicalMapping(p, math_mapping_params);
		case 4: return FilteringMapping(p, math_mapping_params);
		case 5: return Relax(p, math_mapping_params);
		case 6: return Inversion(p);
		case 7: return Translation(p, math_mapping_params);
		case 8: return TwoAgePopulation(p, math_mapping_params);
		case 9: return VolterraLotka(p, math_mapping_params);
		default: return make_float2(0.0, 0.0);
	}
}


struct CycleData { // данные о цикле
	// т.к. данные с такой структурой используются в ядре, то вместо одного поля vector (его в этом случае нельзя использовать),
	// задаётся 2 поля - длина и стандартный сишный одномерный массив
	int length; // длина цикла
	float2 points[LIM_MAX_CYCLE_LENGTH]; // элементы цикла
};

struct DifferingCycleInfo { // информация о цикле
	// т.к. данные с такой структурой используются только в операциях на CPU, то можно исполльзовать vector
	vector <float2> elements; // элементы цикла
	vector <float2> startpoints; // стартовые точки, из которых его можно получить
};