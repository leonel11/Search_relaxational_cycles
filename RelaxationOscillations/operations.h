#pragma region ��������������� ������������ �����

#include "arithmetic.h"

#pragma endregion

#ifndef OPERATIONS_H
#define OPERATIONS_H

#pragma region �������� ��������� ������������ ������

/**
* @class PointOperations
*/
static class PointOperations {

	public:
		PointOperations() { };
		~PointOperations() { };

	#pragma region static public functions

		/**
		* @brief ��������� ��������� �������-����� � ������������� ������������
		* @param point - ������-�����, ��� ���������� ��������� ��������
		*/
		static void ZeroizePoint(float *point = NULL) { 
			if (point != NULL) // ������� ���������� � ��� ������, ���� ���������� ����� ����������
				for (int i = 0; i < MAX_DIM; i++) 
					point[i] = 0.0; 
		};

		/**
		* @brief ��������� ��������� �������-����� � �������������� ������������
		* @param point - ������-�����, ��� ���������� ��������� ��������
		*/
		static void ZeroizePoint(int *point = NULL) { 
			if (point != NULL) // ������� ���������� � ��� ������, ���� ���������� ����� ����������
				for (int i = 0; i < MAX_DIM; i++) 
					point[i] = 0; 
		};

		/**
		* @brief ��������� � ����������� ����� ����� �������� ��������� ������ �����
		* @param point - ����������� �����, � ������� ��������� ���������� ����� �������� ���������
		* @param other_point - ����� � ������������ ���������� ���������
		*/
		static void SetPoint(float *point = NULL, float *other_point = NULL) { 
			if ((point != NULL) && (other_point != NULL)) // ������� ����������, ���� ��� ����� ����������
				for (int i = 0; i < MAX_DIM; i++)
					point[i] = other_point[i];
		};

		// ������������� ����������� ����� (point) ������ ������ (other_point)
		/**
		* @brief ������������� ����������� ����� ������ ������
		* @param point - ����������� �����, ������� ���������� ����������������
		* @param other_point - ����� � ������������ ���������� ���������
		*/
		static void InitPoint(float *point = NULL, float *other_point = NULL) {
			ZeroizePoint(point);
			SetPoint(point, other_point);
		}

		/**
		* @brief �������� ���� ����� �� ��������������� (��������).
		* 2 ����� ������������, ���� ������ ���������� ����� ����� ���������� �� ��������������� ���������� ������ ����� �� ����� ��� �� �����������, ����� ����� ��������
		* @param point1 - ������ �����
		* @param point2 - ������ �����
		* @param eps - ����� �������� ��������� ��������� ���� �����
		* @return true, ���� ����� ������������, false - �����
		*/
		static bool IsEqualPoints(float *point1, float *point2, float eps) {
			if ((point1 != NULL) && (point2 != NULL)) // ������� ����������, ���� ��� ����� ���������� 
				for (int i = 0; i < MAX_DIM; ++i)
					if ((fabs(point1[i] - point2[i]) >= eps))
						return false;
			return true;
		}

