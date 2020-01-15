#pragma region ��������������� ������������ �����

#include "parser.h"

#pragma endregion

#ifndef CALCULATION_H
#define CALCULATION_H

#pragma region �������� ��������� ������

/**
* @class RelaxarionOscillations
*/
class RelaxarionOscillations {

	public:
		RelaxarionOscillations(DataParser config, float *start_point);
		void SetSolution(float *solution); // ��������� ������ �������� ��������� ����� � �����-�������
		float *GetSolution(); // ������� �����-�������
		void CalculateSolution(); // ������ ��������� �����-������� ����� �������������� ���������
		~RelaxarionOscillations();

	#pragma region static public functions

		/**
		* @brief ������ �� ����� ������������ �����, ������� ������ ���������� � ������, ������ �� ��������� ���������� ������
		* @param config - ��������, �������� � ���� ��� ��������� ��������� ������
		*/
		static void DisplayTheoreticalSolutions(DataParser config) {
			int dim = config.Dim();
			float d = config.D();
			float alpha = config.Alpha();
			float beta = config.Beta();
			cout << "Stationary points of the theory for given initial values:" << endl;
			float theor_sln[MAX_DIM]; // ������ ��� �������� ��������� �������, ������������� �� ������
			PointOperations::ZeroizePoint(theor_sln);
			cout << TypesChange::PointToString(theor_sln, 1, dim) << endl;
			for (int k = 0; k <= dim; ++k) {
				PointOperations::ZeroizePoint(theor_sln);
				// ������ ��������� ������������� ����������� � ����������� �� ������� ����������
				// (��. ������� 2 �� ����� DU0155.pdf)
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
		float Period(); // ������� �������� �������
		void ZeroizeBoundaryConditions(); // ��������� ��������� �����-������� ������� ���������� ����������������� ���������
		bool IsPassedSwitchPoint(float t); // �������� ���� �� �������� ����� ������������, � ������� ������� �������� �������
		void RungeKuttaMethod(); // ������ ����� ��������� �����-������� ������� �����-����� ���������� �������
		void MakeImpetusToSolution(float t); // ����������� ��������� �� ���������� ������� � ������ ������������ � ����������� �� �������
		void FormulaMethod(float t); // ������ ����� ��������� �����-������� �� ������� ������� ����������������� ���������
		void NumericComputingOfSolution(); // ��������� ������ ��������� �����-������� ��������
		void AnalyticComputingOfSolution(); // ������ ��������� �����-������� ������������
		void CalculateCoefsOfAnalyticSolution(float t); // ������ �������� ��������� ������������� � ������� ����������������� ���������

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
		float sln[MAX_DIM]; // ������ ��� �������� ��������� �������
							// ���������� �����, ���������� �� ������� �������, ������� ����� ��� ����������� ��������
		float c_coef[MAX_DIM]; // �������� ��������� ������������� � ������� ����������������� ���������
		float bound_cond_plus_zero[MAX_DIM]; // z(+0)
		float bound_cond_alpha_plus_zero[MAX_DIM]; // z(alpha+0)

};

#pragma endregion

#endif