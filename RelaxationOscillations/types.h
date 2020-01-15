#pragma region Вспомогательные заголовочные файлы

#include "operations.h"

#pragma endregion

#ifndef TYPES_H
#define TYPES_H

#pragma region Описание структуры статического класса

/**
* @class TypesChange
*/
static class TypesChange {

	public:
		TypesChange() { };
		~TypesChange() { };

	#pragma region static public functions

		/**
		* @brief Возврат символьного массива, преобразованного из вещественного числа с определенной точностью знаков после запятой
		* @param numb - вещественное число
		* @param precision - точность знаков после запятой. По умолчанию значение этого параметра равно 6
		* @return символьный массив, хранящий в себе вещественное число
		*/
		static char* FloatToCharArray(float numb, int precision = 6) {
			char res[MAX_ARRAY_SIZE];
			// формирование форматной строки для дальнейшего преобразования числа в массив символов
			char buffer[MAX_ARRAY_SIZE];
			itoa(precision, buffer, 10);
			string str_format = "%." + CharArrayToString(buffer) + "f";
			sprintf(res, str_format.c_str(), numb); // перевод числа в массив символов
			return res;
		}

		/**
		* @brief Возврат строки, преобразованной из целого числа
		* @param numb - целое число
		* @return строка, хранящая в себе целое число
		*/
		static string IntToString(int numb) {
			char buffer[MAX_ARRAY_SIZE];
			itoa(numb, buffer, 10);
			string res = CharArrayToString(buffer);
			return res;
		}

		/**
		* @brief Возврат строки, преобразованной из вещественного числа
		* @param numb - вещественное число
		* @return строка, хранящая в себе вещественное число
		*/
		static string FloatToString(float numb) {
			string res = CharArrayToString(FloatToCharArray(numb));
			// выкидывание всех нулей, которые идут после ненулевой цифры, из дробной части строковой записи вещественного числа
			res = res.substr(0, res.find_last_not_of('0')+1); 
			if (res[res.size()-1] == '.') // случай, когда были откинуты все цифры из дробной части строки-вещественного числа
				res += "0"; // после точки искусственно добавляется один 0
			return res;
		}

		/**
		* @brief Возврат строки, преобразованной из массива символов
		* @param symbols - символьный массив
		* @return строка, хранящая в себе все необходимые непустые символы из массива
		*/
		static string CharArrayToString(char symbols[MAX_ARRAY_SIZE]) {
			string res = string(symbols);
			// если в конце символьного массива находится стандартный пустой символ '\0'
			if (res[res.size()-1] == char(10))
				res = res.substr(0, strlen(symbols)-1); // то этот символ удаляется из строки
			return res;
		}

		/**
		* @brief Возврат строки, хранящую определенные вещественные координаты точки в виде ( p1, p2, ..., pn )
		* @param point - точка
		* @param first_pos - начальная позиция координаты точки, которую необходимо перевести в строковый вид
		* @param last_pos - конечная позиция координаты точки
		* @return строка вида ( p1, p2, ..., pn )
		*/
		static string PointToString(float *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) { // процесс начинается в том случае, если точка существует
				res += "(";
				for (int i = first_pos; i <= last_pos; i++) {
					res += " " + CharArrayToString(FloatToCharArray(point[i], 4));
					if (i != last_pos)
						res += ",";
				}
				res += " )";
			}
			return res; 
		}

		/**
		* @brief Возврат строки, хранящую определенные целочисленные координаты точки в виде ( p1, p2, ..., pn )
		* @param point - точка
		* @param first_pos - начальная позиция координаты точки, которую необходимо перевести в строковый вид
		* @param last_pos - конечная позиция координаты точки
		* @return строка вида ( p1, p2, ..., pn )
		*/
		static string IntPointToString(int *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) { // процесс начинается в том случае, если точка существует
				res += "(";
				for (int i = first_pos; i <= last_pos; i++) {
					res += " " + CharArrayToString(FloatToCharArray(point[i], 4));
					if (i != last_pos)
						res += ",";
				}
				res += " )";
			}
			return res; 
		}

		/**
		* @brief Возврат строки, хранящую определенные вещественные координаты точки в виде p1 p2 ... pn
		* @param point - точка
		* @param first_pos - начальная позиция координаты точки, которую необходимо перевести в строковый вид
		* @param last_pos - конечная позиция координаты точки
		* @return строка вида p1 p2 ... pn
		*/
		static string PointCoordinatesToString(float *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) // процесс начинается в том случае, если точка существует
				for (int i = first_pos; i <= last_pos; i++) {
					res += CharArrayToString(FloatToCharArray(point[i], 4));
					if (i != last_pos)
						res += " ";
				}
			return res;
		}

		/**
		* @brief Возврат вектора вещественных чисел, хранящего в себе вещественные координаты точки
		* @param point - точка
		* @return вектор вещественных чисел
		*/
		static vector<float> PointToFloatVector(float *point) {
			float aux_point[MAX_DIM];
			PointOperations::InitPoint(aux_point, point);
			vector<float> res(aux_point, aux_point + sizeof(aux_point) / sizeof(aux_point[0]));
			return res;
		}

		/**
		* @brief Возврат точки, преобразованной из вектора вещественных чисел
		* @param vect_point - вектор вещественных чисел
		* @return точка
		*/
		static float* FloatVectorToPoint(vector<float> vect_point) {
			float *point = new float[MAX_DIM];
			PointOperations::InitPoint(point, &(vect_point)[0]);
			return point;
		}

	#pragma endregion

};

#pragma endregion

#endif