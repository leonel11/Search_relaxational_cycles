#ifndef TYPES_H
#define TYPES_H

// Используемые стандартные заголовочные файлы

#include <iostream>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <limits>
#include <string>

// Используемые стандартные пространства имён

using namespace std;

#define MAX_ARRAY_SIZE 10000 // максимально возможная длина массива

/**
  * @class TypesChange
  */
class TypesChange {

    public:

        TypesChange() { };
        ~TypesChange() { };

        /**
          * @brief Возврат строки, преобразованной из вещественного числа
          * @param numb - вещественное число
          * @return строка, хранящая в себе вещественное число
          */
        static string doubleToString(double numb) {
            char tmp[MAX_ARRAY_SIZE];
            sprintf(tmp, "%lf", numb);
            string res = tmp;
            // выкидывание всех нулей, которые идут после ненулевой цифры,
            // из дробной части строковой записи вещественного числа
            res = res.substr(0, res.find_last_not_of('0')+1);
            // случай, когда были откинуты все цифры из дробной части строки-вещественного числа
            if (res[res.size()-1] == '.')
                res += "0"; // после точки искусственно добавляется один 0
            return res;
        }

        /**
          * @brief Возврат строки, преобразованной из целого числа
          * @param numb - целое число
          * @return строка, хранящая в себе целое число
          */
        static string intToString(int numb) {
            char buffer[MAX_ARRAY_SIZE];
            itoa(numb, buffer, 10);
            string res = string(buffer);
            // если в конце символьного массива находится стандартный пустой символ '\0'
            if (res[res.size()-1] == char(10))
                res = res.substr(0, strlen(buffer)-1); // то этот символ удаляется из строки
            return res;
        }

};

#endif // TYPES_H
