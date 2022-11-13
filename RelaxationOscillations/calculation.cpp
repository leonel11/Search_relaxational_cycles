#include "calculation.h"
#define _USE_MATH_DEFINES
#include <math.h>

RelaxarionOscillations::RelaxarionOscillations(DataParser config, float *start_point = NULL) {
	dim = config.Dim();
	sln_mode = config.Computing_Method();
	//bound_conds = config.Boundary_Conditions();
	//dim_wave = config.Wave_Direction();
	wave_type = config.Wave_Type();
	d = config.D();
	alpha = config.Alpha();
	beta = config.Beta();
	eps = config.Eps();
	h = config.H();
	PointOperations::InitPoint(sln, start_point);
	PointOperations::ZeroizePoint(c_coef);
	ZeroizeBoundaryConditions();
}

/**
* @brief Обнуление координат точек-краевых условий начального дифференциального уравнения. Они необходимы для расчета координат точки-решения после прохождения точки-переключения
*/
void RelaxarionOscillations::ZeroizeBoundaryConditions() {
	PointOperations::ZeroizePoint(bound_cond_plus_zero);
	PointOperations::ZeroizePoint(bound_cond_alpha_plus_zero);
}

/**
* @brief Установка других значений координат точки в точку-решение.
* @param solution - точка, хранящая в себе необходимые координаты для присваивания
*/
void RelaxarionOscillations::SetSolution(float *solution = NULL) {
	PointOperations::SetPoint(sln, solution);
}

/**
* @brief Возврат точки-решения
*/
float* RelaxarionOscillations::GetSolution() {
	return sln;
}

/**
* @brief Возврат значения периода (T0)
* До этого значения изменяется значение времени, определяющего положение точки-решения в колебательной системе.
* Как только значение времени достигает значения периода, колебания в системе затухают и координаты точки-решения не изменяются
*/
float RelaxarionOscillations::Period() {
	return alpha + 1.0 + (beta+1.0)/(alpha-beta-1.0);
}

/**
* @brief Расчёт значений свободных коэффициентов в решении дифференциального уравнения
* @param t - параметр времени
*/
void RelaxarionOscillations::CalculateCoefsOfAnalyticSolution(float t) {
	// FIXME: реализовано пока только для одномерного и двумерного случая
	if (wave_type == "UNIWAVE") {
		if (dim == 2) {
			c_coef[2] = ( (exp(sln[2])-1.0)*(sin(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)-exp(sln[1])*sin(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)) - (exp(sln[1])-1.0)*(sin(sqrt(3.0)/2.0*d*t)-exp(sln[2])*sin(sqrt(3.0)/2.0*d*t-2*M_PI/3)) ) / ( (exp(sln[2])-1.0)*(exp(sln[1])*cos(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)-cos(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)) - (exp(sln[1])-1.0)*(exp(sln[2])*cos(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)-cos(sqrt(3.0)/2.0*d*t)) );
			c_coef[1] = exp(-3.0/2.0*d*t)/(exp(sln[1])-1.0) * ( sin(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)-exp(sln[1])*sin(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)-c_coef[2]*(exp(sln[1])*cos(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)-cos(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)) );
		}
		return;
	}
	if (wave_type == "NEUMAN") {
		if (dim == 1) 
			c_coef[1] = (1+exp(sln[1]))/(1-exp(sln[1]))*exp(-2*d*t);
		if (dim == 2) {
			c_coef[1] = 2*exp(-3*d*t)*(exp(sln[1]+sln[2]) + exp(sln[1]) + 1) / (exp(sln[1]+sln[2]) - 2*exp(sln[1]) + 1);
			c_coef[2] = c_coef[1]*(1 - exp(sln[2]))*exp(d*t) + (2*exp(sln[2]) + 1)*exp(-2*d*t);
		}
	}
	if (wave_type == "PERIODIC") {
		if (dim == 1) 
			c_coef[1] = (1+exp(sln[1]))/(1-exp(sln[1]))*exp(-4*d*t);
		if (dim == 2) {
			c_coef[1] = exp(-3*d*t)*(exp(sln[1]+sln[2]) + exp(sln[1]) + 1) / (exp(sln[1]) - 2*exp(sln[1]+sln[2]) + 1);
			c_coef[2] = c_coef[1]*(1 - exp(-sln[2]))*exp(3*d*t) + exp(-sln[2]);
		}
	}
}

