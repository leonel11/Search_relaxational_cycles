#pragma region ������������ ������

// ��������������� �������������� ������ ��������� �������, ����������� �������������� ������������� � ����������� ������������. � ������� ������� ���������� 
// ������������ ��� ����������������� ��������� � ������ ������������� (���� ��������� ���������� � ��������� ������ ������� (t = 0) �� ��������� ����� - ����� -0). 
// ��������� ����� ���������� �������� ����� (y, sln, solution).
// (��������� � �������������� �������������� ����� ��������� �� ������ DU0155.pdf, DU0919.pdf, DU1675.pdf, ������������� � ����� Theory ����� �������)
//
// ����: � ����� in.txt ���������� ������ ��� ������ ���������
//		dimension (size, m) - ����������� ������������ ��� ������;
//		d - ���������, �� ������� ����������� �������� ����� ����������������� ���������, ������������ �������������� �������������;
//		alpha, beta - ���������, ������������ ������ (period), �� �������� ���������� ������������� �������������� ������������� � �������;
//		eps - �������� ����� (���� �����-������ 2 �������� � ������������������ ������� ���������� ���� �� ���� �� ����� ��� �� eps �� ������, 
//			  �� ���������� ����������, ��� � ��������� ���������� ����� - ������������;
//		calc_acc (step, h) - ����� ��������, ������������ ��������, �� ������� ���������� ��������� ���������� (���� phase) �� ������ ���� (���������� ��� ������ 
//							 �����-�����).
// 
// ��������� ������� ��� ������: 
//		dimension >= 1;
//		alpha, beta > 0
//		alpha ~ 2.1;
//		beta ~ 1;
//		alpha-beta-1 > 0;
//		0<=phase<=period, ��� phase (tn, t) - �����, � ������� �������� ���������� �������������� ������������� � �������;
//		period = alpha + 1 + (beta+1)/(alpha-beta-1);
//

#pragma endregion 

// ����������� ������������ �����

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
// ����������� ������������ ���

using namespace std;

// ������������ ���������

const int MAX_DIMENSION = 100; // ����������� ��������� ����������� �������-�������

class RelaxarionOscillations {
	public:
		__device__ RelaxarionOscillations(int dim, float *solution, float par_d, float par_alpha, float par_beta, float epsilon, float h);
		__device__ void SetSolution(float *solution);
		__device__ bool IsEqualPoints(float *prev); // ������������� ����� � ��������� �� eps
		__device__ int GetDimension();
		__device__ void CalcSolutionCoordinates(); // ������������ ������ ��������� �������� ����� - �������
		__device__ void OutputPointCoordinates(FILE *output_file, int first, int last); // ����� ��������� �����, ������ � ������� first � ���������� last ������������, � ����
		__device__ ~RelaxarionOscillations();
		float sln[MAX_DIMENSION]; // ������ ��� �������� ��������� �������
	private:
		int dimension;
		float alpha;
		float beta;
		float d; 
		float calc_acc;
		float period;
		float eps;
		// ���������� �����, ���������� �� ������� �������, ������� ����� ��� ����������� ��������
		float sln_plus_zero[MAX_DIMENSION];
		float sln_alpha_plus_zero[MAX_DIMENSION];
	private:
		__device__ float CalcPeriod(); // ������������ �������� �������
		__device__ void InitPointCoordinates(float *solution); // ������������� �������-���������, ��������� ���������� �����, ������
		__device__ bool IsPassedSwitchPoint(float phase); // �������� ���� �� �������� ����� ������������, � ������� ������� �������� �������, � ����������� �� ������� ����
		__device__ void RungeKuttaMethod(); // ������ ��������� ����� ������� �����-����� ���������� �������
		__device__ void MakeImpetusToPointCoordinates(float phase); // ����������� ��������� � ������ ���������, � ����������� �� ����, �� ���������� � ������ ������������
};