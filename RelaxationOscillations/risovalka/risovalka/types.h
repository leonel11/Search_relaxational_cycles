#ifndef TYPES_H
#define TYPES_H

// ������������ ����������� ������������ �����

#include <iostream>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <limits>
#include <string>

// ������������ ����������� ������������ ���

using namespace std;

#define MAX_ARRAY_SIZE 10000 // ����������� ��������� ����� �������

/**
  * @class TypesChange
  */
class TypesChange {

    public:

        TypesChange() { };
        ~TypesChange() { };

        /**
          * @brief ������� ������, ��������������� �� ������������� �����
          * @param numb - ������������ �����
          * @return ������, �������� � ���� ������������ �����
          */
        static string doubleToString(double numb) {
            char tmp[MAX_ARRAY_SIZE];
            sprintf(tmp, "%lf", numb);
            string res = tmp;
            // ����������� ���� �����, ������� ���� ����� ��������� �����,
            // �� ������� ����� ��������� ������ ������������� �����
            res = res.substr(0, res.find_last_not_of('0')+1);
            // ������, ����� ���� �������� ��� ����� �� ������� ����� ������-������������� �����
            if (res[res.size()-1] == '.')
                res += "0"; // ����� ����� ������������ ����������� ���� 0
            return res;
        }

        /**
          * @brief ������� ������, ��������������� �� ������ �����
          * @param numb - ����� �����
          * @return ������, �������� � ���� ����� �����
          */
        static string intToString(int numb) {
            char buffer[MAX_ARRAY_SIZE];
            itoa(numb, buffer, 10);
            string res = string(buffer);
            // ���� � ����� ����������� ������� ��������� ����������� ������ ������ '\0'
            if (res[res.size()-1] == char(10))
                res = res.substr(0, strlen(buffer)-1); // �� ���� ������ ��������� �� ������
            return res;
        }

};

#endif // TYPES_H
