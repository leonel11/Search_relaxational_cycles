#pragma region Вспомогательные заголовочные файлы

#include "arithmetic.h"

#pragma endregion

#ifndef OPERATIONS_H
#define OPERATIONS_H

#pragma region Описание структуры статического класса

/**
* @class PointOperations
*/
static class PointOperations {

	public:
		PointOperations() { };
		~PointOperations() { };

	#pragma region static public functions

		/**
		* @brief Обнуление координат вектора-точки с вещественными координатами
		* @param point - вектор-точка, чьи координаты требуется обнулить
		*/
		static void ZeroizePoint(float *point = NULL) { 
			if (point != NULL) // процесс начинается в том случае, если обнуляемая точка существует
				for (int i = 0; i < MAX_DIM; i++) 
					point[i] = 0.0; 
		};

		/**
		* @brief Обнуление координат вектора-точки с целочисленными координатами
		* @param point - вектор-точка, чьи координаты требуется обнулить
		*/
		static void ZeroizePoint(int *point = NULL) { 
			if (point != NULL) // процесс начинается в том случае, если обнуляемая точка существует
				for (int i = 0; i < MAX_DIM; i++) 
					point[i] = 0; 
		};

		/**
		* @brief Установка в изначальную точку новых значений координат другой точки
		* @param point - изначальная точка, в которую требуется установить новые значения координат
		* @param other_point - точка с необходимыми значениями координат
		*/
		static void SetPoint(float *point = NULL, float *other_point = NULL) { 
			if ((point != NULL) && (other_point != NULL)) // процесс начинается, если обе точки существуют
				for (int i = 0; i < MAX_DIM; i++)
					point[i] = other_point[i];
		};

		// инициализация изначальной точки (point) другой точкой (other_point)
		/**
		* @brief Инициализация изначальной точкй другой точкой
		* @param point - изначальная точка, которую необходимо инициализировать
		* @param other_point - точка с необходимыми значениями координат
		*/
		static void InitPoint(float *point = NULL, float *other_point = NULL) {
			ZeroizePoint(point);
			SetPoint(point, other_point);
		}

		/**
		* @brief Проверка пары точек на эквивалентность (близость).
		* 2 точки эквивалентны, если каждая координата одной точки отличается от соответствующей координаты другой точки не более чем на определеную, очень малую величину
		* @param point1 - первая точка
		* @param point2 - вторая точка
		* @param eps - малая величина сравнения координат двух точек
		* @return true, если точки эквивалентны, false - иначе
		*/
		static bool IsEqualPoints(float *point1, float *point2, float eps) {
			if ((point1 != NULL) && (point2 != NULL)) // процесс начинается, если обе точки существуют 
				for (int i = 0; i < MAX_DIM; ++i)
					if ((fabs(point1[i] - point2[i]) >= eps))
						return false;
			return true;
		}

		/**
		* @brief Возврат сгенерированной точки сетки
		* @param start_grid_point - изначальная точка, относительно которой генерируется сетка
		* @param grid_idx - многомерный целочисленный индекс, определяющий на какое расстояние по каждой проекции необходимо уйти от изначальной точки
		* @param discr - растояние между ближайшими координатами соседних точек. Этой величине кратно расстояние по каждой проекции, на которое необходимо уйти от начальной точки
		* @return сгенерированная точка сетки
		*/
		static float *GetGridPoint(float *start_grid_point, int *grid_idx, float discr) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			// процесс начинается, если начальная точка сетки и многомерный индекс для генерации существуют
			if ((start_grid_point != NULL) && (grid_idx != NULL)) 
				for (int i = 0; i < MAX_DIM; i++)
					res[i] = start_grid_point[i]+grid_idx[i]*discr;
			return res;
		}

		static void SetBoundaryValues(float *point, int dim, string type_bound_conds) {
			if ((point != NULL) && (dim > 0) && (dim < MAX_DIM-1) && ((type_bound_conds == "NEUMAN") || (type_bound_conds == "PERIODIC") || (type_bound_conds == "UNIWAVE"))) {
				if (type_bound_conds == "PERIODIC")
					point[0] = point[dim+1] = -SumOfCoordinates(point, 1, dim);
				else if (type_bound_conds == "NEUMAN")
					point[0] = point[dim+1] = 0.0;
				else {
					point[0] = 0;
					point[dim+1] = -SumOfCoordinates(point, 1, dim);
				}
			}
		}

		/**
		* @brief Возврат другой точки сетки, противоположной начальной (относительно "центральной" точки сетки)
		* @param start_grid_point - стартовая точка, относительно которой подсчитываются координаты противоположной точки сетки
		* @param discr, steps - параметры, определяющие величину отступа (discr*steps) по значениям каждой координаты относительно начальной точки
		* @param dim - размерность задачи, определяющая количество координат противоположной точки, по которым нужно сделать смещение в сторону относительно начальной точки сетки
		* @return точка сетки, противоположная начальной 
		*/
		static float *GetOppositeGridPoint(float *start_grid_point, float discr, float steps, float dim) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if (start_grid_point != NULL) // процесс начинается, если стартовая точка генерации сетки существует
				for (int i = 1; i <= dim; i++)
					res[i] = start_grid_point[i]+discr*steps;
			return res;
		}

		/**
		* @brief Возврат случайной точки из многомерного кубика
		* @param start_point, opposite_point - противоположные точки многомерного кубика. Они определяют многомерный кубик, из которого будет браться некоторая случайная точка
		* @return случайная точка из многоменого кубика
		*/
		static float *GetRandomPointBetween(float *start_point, float *opposite_point) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if ((start_point != NULL) && (opposite_point != NULL)) // процесс начинается, если обе точки существуют 
				for (int i = 0; i < MAX_DIM; i++) {
					float first_val = min(start_point[i], opposite_point[i]);
					float last_val = max(start_point[i], opposite_point[i]);
					res[i] = Arithmetics::RandomBetween(first_val, last_val);
				}
			return res;
		}

		/**
		* @brief Возврат "склеенной" точки
		* @param point1 - первая точка
		* @param dim1 - количество координат первой точки
		* @param point2 - вторая точка
		* @param dim2 - количество координат второй точки
		* @return "склеенная" точка, в которой последовательно записаны сначала координат первой точки, а потом второй.
		* В том случае, если количество координат обеих точек превышает максимально возможную размерность, в "склееную" точку записывается максимально возможное
		* (поскольку размерность первой точки не может превышать максимально допустимое значение, гарантированно все координаты первой точки можно записать в "склеенную").
		*/
		static float *GluePoint(float *point1, int dim1, float *point2, int dim2) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if ((point1 != NULL) && (point2 != NULL)) { // процесс начинается, если обе точки существуют 
				int i = 0;
				while (i < MAX_DIM) {
					if (i <= dim1) // запись координат первой точки в "склеенную" точку
						res[i] = point1[i];
					if ((i > dim1) && (i <= dim1+dim2)) // запись координат второй точки в "склеенную" точку
						res[i] = point2[i-dim1];
					i++;
				}
			}
			return res;
		}

		static float SumOfCoordinates(float *point, int first_pos, int last_pos) {
			float res = 0.0;
			if (point != NULL)
				for (int i = first_pos; i <= last_pos; i++) 
					res += point[i];
			return res;
		}

	#pragma endregion

};

#pragma endregion

#endif