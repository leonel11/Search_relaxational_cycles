#ifndef TASKPARAMETERS_H
#define TASKPARAMETERS_H

// Используемые вспомогательные заголовочные файлы

#include "types.h"

static string image_extansion = ".png"; // расширение файлов-скришотов программы

/**
  * @class TaskParameters
  */
class TaskParameters
{

    public:

        TaskParameters(const char *path_to_file_parameters);
        string getScreenshotName(string directory); // возврат имени скриншота окна программы
        ~TaskParameters();

        // get-functions (as get-"properties" in C Sharp)

        double Alpha() { return alpha; };
        double Beta() { return beta; };
        double D() { return d; };
        double Eps() { return eps; };
        double H() { return h; };
        int Dim() { return dim; };
        int Trajectories() { return trajectories; };

    private:

        void readParameters(); // считывание параметров задачи

    private:

        FILE *file_parameters;
        double alpha;
        double beta;
        double d;
        double eps;
        double h;
        int dim;
        int trajectories;

};

#endif // TASKPARAMETERS_H
