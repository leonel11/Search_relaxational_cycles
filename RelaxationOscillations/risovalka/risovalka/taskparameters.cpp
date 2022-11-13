#include "taskparameters.h"

TaskParameters::TaskParameters(const char *path_to_file_parameters) {
    alpha = beta = d = h = eps = 0.0;
    dim = trajectories = 0;
    file_parameters = fopen(path_to_file_parameters, "rt");
    if (!file_parameters)
        return;
    else
        readParameters();
}

/**
  * @brief Считывание параметров задачи
  */
void TaskParameters::readParameters() {
    fscanf(file_parameters, "%d %lf %lf %lf %lf %lf %d", &dim, &alpha, &beta, &d, &h, &eps, &trajectories);
}

/**
  * @brief Возврат имени скриншота окна программы, сформированного из значений параметров
  * @param directory - путь до нужной директории, где будет храниться скриншот
  */
string TaskParameters::getScreenshotName(string directory) {
    string res = directory + "\\alpha=" + TypesChange::doubleToString(alpha) + " beta=" +
                 TypesChange::doubleToString(beta) + " d=" + TypesChange::doubleToString(d) +
                 " dim=" + TypesChange::intToString(dim) +
                 " trajectories=" + TypesChange::intToString(trajectories) + image_extansion;
    return res;
}

TaskParameters::~TaskParameters() {
    if (file_parameters)
        fclose(file_parameters);
}