		/**
		* @brief ������� ��������������� ����� �����
		* @param start_grid_point - ����������� �����, ������������ ������� ������������ �����
		* @param grid_idx - ����������� ������������� ������, ������������ �� ����� ���������� �� ������ �������� ���������� ���� �� ����������� �����
		* @param discr - ��������� ����� ���������� ������������ �������� �����. ���� �������� ������ ���������� �� ������ ��������, �� ������� ���������� ���� �� ��������� �����
		* @return ��������������� ����� �����
		*/
		static float *GetGridPoint(float *start_grid_point, int *grid_idx, float discr) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			// ������� ����������, ���� ��������� ����� ����� � ����������� ������ ��� ��������� ����������
			if ((start_grid_point != NULL) && (grid_idx != NULL)) 
				for (int i = 0; i < MAX_DIM; i++)
					res[i] = start_grid_point[i]+grid_idx[i]*discr;
			return res;
		}

		static void SetBoundaryValues(float *point, int dim, string type_bound_conds) {
			if ((point != NULL) && (dim > 0) && (dim < MAX_DIM-1) && ((type_bound_conds == "NEUMAN") || (type_bound_conds == "PERIODIC") || (type_bound_conds == "UNIWAVE"))) {
				if (type_bound_conds == "PERIODIC")
					point[0] = point[dim+1] = -SumOfCoordinates(point, 1, dim);
				else if (type_bound_conds == "NEUMAN")
					point[0] = point[dim+1] = 0.0;
				else {
					point[0] = 0;
					point[dim+1] = -SumOfCoordinates(point, 1, dim);
				}
			}
		}

		/**
		* @brief ������� ������ ����� �����, ��������������� ��������� (������������ "�����������" ����� �����)
		* @param start_grid_point - ��������� �����, ������������ ������� �������������� ���������� ��������������� ����� �����
		* @param discr, steps - ���������, ������������ �������� ������� (discr*steps) �� ��������� ������ ���������� ������������ ��������� �����
		* @param dim - ����������� ������, ������������ ���������� ��������� ��������������� �����, �� ������� ����� ������� �������� � ������� ������������ ��������� ����� �����
		* @return ����� �����, ��������������� ��������� 
		*/
		static float *GetOppositeGridPoint(float *start_grid_point, float discr, float steps, float dim) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if (start_grid_point != NULL) // ������� ����������, ���� ��������� ����� ��������� ����� ����������
				for (int i = 1; i <= dim; i++)
					res[i] = start_grid_point[i]+discr*steps;
			return res;
		}

		/**
		* @brief ������� ��������� ����� �� ������������ ������
		* @param start_point, opposite_point - ��������������� ����� ������������ ������. ��� ���������� ����������� �����, �� �������� ����� ������� ��������� ��������� �����
		* @return ��������� ����� �� ����������� ������
		*/
		static float *GetRandomPointBetween(float *start_point, float *opposite_point) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if ((start_point != NULL) && (opposite_point != NULL)) // ������� ����������, ���� ��� ����� ���������� 
				for (int i = 0; i < MAX_DIM; i++) {
					float first_val = min(start_point[i], opposite_point[i]);
					float last_val = max(start_point[i], opposite_point[i]);
					res[i] = Arithmetics::RandomBetween(first_val, last_val);
				}
			return res;
		}

		/**
		* @brief ������� "���������" �����
		* @param point1 - ������ �����
		* @param dim1 - ���������� ��������� ������ �����
		* @param point2 - ������ �����
		* @param dim2 - ���������� ��������� ������ �����
		* @return "���������" �����, � ������� ��������������� �������� ������� ��������� ������ �����, � ����� ������.
		* � ��� ������, ���� ���������� ��������� ����� ����� ��������� ����������� ��������� �����������, � "��������" ����� ������������ ����������� ���������
		* (��������� ����������� ������ ����� �� ����� ��������� ����������� ���������� ��������, �������������� ��� ���������� ������ ����� ����� �������� � "���������").
		*/
		static float *GluePoint(float *point1, int dim1, float *point2, int dim2) {
			float *res = new float[MAX_DIM];
			ZeroizePoint(res);
			if ((point1 != NULL) && (point2 != NULL)) { // ������� ����������, ���� ��� ����� ���������� 
				int i = 0;
				while (i < MAX_DIM) {
					if (i <= dim1) // ������ ��������� ������ ����� � "���������" �����
						res[i] = point1[i];
					if ((i > dim1) && (i <= dim1+dim2)) // ������ ��������� ������ ����� � "���������" �����
						res[i] = point2[i-dim1];
					i++;
				}
			}
			return res;
		}

		static float SumOfCoordinates(float *point, int first_pos, int last_pos) {
			float res = 0.0;
			if (point != NULL)
				for (int i = first_pos; i <= last_pos; i++) 
					res += point[i];
			return res;
		}

	#pragma endregion

};

#pragma endregion

#endif