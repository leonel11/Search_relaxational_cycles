#ifndef STD_THINGS 
#define STD_THINGS

#pragma region ������������ ����������� ������������ �����

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <windows.h>
#include <time.h>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <sstream> 
#include <string>
#include <vector>
#include <algorithm>

#pragma endregion

#pragma region ������������ ����������� ������������ ���

using namespace std;

#pragma endregion

#endif

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#pragma region ���������-����������� ������

const int MAX_DIM = 20; // ����������� ��������� ����������� ������ (� ��� ��������� �������-�������)
const int MAX_ARRAY_SIZE = 10000; // ����������� ��������� ����� �������

#pragma endregion

#pragma region �������� ��������� ������������ ������

/**
* @class Arithmetics
*/
static class Arithmetics {

	public:
		Arithmetics() { };
		~Arithmetics() { };

	#pragma region static public functions

		/**
		* @brief ������� ���������� ����� �� ���������
		* @param first_val - ����� ������� ���������
		* @param last_val - ������ ������� ���������
		* @return ��������� ����� �� ���������
		*/
		static float RandomBetween(float first_val, float last_val) {
			float diff = last_val-first_val;
			float rest = ((float) rand()) / (float) RAND_MAX * diff;
			float random_number = first_val+rest;
			return random_number;
		}

		/**
		* @brief ������� ���������� �� ����� ������������ �������������� �������, ������������ � ������������ �������� ���������� ����� �����
		* @param grid_idx - ����������� ������������� ������, ������������ �������� ��������� �������� ��������� �� ����� ������
		* @param steps - ���������� ������� �� ������ �������� ������� �����. ��� ���������� �������� ���������� �������
		* @return ��������� �� ����� ����������� ������������� ������
		*/
		static int *GetNextGridIndex(int *grid_idx, int steps) {
			if (grid_idx != NULL) { // ������� ������������, ���� ����������� ������ ����������
				int rest = 1; // "�������" ��� ��������
				for (int i = 1; i < MAX_DIM; i++) {
					int tmp = grid_idx[i];
					grid_idx[i] = (tmp+rest) % steps;
					rest = (tmp+rest) / steps;
				}
			}
			return grid_idx;
		}

	#pragma endregion

};

#pragma endregion

#endif