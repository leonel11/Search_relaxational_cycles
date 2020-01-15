#pragma region Вспомогательные заголовочные файлы

#include "types.h"

#pragma endregion

#ifndef PROGRAM_H
#define PROGRAM_H

#pragma region Описание структуры класса

/**
* @class ProgramOperations
*/
static class ProgramOperations {

	public:

	#pragma region static public functions

		/**
		* @brief Запуск стороннего exe-файла
		* @param app - строка, хранящая в себе путь до приложения-экзешника
		*/
		static void RunExeFile(string app) {
			WinExec(app.c_str(), 1);
		}

		/**
		* @brief Запуск стороннего скрипта, написанного на языке Python
		* @param script - строка, хранящая в себе путь до запускаемого питоновского файла
		* @param launch_params - строка с входными параметрами, необходимыми для работы скрипта
		* @return true, если скрипт сработал успешно и корректно, false - иначе
		*/
		static bool RunPythonScript(string script, string launch_params) {
			char buffer[MAX_ARRAY_SIZE]; // выходящая информация из скрипта
			string callcommand = ""; // строка вызова для функции _popen
			callcommand = "python " + script + " " + launch_params; // формирование вызываемой в функции _popen строки
			FILE *script_file = NULL;
			if (!(script_file = _popen(callcommand.c_str(), "r"))) { // в том случае, если скрипт не выполнился
				cout << "Python script doesn't launch correctly!"; // вывод соответствующего сообщения
				return false;
			}
			while (fgets(buffer, sizeof(buffer), script_file) != NULL) // выгрузка выходящей из скрипта информации о его работе
				cout << buffer;
			_pclose(script_file);
			return true;
		}

	#pragma endregion

};

#pragma endregion

#endif