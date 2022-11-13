#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

// Вспомогательные заголовочные файлы

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QApplication>
#include <QRect>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_text_engine.h>

#include "taskparameters.h"

#define MAX_APPSCREEN_SIZE 5000 // максимально возможная длина окна приложения

// Константы-пути до файлов со вспомогательными данными для программы

// путь до файла с некоторыми необходимыми входными параметрами задачи
static const char *path_to_file_parameters = "..\\risovalka\\risovalka\\parameters.txt";
//static const char *path_to_file_parameters = "parameters.txt";

// путь до файла с траекториями фазового портрета
static const char *path_to_file_trajectories = "..\\risovalka\\risovalka\\trajectories.txt";
//static const char *path_to_file_trajectories = "trajectories.txt";

// путь до папки с сохраняемыми скриштотами программы
static string path_to_folder_with_screenshots = "..\\Screenshots";

/**
  * @class MainWindow
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        MainWindow(QWidget *parent = 0, QApplication *app=0);
        ~MainWindow();

    private:

        QApplication *ap;
        QwtPlot *plot; // координатная плоскость
        QwtPlotGrid *plotGrid; // тетрадная сетка
        QwtText xtitle, ytitle; // подписи к координатным осям
        QString bottom_str, left_str; // текст подписей к графикам
        QWidget *appWidget; // виджет, содержащий все компоненты приложения
        QPushButton *screenshotButton;
        QDoubleSpinBox  *aEdit, *bEdit, *cEdit, *dEdit;
        QLabel *aLabel, *bLabel, *cLabel, *dLabel, *dprogLabel, *alphaLabel, *betaLabel, *epsLabel, *hLabel, *trajecLabel;
        QHBoxLayout *mainLay;
        QVBoxLayout *vertLay, *labelLay;
        QFormLayout *spaceLay;
        QGridLayout *editLay;
        TaskParameters *params;
        FILE *file_trajectories;
        double a, b, c, d, minX, maxX, minY, maxY;
        int screenWidth;
        void assemblyMainWindow(); // сборка основного окна приложения
        void createDoubleSpinBoxes();
        void createPlotForDrawing();
        void createPlotGrid();
        void createApplicationWidget();
        void createButtons();
        void createLabels();
        void createLayouts();
        void assemblyApplicationElements(); // сборка элементов приложения
        void setAxisTitles(); // подписать оси координат
        void setConnections(); // соединение элементов окна с событиями
        void setViewMainWindow(); // установка вида основного окна приложения
        void UpdateDoubleSpinBoxes(); // обновление содержимого каждого DoubleSpinBox
        void refreshPhasePortrait(); // обновление фазового портрета
        void setPlotEdges(double x, double y); // установка краев области, отображающей фазовый портрет

    private slots:

        void refreshWindow(); // обновление содержимого окна приложения
        void takeScreenshot(); // формирование скриншота окна программы

};

#endif	// MAINWINDOW_H