/**
* @brief Воздействие импульсом на систему (на координаты точки-решения) в точках переключения
* @param t - параметр времени, определяющий какую точку переключения прошла система
*/
void RelaxarionOscillations::MakeImpetusToSolution(float t) {
	for (int j = 1; j <= dim; j++) {
		if (t-h <= 0.0 && t >= 0.0) { // t = +0
			bound_cond_plus_zero[j] = (alpha-1)/(alpha-beta-1)*sln[j];
			sln[j] = bound_cond_plus_zero[j];
		}
		if (t-h <= 1.0 && t >= 1.0) { // t = 1+0
			sln[j] = sln[j] - alpha/(alpha-1)*bound_cond_plus_zero[j];
		}
		if (t-h <= alpha && t >= alpha) { // t = alpha+0
			bound_cond_alpha_plus_zero[j] = (1+beta)*sln[j];
			sln[j] = bound_cond_alpha_plus_zero[j];
		}
		if (t-h <= alpha+1.0 && t >= alpha+1.0) { // t = alpha+1+0
			sln[j] = sln[j] - alpha/(beta+1)*bound_cond_alpha_plus_zero[j];
		}
	}
	PointOperations::SetBoundaryValues(sln, dim, wave_type);
}

/**
* @brief Расчёт новых координат точки-решения методом Рунге-Кутты четвертого порядка
* https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%A0%D1%83%D0%BD%D0%B3%D0%B5_%E2%80%94_%D0%9A%D1%83%D1%82%D1%82%D1%8B#.D0.9A.D0.BB.D0.B0.D1.81.D1.81.D0.B8.D1.87.D0.B5.D1.81.D0.BA.D0.B8.D0.B9_.D0.BC.D0.B5.D1.82.D0.BE.D0.B4_.D0.A0.D1.83.D0.BD.D0.B3.D0.B5.C2.A0.E2.80.94.C2.A0.D0.9A.D1.83.D1.82.D1.82.D1.8B_.D1.87.D0.B5.D1.82.D0.B2.D1.91.D1.80.D1.82.D0.BE.D0.B3.D0.BE_.D0.BF.D0.BE.D1.80.D1.8F.D0.B4.D0.BA.D0.B0
*/
void RelaxarionOscillations::RungeKuttaMethod() {
	float k1 = 0.0, k2 = 0.0, k3 = 0.0, k4 = 0.0; // вспомогательные слагаемые для метода Рунге-Кутты
	if (wave_type == "UNIWAVE") {
		for (int j = 1; j <= dim; j++) {
			k1 = h*d*(exp(sln[j+1])-exp(sln[j]));
			k2 = h*d*(exp(sln[j+1]+h*k1/2)-exp(sln[j]+h*k1/2));
			k3 = h*d*(exp(sln[j+1]+h*k2/2)-exp(sln[j]+h*k2/2));
			k4 = h*d*(exp(sln[j+1]+h*k3)-exp(sln[j]+h*k3));
			sln[j] = sln[j] + (1.0/6)*(k1+2*k2+2*k3+k4);
		}
		PointOperations::SetBoundaryValues(sln, dim, "UNIWAVE");
	}
	if (wave_type == "NEUMAN" || wave_type == "PERIODIC") {
		for (int j = 1; j <= dim; j++) {
			k1 = h*d*(exp(sln[j+1])+exp(-sln[j])-exp(sln[j])-exp(-sln[j-1]));
			k2 = h*d*(exp(sln[j+1]+h*k1/2)+exp(-sln[j]+h*k1/2)-exp(sln[j]+h*k1/2)-exp(-sln[j-1]+h*k1/2));
			k3 = h*d*(exp(sln[j+1]+h*k2/2)+exp(-sln[j]+h*k2/2)-exp(sln[j]+h*k2/2)-exp(-sln[j-1]+h*k2/2));
			k4 = h*d*(exp(sln[j+1]+h*k3)+exp(-sln[j]+h*k3)-exp(sln[j]+h*k3)-exp(-sln[j-1]+h*k3));
			sln[j] = sln[j] + (1.0/6)*(k1+2*k2+2*k3+k4);
		}
		PointOperations::SetBoundaryValues(sln, dim, wave_type);
	}
	if (wave_type == "TWOCLUSTERS") {
		for (int j = 1; j <= dim; j++) {
			if (j == 1) {
				k1 = h*d*(Arithmetics::gate(exp(-sln[1]))+Arithmetics::gate(exp(sln[2]))-Arithmetics::gate(exp(sln[1]))-Arithmetics::gate(exp(sln[1]+sln[2])));
				k2 = h*d*(Arithmetics::gate(exp(-sln[1]+h*k1/2))+Arithmetics::gate(exp(sln[2]+h*k1/2))-Arithmetics::gate(exp(sln[1]+h*k1/2))-Arithmetics::gate(exp(sln[1]+sln[2]+h*k1/2)));
				k3 = h*d*(Arithmetics::gate(exp(-sln[1]+h*k2/2))+Arithmetics::gate(exp(sln[2]+h*k2/2))-Arithmetics::gate(exp(sln[1]+h*k2/2))-Arithmetics::gate(exp(sln[1]+sln[2]+h*k2/2)));
				k4 = h*d*(Arithmetics::gate(exp(-sln[1]+h*k3))+Arithmetics::gate(exp(sln[2]+h*k3))-Arithmetics::gate(exp(sln[1]+h*k3))-Arithmetics::gate(exp(sln[1]+sln[2]+h*k3)));
			}
			if (j == 2) {
				k1 = h*d*(Arithmetics::gate(exp(-sln[1]-sln[2]))+Arithmetics::gate(exp(-sln[2]))-Arithmetics::gate(exp(-sln[1]))-Arithmetics::gate(exp(sln[2])));
				k2 = h*d*(Arithmetics::gate(exp(-sln[1]-sln[2]+h*k1/2))+Arithmetics::gate(exp(-sln[2]+h*k1/2))-Arithmetics::gate(exp(-sln[1]+h*k1/2))-Arithmetics::gate(exp(sln[2]+h*k1/2)));
				k3 = h*d*(Arithmetics::gate(exp(-sln[1]-sln[2]+h*k2/2))+Arithmetics::gate(exp(-sln[2]+h*k2/2))-Arithmetics::gate(exp(-sln[1]+h*k2/2))-Arithmetics::gate(exp(sln[2]+h*k2/2)));
				k4 = h*d*(Arithmetics::gate(exp(-sln[1]-sln[2]+h*k3))+Arithmetics::gate(exp(-sln[2]+h*k3))-Arithmetics::gate(exp(-sln[1]+h*k3))-Arithmetics::gate(exp(sln[2]+h*k3)));
			}
			sln[j] = sln[j] + (1.0/6)*(k1+2*k2+2*k3+k4);
		}
	}
}

