/** @file main.cpp
* ��������, ����������� ���� �������
*/

#include <QtGui/QApplication>
#include "mainwindow.h"

/**
* @brief ������� ������� �������
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
