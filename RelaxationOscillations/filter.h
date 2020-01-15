#pragma region ��������������� ������������ �����

#include "parser.h"

#pragma endregion

#ifndef FILTER_H
#define FILTER_H

#pragma region ���� �� ����� ������ �����

static const char *path_to_output_file = "different_stationary_points.txt"; // ���� ��� ������ ��������� ������������ �����

#pragma endregion

#pragma region �������� ��������� ������

/**
* @class FilterPoints
*/
	class FilterPoints {

		public:
			FilterPoints(DataParser config);
			void AddPoint(float *point); // ���������� �����
			void ExcludeEqualPoints(); // ���������� ������������� �����
			~FilterPoints();

		private:
			void DeleteEqualPoints(); // �������� ������������� �����
			void OutputPoints(); // ����� �����

		private:
			vector<vector<float>> points; // ������ ���� ����� �������
			FILE *output_file; // ���� ��� ������ ����������
			float dim; // ����������� ������ (���������� ���������� ��� ������ ������ ����������� ��������� �����)
			float eps; // �������� ��������� ���� �����

	};

#pragma endregion

#endif