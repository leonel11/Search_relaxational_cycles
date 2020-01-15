#pragma region Вспомогательные заголовочные файлы

#include "parser.h"

#pragma endregion

#ifndef CALCULATION_H
#define CALCULATION_H

#pragma region Описание структуры класса

/**
* @class RelaxarionOscillations
*/
class RelaxarionOscillations {

	public:
		RelaxarionOscillations(DataParser config, float *start_point);
		void SetSolution(float *solution); // установка других значений координат точки в точку-решение
		float *GetSolution(); // возврат точки-решения
		void CalculateSolution(); // расчёт координат точки-решения после релаксационных колебаний
		~RelaxarionOscillations();

	#pragma region static public functions

		/**
		* @brief Печать на экран стационарных точек, которые должны получаться в теории, исходя из начальных параметров задачи
		* @param config - параметр, хранящий в себе все начальные параметры задачи
		*/
		static void DisplayTheoreticalSolutions(DataParser config) {
			int dim = config.Dim();
			float d = config.D();
			float alpha = config.Alpha();
			float beta = config.Beta();
			cout << "Stationary points of the theory for given initial values:" << endl;
			float theor_sln[MAX_DIM]; // массив для хранения координат решения, получающегося из теории
			PointOperations::ZeroizePoint(theor_sln);
			cout << TypesChange::PointToString(theor_sln, 1, dim) << endl;
			for (int k = 0; k <= dim; ++k) {
				PointOperations::ZeroizePoint(theor_sln);
				// расчет различных теоретических результатов в зависимости от области параметров
				// (см. Теорему 2 из файла DU0155.pdf)
				for (int idx = 1; idx <= dim; ++idx) {
					// (alpha, beta) in U1 = {(alpha, beta): alpha > (beta+1)*(beta+2), beta > 0}             or
					// (alpha, beta) in U3 = {(alpha, beta): 2*beta+2 < alpha < (beta+1)*(beta+2), beta > 0}  or
					// (alpha, beta) in U4 = {(alpha, beta): max(2, 1+beta) < alpha < 2*beta+2, beta > 0}
					if ((beta > 0.0 && alpha > (beta+1.0)*(beta+2.0)) || 
						(beta > 0.0 && alpha > 2.0*beta+2.0 && alpha < (beta+1.0)*(beta+2.0)) ||
						(beta > 0.0 && alpha > max(2.0, 1.0+beta) && alpha < 2.0*beta+2.0)) {
						if (idx <= k) // idx = 1...k
							theor_sln[idx] = log(1.0/d)-log((beta+1.0)/(alpha-beta-1.0))+log((double)k+1.0-idx);
						else // idx = k+1...m-1
							theor_sln[idx] = -log(1.0/d)+log((beta+1.0)/(alpha-beta-1.0))-log((double)idx-k);
					}
					// (alpha, beta) in U2 = {(alpha, beta): 1 < alpha < 2, 0 < beta<(alpha-1)*(2-alpha)}          or
					// (alpha, beta) in U5 = {(alpha, beta): 1 < alpha < 2, (alpha-1)*(2-alpha) < beta < alpha-1}
					else if ((alpha > 1.0 && alpha < 2.0 && beta > 0.0 && beta < (alpha-1.0)*(2.0-alpha)) || 
							 (alpha > 1.0 && alpha < 2.0 && beta > (alpha-1.0)*(2.0-alpha) && beta < (alpha-1.0))) { 
						if (idx <= k) // idx = 1...k
							theor_sln[idx] = -(alpha-1.0)*log(1.0/d)-(alpha-1.0)*log((double)k+1.0-idx)+alpha*log(alpha-1.0);
						else // idx = k+1...m-1
							theor_sln[idx] = (alpha-1.0)*log(1.0/d)+(alpha-1.0)*log((double)idx-k)-alpha*log(alpha-1.0);
					}
					else {
						cout << endl;
						return;
					}
				}
				cout << TypesChange::PointToString(theor_sln, 1, dim) << endl;
			}
			cout << endl;
		}

	#pragma endregion

	private:
		float Period(); // возврат значения периода
		void ZeroizeBoundaryConditions(); // обнуление координат точек-краевых условий начального дифференциального уравнения
		bool IsPassedSwitchPoint(float t); // проверка была ли пройдена точка переключения, в которой системе придаётся импульс
		void RungeKuttaMethod(); // расчёт новых координат точки-решения методом Рунге-Кутты четвертого порядка
		void MakeImpetusToSolution(float t); // воздействие импульсом на координаты решения в точках переключения в зависимости от времени
		void FormulaMethod(float t); // расчёт новых координат точки-решения по формуле решения дифференциального уравнения
		void NumericComputingOfSolution(); // численный расчёт координат точки-решения численно
		void AnalyticComputingOfSolution(); // расчёт координат точки-решения аналитически
		void CalculateCoefsOfAnalyticSolution(float t); // расчёт значений свободных коэффициентов в решении дифференциального уравнения

	private:
		int dim;
		string sln_mode;
		//string bound_conds;
		//string dim_wave;
		string wave_type;
		float alpha;
		float beta;
		float d; 
		float h;
		float eps;
		float sln[MAX_DIM]; // массив для хранения координат решения
							// координаты точек, полученных из краевых условий, которые нужны для последующих расчётов
		float c_coef[MAX_DIM]; // значения свободных коэффициентов в решении дифференциального уравнения
		float bound_cond_plus_zero[MAX_DIM]; // z(+0)
		float bound_cond_alpha_plus_zero[MAX_DIM]; // z(alpha+0)

};

#pragma endregion

#endif