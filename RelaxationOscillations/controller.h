#pragma region Вспомогательные заголовочные файлы

#include "calculation.h"
#include "filter.h"

#pragma endregion

#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma region Константы класса

const int MAX_THREADS = 4; // максимальное количество потоков, в которых работает программа

#pragma endregion

#pragma region Пути до вспомогательных файлов и приложений

// директория к временным файлам с траекториями фазового портрета
static string dir_to_temp_file_trajectories = "../risovalka/risovalka/trajectories";
// путь к файлу траекторий фазового портрета
static const char *path_to_file_trajectories = "../risovalka/risovalka/trajectories.txt";
// путь к файлу некоторых начальных параметров задачи (dim, alpha, beta, d, h, eps)
static const char *path_to_file_parameters = "../risovalka/risovalka/parameters.txt";
// путь к файлу со всеми найденными стационарными точками
static const char *path_to_file_all_stationary_points = "all_stationary_points.txt";
// путь к файлу с начальными точками, для которых будут подсчитываться устойчивые состояния
static const char *path_to_file_start_points = "start_points.txt";
// путь к приложению, отрисовывающему двумерные (и одномерные) фазовые портреты
static string path_to_app_risovalka = "../risovalka/risovalka/debug/risovalka.exe";
// путь к приложению, отрисовывающему трехмерные фазовые портреты
static string path_to_app_risovalka3d = "../risovalka/risovalka/risovalka3d.py";

#pragma endregion

#pragma region Описание структуры класса

/**
* @class Controller
*/
	class Controller {

		public:
			Controller();
			void Run(); // запуск работы контроллера 
			~Controller();

		private:
			void AllocWorkThreads(); // выделение рабочих потоков для вычислений
			void OpenAuxFiles(); // открытие всех вспомогательных файлов
			void CloseAuxFiles(); // закрытие всех вспомогательных файлов
			void RunCalculations(); // вычисление всех устойчивых состояний
			void SearchStationaryPoint(float *solution, FilterPoints *filter); // поиск стационарного состояния для данной точки
			void DrawPhasePortrait(); // отрисовка фазового портрета
			void UniteFiles();

		private:
			FILE *file_trajectories[4];
			FILE *file_united_trajectories;
			FILE *file_parameters;
			FILE *file_start_points;
			FILE *file_all_stationary_points; 
			DataParser config;
			int work_threads; // количество рабочих потоков, на которых осуществляются необходимые вычисления

	};

#pragma endregion

#endif