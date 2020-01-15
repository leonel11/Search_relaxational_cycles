#pragma region ��������������� ������������ �����

#include "operations.h"

#pragma endregion

#ifndef TYPES_H
#define TYPES_H

#pragma region �������� ��������� ������������ ������

/**
* @class TypesChange
*/
static class TypesChange {

	public:
		TypesChange() { };
		~TypesChange() { };

	#pragma region static public functions

		/**
		* @brief ������� ����������� �������, ���������������� �� ������������� ����� � ������������ ��������� ������ ����� �������
		* @param numb - ������������ �����
		* @param precision - �������� ������ ����� �������. �� ��������� �������� ����� ��������� ����� 6
		* @return ���������� ������, �������� � ���� ������������ �����
		*/
		static char* FloatToCharArray(float numb, int precision = 6) {
			char res[MAX_ARRAY_SIZE];
			// ������������ ��������� ������ ��� ����������� �������������� ����� � ������ ��������
			char buffer[MAX_ARRAY_SIZE];
			itoa(precision, buffer, 10);
			string str_format = "%." + CharArrayToString(buffer) + "f";
			sprintf(res, str_format.c_str(), numb); // ������� ����� � ������ ��������
			return res;
		}

		/**
		* @brief ������� ������, ��������������� �� ������ �����
		* @param numb - ����� �����
		* @return ������, �������� � ���� ����� �����
		*/
		static string IntToString(int numb) {
			char buffer[MAX_ARRAY_SIZE];
			itoa(numb, buffer, 10);
			string res = CharArrayToString(buffer);
			return res;
		}

		/**
		* @brief ������� ������, ��������������� �� ������������� �����
		* @param numb - ������������ �����
		* @return ������, �������� � ���� ������������ �����
		*/
		static string FloatToString(float numb) {
			string res = CharArrayToString(FloatToCharArray(numb));
			// ����������� ���� �����, ������� ���� ����� ��������� �����, �� ������� ����� ��������� ������ ������������� �����
			res = res.substr(0, res.find_last_not_of('0')+1); 
			if (res[res.size()-1] == '.') // ������, ����� ���� �������� ��� ����� �� ������� ����� ������-������������� �����
				res += "0"; // ����� ����� ������������ ����������� ���� 0
			return res;
		}

		/**
		* @brief ������� ������, ��������������� �� ������� ��������
		* @param symbols - ���������� ������
		* @return ������, �������� � ���� ��� ����������� �������� ������� �� �������
		*/
		static string CharArrayToString(char symbols[MAX_ARRAY_SIZE]) {
			string res = string(symbols);
			// ���� � ����� ����������� ������� ��������� ����������� ������ ������ '\0'
			if (res[res.size()-1] == char(10))
				res = res.substr(0, strlen(symbols)-1); // �� ���� ������ ��������� �� ������
			return res;
		}

		/**
		* @brief ������� ������, �������� ������������ ������������ ���������� ����� � ���� ( p1, p2, ..., pn )
		* @param point - �����
		* @param first_pos - ��������� ������� ���������� �����, ������� ���������� ��������� � ��������� ���
		* @param last_pos - �������� ������� ���������� �����
		* @return ������ ���� ( p1, p2, ..., pn )
		*/
		static string PointToString(float *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) { // ������� ���������� � ��� ������, ���� ����� ����������
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
		* @brief ������� ������, �������� ������������ ������������� ���������� ����� � ���� ( p1, p2, ..., pn )
		* @param point - �����
		* @param first_pos - ��������� ������� ���������� �����, ������� ���������� ��������� � ��������� ���
		* @param last_pos - �������� ������� ���������� �����
		* @return ������ ���� ( p1, p2, ..., pn )
		*/
		static string IntPointToString(int *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) { // ������� ���������� � ��� ������, ���� ����� ����������
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
		* @brief ������� ������, �������� ������������ ������������ ���������� ����� � ���� p1 p2 ... pn
		* @param point - �����
		* @param first_pos - ��������� ������� ���������� �����, ������� ���������� ��������� � ��������� ���
		* @param last_pos - �������� ������� ���������� �����
		* @return ������ ���� p1 p2 ... pn
		*/
		static string PointCoordinatesToString(float *point, int first_pos, int last_pos) {
			string res = "";
			if (point != NULL) // ������� ���������� � ��� ������, ���� ����� ����������
				for (int i = first_pos; i <= last_pos; i++) {
					res += CharArrayToString(FloatToCharArray(point[i], 4));
					if (i != last_pos)
						res += " ";
				}
			return res;
		}

		/**
		* @brief ������� ������� ������������ �����, ��������� � ���� ������������ ���������� �����
		* @param point - �����
		* @return ������ ������������ �����
		*/
		static vector<float> PointToFloatVector(float *point) {
			float aux_point[MAX_DIM];
			PointOperations::InitPoint(aux_point, point);
			vector<float> res(aux_point, aux_point + sizeof(aux_point) / sizeof(aux_point[0]));
			return res;
		}

		/**
		* @brief ������� �����, ��������������� �� ������� ������������ �����
		* @param vect_point - ������ ������������ �����
		* @return �����
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