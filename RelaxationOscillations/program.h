#pragma region ��������������� ������������ �����

#include "types.h"

#pragma endregion

#ifndef PROGRAM_H
#define PROGRAM_H

#pragma region �������� ��������� ������

/**
* @class ProgramOperations
*/
static class ProgramOperations {

	public:

	#pragma region static public functions

		/**
		* @brief ������ ���������� exe-�����
		* @param app - ������, �������� � ���� ���� �� ����������-���������
		*/
		static void RunExeFile(string app) {
			WinExec(app.c_str(), 1);
		}

		/**
		* @brief ������ ���������� �������, ����������� �� ����� Python
		* @param script - ������, �������� � ���� ���� �� ������������ ������������ �����
		* @param launch_params - ������ � �������� �����������, ������������ ��� ������ �������
		* @return true, ���� ������ �������� ������� � ���������, false - �����
		*/
		static bool RunPythonScript(string script, string launch_params) {
			char buffer[MAX_ARRAY_SIZE]; // ��������� ���������� �� �������
			string callcommand = ""; // ������ ������ ��� ������� _popen
			callcommand = "python " + script + " " + launch_params; // ������������ ���������� � ������� _popen ������
			FILE *script_file = NULL;
			if (!(script_file = _popen(callcommand.c_str(), "r"))) { // � ��� ������, ���� ������ �� ����������
				cout << "Python script doesn't launch correctly!"; // ����� ���������������� ���������
				return false;
			}
			while (fgets(buffer, sizeof(buffer), script_file) != NULL) // �������� ��������� �� ������� ���������� � ��� ������
				cout << buffer;
			_pclose(script_file);
			return true;
		}

	#pragma endregion

};

#pragma endregion

#endif