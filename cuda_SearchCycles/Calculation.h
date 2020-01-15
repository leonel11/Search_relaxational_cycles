#pragma region Формулировка задачи

// Рассматривается математическая модель нейронной системы, реализующая релаксационные автоколебания в многомерном пространстве. С помощью входных параметров 
// определяются вид дифференциального уравнения и период автоколебаний (сами колебания начинаются в начальный момент времени (t = 0) из состояния покоя - точки -0). 
// Требуется найти координаты конечной точки (y, sln, solution).
// (подробнее о релаксационных автоколебаниях можно прочитать из статей DU0155.pdf, DU0919.pdf, DU1675.pdf, расположенных в папке Theory этого проекта)
//
// Вход: в файле in.txt содержатся данные для работы программы
//		dimension (size, m) - размерность пространства для задачи;
//		d - множитель, на который домножается основная часть дифференциального уравнения, реализующего релаксационные автоколебания;
//		alpha, beta - параметры, определяющие период (period), до которого необходимо рассматривать релаксационные автоколебания в системе;
//		eps - точность цикла (если какие-нибудь 2 значения в последовательности решений отличаются друг от друг не более чем на eps по модулю, 
//			  то существует подозрение, что в последняя полученная точка - стационарная;
//		calc_acc (step, h) - малый параметр, определяющий величину, на которую изменяется зависимая переменная (фаза phase) на каждом шаге (необходимо для метода 
//							 Рунге-Кутты).
// 
// Начальные условия для задачи: 
//		dimension >= 1;
//		alpha, beta > 0
//		alpha ~ 2.1;
//		beta ~ 1;
//		alpha-beta-1 > 0;
//		0<=phase<=period, где phase (tn, t) - время, в течение которого происходят релаксационные автоколебания в системе;
//		period = alpha + 1 + (beta+1)/(alpha-beta-1);
//

#pragma endregion 

// стандартные заголовочные файлы

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
// стандартные пространства имён

using namespace std;

// используемые константы

const int MAX_DIMENSION = 100; // максимально возможная размерность вектора-решения

class RelaxarionOscillations {
	public:
		__device__ RelaxarionOscillations(int dim, float *solution, float par_d, float par_alpha, float par_beta, float epsilon, float h);
		__device__ void SetSolution(float *solution);
		__device__ bool IsEqualPoints(float *prev); // эквивалентные точки с точностью до eps
		__device__ int GetDimension();
		__device__ void CalcSolutionCoordinates(); // итерационный расчёт координат конечной точки - решения
		__device__ void OutputPointCoordinates(FILE *output_file, int first, int last); // вывод координат точки, начиая с позиции first и заканчивая last включительно, в файл
		__device__ ~RelaxarionOscillations();
		float sln[MAX_DIMENSION]; // массив для хранения координат решения
	private:
		int dimension;
		float alpha;
		float beta;
		float d; 
		float calc_acc;
		float period;
		float eps;
		// координаты точек, полученных из краевых условий, которые нужны для последующих расчётов
		float sln_plus_zero[MAX_DIMENSION];
		float sln_alpha_plus_zero[MAX_DIMENSION];
	private:
		__device__ float CalcPeriod(); // высчитывание значения периода
		__device__ void InitPointCoordinates(float *solution); // инициализация массива-параметра, хранящего координаты точки, нулями
		__device__ bool IsPassedSwitchPoint(float phase); // проверка была ли пройдена точка переключения, в которой системе придаётся импульс, в зависимости от текущей фазы
		__device__ void RungeKuttaMethod(); // расчёт координат точки методом Рунге-Кутты четвертого порядка
		__device__ void MakeImpetusToPointCoordinates(float phase); // воздействие импульсом с нужной величиной, в зависимости от фазы, на координаты в точках переключения
};