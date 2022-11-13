/** @file main.cpp
* Основной, запускаемый файл проекта
*/

#include <QtGui/QApplication>
#include "mainwindow.h"

/**
* @brief Функция запуска проекта
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
