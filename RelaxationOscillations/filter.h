#pragma region Вспомогательные заголовочные файлы

#include "parser.h"

#pragma endregion

#ifndef FILTER_H
#define FILTER_H

#pragma region Путь до файла вывода точек

static const char *path_to_output_file = "different_stationary_points.txt"; // файл для вывода различных стационарных точек

#pragma endregion

#pragma region Описание структуры класса

/**
* @class FilterPoints
*/
	class FilterPoints {

		public:
			FilterPoints(DataParser config);
			void AddPoint(float *point); // добавление точки
			void ExcludeEqualPoints(); // исключение эквивалентных точек
			~FilterPoints();

		private:
			void DeleteEqualPoints(); // удаление эквивалентных точек
			void OutputPoints(); // вывод точек

		private:
			vector<vector<float>> points; // список всех точек фильтра
			FILE *output_file; // файл для вывода информации
			float dim; // размерность задачи (переменная необходима для вывода только необходимых координат точек)
			float eps; // точность сравнения двух точек

	};

#pragma endregion

#endif