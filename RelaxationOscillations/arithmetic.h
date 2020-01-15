#ifndef STD_THINGS 
#define STD_THINGS

#pragma region Используемые стандартные заголовочные файлы

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

#pragma region Используемые стандартные пространства имён

using namespace std;

#pragma endregion

#endif

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#pragma region Константы-размерности класса

const int MAX_DIM = 20; // максимально возможная размерность задачи (и как следствие вектора-решения)
const int MAX_ARRAY_SIZE = 10000; // максимально возможная длина массива

#pragma endregion

#pragma region Описание структуры статического класса

/**
* @class Arithmetics
*/
static class Arithmetics {

	public:
		Arithmetics() { };
		~Arithmetics() { };

	#pragma region static public functions

		/**
		* @brief Возврат случайного числа из интервала
		* @param first_val - левая граница интервала
		* @param last_val - правая граница интервала
		* @return случайное число из интервала
		*/
		static float RandomBetween(float first_val, float last_val) {
			float diff = last_val-first_val;
			float rest = ((float) rand()) / (float) RAND_MAX * diff;
			float random_number = first_val+rest;
			return random_number;
		}

		/**
		* @brief Возврат следующего по счету многомерного целочисленного индекса, участвующего в формировании значений коорднинат точек сетки
		* @param grid_idx - многомерный целочисленный индекс, относительно которого требуется получить следующий по счету индекс
		* @param steps - количество делений по каждой проекции области сетки. Оно определяет величину увеличения индекса
		* @return следующий по счету многомерный целочисленный индекс
		*/
		static int *GetNextGridIndex(int *grid_idx, int steps) {
			if (grid_idx != NULL) { // процесс продолжается, если многомерный индекс существует
				int rest = 1; // "перенос" при сложении
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