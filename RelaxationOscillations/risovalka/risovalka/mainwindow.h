#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

// ��������������� ������������ �����

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

#define MAX_APPSCREEN_SIZE 5000 // ����������� ��������� ����� ���� ����������

// ���������-���� �� ������ �� ���������������� ������� ��� ���������

// ���� �� ����� � ���������� ������������ �������� ����������� ������
static const char *path_to_file_parameters = "..\\risovalka\\risovalka\\parameters.txt";
//static const char *path_to_file_parameters = "parameters.txt";

// ���� �� ����� � ������������ �������� ��������
static const char *path_to_file_trajectories = "..\\risovalka\\risovalka\\trajectories.txt";
//static const char *path_to_file_trajectories = "trajectories.txt";

// ���� �� ����� � ������������ ����������� ���������
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
        QwtPlot *plot; // ������������ ���������
        QwtPlotGrid *plotGrid; // ��������� �����
        QwtText xtitle, ytitle; // ������� � ������������ ����
        QString bottom_str, left_str; // ����� �������� � ��������
        QWidget *appWidget; // ������, ���������� ��� ���������� ����������
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
        void assemblyMainWindow(); // ������ ��������� ���� ����������
        void createDoubleSpinBoxes();
        void createPlotForDrawing();
        void createPlotGrid();
        void createApplicationWidget();
        void createButtons();
        void createLabels();
        void createLayouts();
        void assemblyApplicationElements(); // ������ ��������� ����������
        void setAxisTitles(); // ��������� ��� ���������
        void setConnections(); // ���������� ��������� ���� � ���������
        void setViewMainWindow(); // ��������� ���� ��������� ���� ����������
        void UpdateDoubleSpinBoxes(); // ���������� ����������� ������� DoubleSpinBox
        void refreshPhasePortrait(); // ���������� �������� ��������
        void setPlotEdges(double x, double y); // ��������� ����� �������, ������������ ������� �������

    private slots:

        void refreshWindow(); // ���������� ����������� ���� ����������
        void takeScreenshot(); // ������������ ��������� ���� ���������

};

#endif	// MAINWINDOW_H
