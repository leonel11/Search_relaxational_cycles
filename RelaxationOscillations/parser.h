#pragma region Вспомогательные заголовочные файлы

#include "types.h"
#include "program.h"

#pragma endregion

#ifndef PARSER_H
#define PARSER_H

#pragma region Константы для запуска скрипта-парсера

const string pyscriptname = "parser.py"; // название запускаемого питоновского скрипта - парсера конфигурационного файла
const string configfile = "data.config"; // конфигурационный файл, в котором хранятся входные параметры программы
const string readsection = "data_program"; // секция, определяющая какие данные следует считывать из конфигурационного файла

#pragma endregion

#pragma region Описание структуры класса

/**
* @class DataParser
*/
class DataParser {

	public:
		DataParser();
		~DataParser();
		void PullData(); // вытягивание входных параметров программы

	#pragma region get-functions (as get-"properties" in C Sharp)

		int Dim() { return dim; };
		//string Wave_Direction() { return wave_direction; };
		//string Boundary_Conditions() { return boundary_conditions; };
		string Wave_Type() { return wave_type; };
		string Computing_Method() { return computing_method; };
		string Points_Source() { return points_source; };
		float *Start_Point() { return start_point; };
		float Discr() { return discr; };
		int Steps() { return steps; };
		int Sample() { return sample; }
		float D() { return d; };
		float Alpha() { return alpha; };
		float Beta() { return beta; };
		int Max_Iter() { return max_iter; };
		float Eps() { return eps; };
		float H() { return h; };
		bool ReadingState() { return reading_state; };

	#pragma endregion

	private:
		int dim;
		//string wave_direction;
		//string boundary_conditions;
		string wave_type;
		string computing_method;
		string points_source;
		float start_point[MAX_DIM];
		float discr;
		int steps;
		int sample;
		float d; 
		float alpha; 
		float beta;
		int max_iter;
		float eps;
		float h;
		bool reading_state; // состояние считывания данных из конфигурационного файла.
							// принимает значение true, если считывание прошло успешно, false - нет.
		bool IsParseData(); // распарсивание данных из конфигурационного файла
		void ReadData(FILE *aux_input_file); // cчитывание входных параметров программы
};

#pragma endregion

#endif