/**
* @brief Расчёт новых координат точки-решения явно, по формуле решения дифференциального уравнения
* @param t -  параметр, хранящий в себе значение времени
*/
void RelaxarionOscillations::FormulaMethod(float t) {
	// FIXME: реализовано пока только для одномерного и двумерного случая
	if (wave_type == "UNIWAVE") {
		sln[1] = log( (c_coef[1]+c_coef[2]*exp(-3.0/2.0*d*t)*cos(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)+exp(-3.0/2.0*d*t)*sin(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)) / (c_coef[1]+c_coef[2]*exp(-3.0/2.0*d*t)*cos(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)+exp(-3.0/2.0*d*t)*sin(sqrt(3.0)/2.0*d*t+2.0*M_PI/3.0)) );
		sln[2] = log( (c_coef[1]+c_coef[2]*exp(-3.0/2.0*d*t)*cos(sqrt(3.0)/2.0*d*t)+exp(-3.0/2.0*d*t)*sin(sqrt(3.0)/2.0*d*t)) / (c_coef[1]+c_coef[2]*exp(-3.0/2.0*d*t)*cos(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)+exp(-3.0/2.0*d*t)*sin(sqrt(3.0)/2.0*d*t-2.0*M_PI/3.0)) );
	}
	else {
		if (wave_type == "NEUMAN") {
			if (dim ==1)
				sln[1] = log((c_coef[1] - exp(-2.0*d*t)) / (c_coef[1] + exp(-2.0*d*t)));
			if (dim ==2) {
				sln[1] = log((c_coef[1] - 2*exp(-3*d*t)) / (c_coef[1] + c_coef[2]*exp(-d*t) + exp(-3*d*t)));
				sln[2] = log((c_coef[1] - c_coef[2]*exp(-d*t) + exp(-3*d*t)) / (c_coef[1] - 2*exp(-3*d*t)));
			}	
		}
		if (wave_type == "PERIODIC") {
			if (dim ==1)
				sln[1] = log((c_coef[1] - exp(-4.0*d*t)) / (c_coef[1] + exp(-4.0*d*t)));
			if (dim ==2) {
				sln[1] = log((c_coef[1] - c_coef[2]*exp(-3*d*t)) / (c_coef[1] + c_coef[2]*exp(-3*d*t) + exp(-3*d*t)));
				sln[2] = log((c_coef[1] - exp(-3*d*t)) / (c_coef[1] - c_coef[2]*exp(-3*d*t)));
			}
		}
	}
	PointOperations::SetBoundaryValues(sln, dim, wave_type);
}

