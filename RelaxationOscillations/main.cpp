/** @file main.cpp
* Основной, запускаемый файл проекта
*/ 

#pragma region Вспомогательные заголовочные файлы

#include "controller.h"

#pragma endregion

/**
* @brief Функция запуска проекта
*/
int main() {
	Controller program;
	program.Run();
	return 0;
}