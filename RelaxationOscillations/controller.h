#pragma region ��������������� ������������ �����

#include "calculation.h"
#include "filter.h"

#pragma endregion

#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma region ��������� ������

const int MAX_THREADS = 4; // ������������ ���������� �������, � ������� �������� ���������

#pragma endregion

#pragma region ���� �� ��������������� ������ � ����������

// ���������� � ��������� ������ � ������������ �������� ��������
static string dir_to_temp_file_trajectories = "../risovalka/risovalka/trajectories";
// ���� � ����� ���������� �������� ��������
static const char *path_to_file_trajectories = "../risovalka/risovalka/trajectories.txt";
// ���� � ����� ��������� ��������� ���������� ������ (dim, alpha, beta, d, h, eps)
static const char *path_to_file_parameters = "../risovalka/risovalka/parameters.txt";
// ���� � ����� �� ����� ���������� ������������� �������
static const char *path_to_file_all_stationary_points = "all_stationary_points.txt";
// ���� � ����� � ���������� �������, ��� ������� ����� �������������� ���������� ���������
static const char *path_to_file_start_points = "start_points.txt";
// ���� � ����������, ��������������� ��������� (� ����������) ������� ��������
static string path_to_app_risovalka = "../risovalka/risovalka/debug/risovalka.exe";
// ���� � ����������, ��������������� ���������� ������� ��������
static string path_to_app_risovalka3d = "../risovalka/risovalka/risovalka3d.py";

#pragma endregion

#pragma region �������� ��������� ������

/**
* @class Controller
*/
	class Controller {

		public:
			Controller();
			void Run(); // ������ ������ ����������� 
			~Controller();

		private:
			void AllocWorkThreads(); // ��������� ������� ������� ��� ����������
			void OpenAuxFiles(); // �������� ���� ��������������� ������
			void CloseAuxFiles(); // �������� ���� ��������������� ������
			void RunCalculations(); // ���������� ���� ���������� ���������
			void SearchStationaryPoint(float *solution, FilterPoints *filter); // ����� ������������� ��������� ��� ������ �����
			void DrawPhasePortrait(); // ��������� �������� ��������
			void UniteFiles();

		private:
			FILE *file_trajectories[4];
			FILE *file_united_trajectories;
			FILE *file_parameters;
			FILE *file_start_points;
			FILE *file_all_stationary_points; 
			DataParser config;
			int work_threads; // ���������� ������� �������, �� ������� �������������� ����������� ����������

	};

#pragma endregion

#endif