/**
* @brief Проверка была ли пройдена точка переключения, в которой системе придаётся импульс
* @param t -  параметр, хранящий в себе значение времени
*/
bool RelaxarionOscillations::IsPassedSwitchPoint(float t) {
	// точки переключения: 0, 1, alpha, alpha+1
	if ((t-h <= 0.0 && t >= 0.0) || (t-h <= 1.0 && t >= 1.0) || (t-h <= alpha && t >= alpha) || (t-h <= alpha+1.0 && t >= alpha+1.0)) {
		return true;
	}
	else
		return false;
}

/**
* @brief Расчёт координат точки-решения численным способом
*/
void RelaxarionOscillations::NumericComputingOfSolution() {
	float T0 = Period();
	float t = -h/2;
	while (T0 - t > 0.0) { // итерационный расчёт
		t += h;
		if ((t - T0) >= 0.0) // ограничение, чтобы при расчёте не выйти за пределы рассматриваемого периода
			t = T0;
		if (IsPassedSwitchPoint(t))
			MakeImpetusToSolution(t);
		else 
			RungeKuttaMethod();
	}
}

/**
* @brief Расчёт координат точки-решения аналитическим способом
*/
void RelaxarionOscillations::AnalyticComputingOfSolution() {
	float T0 = Period();
	float t = 0 + h/2.0;
	MakeImpetusToSolution(t);
	CalculateCoefsOfAnalyticSolution(t);
	t = 1 + h/2.0;
	FormulaMethod(t);
	MakeImpetusToSolution(t);
	CalculateCoefsOfAnalyticSolution(t);
	t = alpha + h/2.0;
	FormulaMethod(t);
	MakeImpetusToSolution(t);
	CalculateCoefsOfAnalyticSolution(t);
	t = alpha + 1.0 + h/2.0;
	FormulaMethod(t);
	MakeImpetusToSolution(t);
	CalculateCoefsOfAnalyticSolution(t);
	t = Period();
	FormulaMethod(t);
}

/**
* @brief Расчёт координат точки-решения в результате релаксационных колебаний
*/
void RelaxarionOscillations::CalculateSolution() {
	if (sln_mode == "NUMERIC")
		NumericComputingOfSolution();
	else // sln_mode == "ANALYTIC"
		AnalyticComputingOfSolution();
}

RelaxarionOscillations::~RelaxarionOscillations() {
}