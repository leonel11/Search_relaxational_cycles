#include "filter.h"

FilterPoints::FilterPoints(DataParser config) {
	dim = config.Dim();
	eps = config.Eps();
	output_file = fopen(path_to_output_file, "wt"); // �������� ����� ������ ��������� ����� ��� ������
}

/**
* @brief ���������� ����� � ������ ���� �����, ���������� ����������
* @param point - ����������� �����
*/
void FilterPoints::AddPoint(float *point) {
	if (point != NULL) // ������� ������������ � ��� ������, ���� ����� ����������
		points.push_back(TypesChange::PointToFloatVector(point));
}

/**
* @brief �������� ������������� ����� �� ������ ���� �����.
*/
void FilterPoints::DeleteEqualPoints() {
	int i = 0;
	while (i < ((int)points.size()-1)) { // ������ �� ���� ��������� ������ ��������� �����
		// ������� ����� ������, ������� ����� ������������ �� ���������� �� ��� � ������ ������� �� ���������������
		float *cur_sln = new float[MAX_DIM]; 
		PointOperations::InitPoint(cur_sln, &(points.at(i))[0]);
		int j = i+1;
		while (j < points.size()) { // ��������� � ���������� ������� ������.
			// ��������� ��� �� ����� ������� ������� ������, ������� ����������� � ������ ����� ������� 
			// (�.�. � �������, ������� � ������ ������ �������, �������� ��� ����������� �� ����� ������ ���������)
			float *other_point = new float[MAX_DIM]; // ������ ����� �� ������, ������� ����� ������������ � �������
			PointOperations::InitPoint(other_point, TypesChange::FloatVectorToPoint(points.at(j)));
			if (PointOperations::IsEqualPoints(cur_sln, other_point, eps)) // � ������, ���� ����� ��������� ��������������
				points.erase(points.begin()+j); // �������� ������ ����� ������, ������� ������ ��� ���������� � �������
			else
				j++;
		}
		i++;
	}
}

/**
* @brief ����� ������ ����� � ����
*/
void FilterPoints::OutputPoints() {
	if (output_file != NULL) { // ������� ����������, ���� ���� ��� ������ ����� ������ ��� ������
		for (int i = 0; i < points.size(); i++) {
			float cur_point[MAX_DIM];
			PointOperations::InitPoint(cur_point, TypesChange::FloatVectorToPoint(points.at(i)));
			fprintf(output_file, "%s\n", TypesChange::PointToString(cur_point, 1, dim).c_str());
		}
		fclose(output_file); // �������� ����� ����� ������ ������ �����
	}
}

/**
* @brief ���������� ������������� �����. ������� �������� � ���� ����� �������� ���������� ����� � ������ ��� "����������������" ������ �����
*/
void FilterPoints::ExcludeEqualPoints() {
	DeleteEqualPoints();
	OutputPoints();
}

FilterPoints::~FilterPoints() {
}