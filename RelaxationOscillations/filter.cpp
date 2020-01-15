#include "filter.h"

FilterPoints::FilterPoints(DataParser config) {
	dim = config.Dim();
	eps = config.Eps();
	output_file = fopen(path_to_output_file, "wt"); // открытие файла вывода различных точек дл€ записи
}

/**
* @brief ƒобавление точки в список всех точек, подлежащих фильтрации
* @param point - добавл€ема€ точка
*/
void FilterPoints::AddPoint(float *point) {
	if (point != NULL) // процесс продолжаетс€ в том случае, если точка существует
		points.push_back(TypesChange::PointToFloatVector(point));
}

/**
* @brief ”даление эквивалентных точек из списка всех точек.
*/
void FilterPoints::DeleteEqualPoints() {
	int i = 0;
	while (i < ((int)points.size()-1)) { // пробег по всем элементам списка различных точек
		// текуща€ точка списка, котора€ будет сравниватьс€ со следующими за ней в списке точками на эквивалентность
		float *cur_sln = new float[MAX_DIM]; 
		PointOperations::InitPoint(cur_sln, &(points.at(i))[0]);
		int j = i+1;
		while (j < points.size()) { // сравнение с остальными точками списка.
			// —равнение идЄт со всеми другими точками списка, которые расположены в списке после текущей 
			// (т.к. с точками, идущими в списке раньше текущей, проверка уже проводилась на более ранних итераци€х)
			float *other_point = new float[MAX_DIM]; // друга€ точка из списка, котора€ будет сравниватьс€ с текущей
			PointOperations::InitPoint(other_point, TypesChange::FloatVectorToPoint(points.at(j)));
			if (PointOperations::IsEqualPoints(cur_sln, other_point, eps)) // в случае, если точки оказались эквивалентными
				points.erase(points.begin()+j); // удаление другой точки списка, которую только что сравнивали с текущей
			else
				j++;
		}
		i++;
	}
}

/**
* @brief ¬ывод списка точек в файл
*/
void FilterPoints::OutputPoints() {
	if (output_file != NULL) { // процесс начинаетс€, если файл дл€ вывода точек открыт дл€ записи
		for (int i = 0; i < points.size(); i++) {
			float cur_point[MAX_DIM];
			PointOperations::InitPoint(cur_point, TypesChange::FloatVectorToPoint(points.at(i)));
			fprintf(output_file, "%s\n", TypesChange::PointToString(cur_point, 1, dim).c_str());
		}
		fclose(output_file); // закрытие файла после вывода списка точек
	}
}

/**
* @brief »сключение эквивалентных точек. ‘ункци€ включает в себ€ этапы удалени€ одинаковых точек и вывода уже "отфильтрованного" списка точек
*/
void FilterPoints::ExcludeEqualPoints() {
	DeleteEqualPoints();
	OutputPoints();
}

FilterPoints::~FilterPoints() {
}