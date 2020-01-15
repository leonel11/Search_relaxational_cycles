#pragma region ��������������� ������������ �����

#include "types.h"
#include "program.h"

#pragma endregion

#ifndef PARSER_H
#define PARSER_H

#pragma region ��������� ��� ������� �������-�������

const string pyscriptname = "parser.py"; // �������� ������������ ������������ ������� - ������� ����������������� �����
const string configfile = "data.config"; // ���������������� ����, � ������� �������� ������� ��������� ���������
const string readsection = "data_program"; // ������, ������������ ����� ������ ������� ��������� �� ����������������� �����

#pragma endregion

#pragma region �������� ��������� ������

/**
* @class DataParser
*/
class DataParser {

	public:
		DataParser();
		~DataParser();
		void PullData(); // ����������� ������� ���������� ���������

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
		bool reading_state; // ��������� ���������� ������ �� ����������������� �����.
							// ��������� �������� true, ���� ���������� ������ �������, false - ���.
		bool IsParseData(); // ������������� ������ �� ����������������� �����
		void ReadData(FILE *aux_input_file); // c��������� ������� ���������� ���������
};

#pragma endregion

#endif