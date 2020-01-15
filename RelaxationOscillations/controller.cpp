#include "controller.h"

Controller::Controller() {
	config.PullData();
	AllocWorkThreads();
}

/**
* @brief Выделение рабочих потоков для вычислений
*/
void Controller::AllocWorkThreads() {
	work_threads = 1;
	if (config.Computing_Method() == "NUMERIC")
		work_threads = MAX_THREADS;
}

/**
* @brief Запуск работы контроллера.
* Контроллер последовательно, в нужном порядке запускает функции, составлящие основной функционал данной программы
*/
void Controller::Run() {
	if (config.ReadingState()) {
		OpenAuxFiles();
		RunCalculations();
		CloseAuxFiles();
		UniteFiles();
	    DrawPhasePortrait();
	}
}

/**
* @brief Открытие всех вспомогательных файлов
*/
void Controller::OpenAuxFiles() {
	file_parameters = fopen(path_to_file_parameters, "wt");
	file_all_stationary_points = fopen(path_to_file_all_stationary_points, "wt");
	file_start_points = fopen(path_to_file_start_points, "rt");
	string pth = "../risovalka/risovalka/trajectories.txt";
	file_united_trajectories = fopen(pth.c_str(), "wt");
	for (int i=0; i<work_threads; i++) {
		string pth = dir_to_temp_file_trajectories + TypesChange::IntToString(i) + ".txt";
		file_trajectories[i] = fopen(pth.c_str(), "wt");
	}
}

/**
* @brief Поиск стационарного состояния для данной точки (solution) и ее добавление в фильтр различных точек (filter)
* @param solution - точка, для которой будет осуществляться поиск
* @param filter - фильтр различных точек
*/
void Controller::SearchStationaryPoint(float *solution, FilterPoints *filter) {
	int iter = 0;
	int dim = config.Dim();
	RelaxarionOscillations calculator(config, solution);
	int cur_thread = omp_get_thread_num();
	fprintf(file_trajectories[cur_thread], "\n");
	if (dim != 1)
		fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(solution, 1, dim).c_str());
	float *prev_sln = new float[MAX_DIM]; // решение, полученное на  предыдущей итерации
	PointOperations::InitPoint(prev_sln, solution); // в начальный момент времени считается, что ранее координаты точки не менялись
	for (iter = 0; iter < config.Max_Iter(); ++iter) {
		// во всех итерациях, кроме первой, нужно сначала установить в качестве начальной точки решение, полученное на прошлом шаге
		if (iter != 0)
			calculator.SetSolution(prev_sln);
		calculator.CalculateSolution();
		float *cur_sln = new float[MAX_DIM];
		PointOperations::InitPoint(cur_sln, calculator.GetSolution());
		if (dim == 1) { // в случае одномерной задачи
			// необходимо сформировать двумерную точку вида (y_n, y_n+1), "склеив" 2 одномерные, полученные на предыдущей и текущей итерации 
			float *sln_2d = new float[MAX_DIM];
			PointOperations::InitPoint(sln_2d, PointOperations::GluePoint(prev_sln, dim, cur_sln, dim));
			// вывод "склеенной" двумерной точки в файл траекторий
			fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(sln_2d, 1, 2*dim).c_str());
		}
		else
			fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(cur_sln, 1, dim).c_str()); // вывод точки в файл траекторий
		// если полученная точка оказалась стационарной, т.е. после релаксационных колебаний координаты точки изменяются несильно
		if (PointOperations::IsEqualPoints(prev_sln, cur_sln, config.Eps())) { 
			// добавление полученной стационарной точки в список-хранилище всех стационарных точек
			filter->AddPoint(cur_sln);
			// вывод стационарной точки в необходимые файлы
			fprintf(file_trajectories[cur_thread], "\n");
			// печать координат начальной и конечной точки релаксационного отображения
			cout << TypesChange::PointToString(solution, 1, dim) << "\t=>\t" << TypesChange::PointToString(cur_sln, 1, dim) << endl;
			fprintf(file_all_stationary_points, "%s\n", TypesChange::PointCoordinatesToString(cur_sln, 1, dim).c_str());
			break;
		}
		PointOperations::SetPoint(prev_sln, cur_sln);
	}
	if (iter == config.Max_Iter()) { // в том случае если достигнута максимальная итерация, но стационарная точка так и не найдена
		//cout << "Max iteration was reached!"; // вывод соответствующего сообщения
		cout << TypesChange::PointToString(solution, 1, dim) << "\t=>\tMax iteration was reached!" << endl;
		filter->AddPoint(prev_sln);
		fprintf(file_trajectories[cur_thread], "\n");
		fprintf(file_all_stationary_points, "%s\n", TypesChange::PointCoordinatesToString(prev_sln, 1, dim).c_str());
	}
}

/**
* @brief Запуск вычислений для поиска всех устойчивых состояний
*/
void Controller::RunCalculations() {
	int trajectories = 0; // количество траекторий фазового портрета
	int dim = config.Dim();
	// печать координат точек-решений, которые должны получиться исходя из теоретических результатов
	RelaxarionOscillations::DisplayTheoreticalSolutions(config);
	srand(time(NULL));
	// получение практических результатов
	cout << "Calculations:" << endl << endl;
	FilterPoints filter(config);
	float *sln = new float [MAX_DIM]; // массив для хранения координат решения
	PointOperations::InitPoint(sln, config.Start_Point());
	if (config.Points_Source() == "FROM_FILE") 
		// если точки, для которых осуществляются релаксационные колебания хранятся в файле
		while (file_start_points) {
			for (int i = 1; i <= dim; i++)
				!fscanf(file_start_points, "%f", &sln[i]);
			trajectories++;
			PointOperations::SetBoundaryValues(sln, dim, config.Wave_Type());
			SearchStationaryPoint(sln, &filter);
			if (feof(file_start_points))
				break;
		}
	if (config.Points_Source() == "GENERATE_GRID") {
		// если точки, для которых осуществляются релаксационные колебания генерируются в качестве сетки
		int steps = config.Steps();
		int start_points = (int) pow((double) steps, (double) dim);
		int *grid_idx = new int[MAX_DIM];
		PointOperations::ZeroizePoint(grid_idx);
		trajectories = start_points;
		if (config.Computing_Method() == "NUMERIC") {
			#pragma omp parallel for
			for (int i = 0; i < trajectories; i++) {
				float *cur_point = PointOperations::GetGridPoint(sln, grid_idx, config.Discr());
				PointOperations::SetBoundaryValues(cur_point, dim, config.Wave_Type());
				SearchStationaryPoint(cur_point, &filter);
				grid_idx = Arithmetics::GetNextGridIndex(grid_idx, steps);
			}
		}
		if (config.Computing_Method() == "ANALYTIC") {
			for (int i = 0; i < trajectories; i++) {
				float *cur_point = PointOperations::GetGridPoint(sln, grid_idx, config.Discr());
				PointOperations::SetBoundaryValues(cur_point, dim, config.Wave_Type());
				SearchStationaryPoint(cur_point, &filter);
				grid_idx = Arithmetics::GetNextGridIndex(grid_idx, steps);
			}
		}
	}
	if (config.Points_Source() == "GENERATE_RANDOM") {
		// если точки, для которых осуществляются релаксационные колебания генерируются случайным образом
		float *start_point = config.Start_Point();
		float *opposite_point = new float [MAX_DIM];
		PointOperations::InitPoint(opposite_point, PointOperations::GetOppositeGridPoint(start_point, config.Discr(), config.Steps(), config.Dim()));
		trajectories = config.Sample();
		if (config.Computing_Method() == "NUMERIC") {
			#pragma omp parallel for
			for (int i = 0; i < trajectories; i++) {
				float *cur_point = PointOperations::GetRandomPointBetween(start_point, opposite_point);
				PointOperations::SetBoundaryValues(cur_point, dim, config.Wave_Type());
				SearchStationaryPoint(cur_point, &filter);
			}
		}
		if (config.Computing_Method() == "ANALYTC") {
			for (int i = 0; i < trajectories; i++) {
				float *cur_point = PointOperations::GetRandomPointBetween(start_point, opposite_point);
				PointOperations::SetBoundaryValues(cur_point, dim, config.Wave_Type());
				SearchStationaryPoint(cur_point, &filter);
			}
		}
	}
	// вывод необходимых параметров задачи во вспомогательный файл
	fprintf(file_parameters, "%s %s %s %s %s %s %s\n", TypesChange::IntToString(config.Dim()).c_str(), TypesChange::FloatToString(config.Alpha()).c_str(), 
			TypesChange::FloatToString(config.Beta()).c_str(), TypesChange::FloatToString(config.D()).c_str(), TypesChange::FloatToString(config.H()).c_str(), 
			TypesChange::FloatToString(config.Eps()).c_str(), TypesChange::IntToString(trajectories).c_str());
	filter.ExcludeEqualPoints();
	cout << "\n";
}

/**
* @brief Отрисовка фазового портрета с помощью запуска сторонних программ
*/
void Controller::DrawPhasePortrait() {
	int dim = config.Dim(); // запуск сторонних утилит зависит от размерноти задачи, определяющего многомерность фазового портрета
	if ((dim == 1) || (dim == 2)) // в случае одномерной или двумерной задачи
		ProgramOperations::RunExeFile(path_to_app_risovalka);
	if (dim == 3) { // в случае трехмерной задачи
		string launch_params = TypesChange::CharArrayToString((char *) path_to_file_trajectories) + " " + 
							   TypesChange::CharArrayToString((char *) path_to_file_parameters);
		ProgramOperations::RunPythonScript(path_to_app_risovalka3d, launch_params);
	}
}

void Controller::UniteFiles() {
	string pth = "../risovalka/risovalka/trajectories.txt";
	file_united_trajectories = fopen(pth.c_str(), "wt");
	for (int i = 0; i < work_threads; i++) {
		string pth = "../risovalka/risovalka/trajectories"+TypesChange::IntToString(i)+".txt";
		file_trajectories[i] = fopen(pth.c_str(), "rt");
	}
	char buffer[MAX_ARRAY_SIZE];
	for (int i = 0; i < work_threads; i++) {
		while (1) {
			if ( fgets(buffer, MAX_ARRAY_SIZE, file_trajectories[i]) == NULL )
				break;
			fprintf(file_united_trajectories, "%s", buffer);
		}
	}
	for (int i = 0; i < work_threads; i++) {
		fclose(file_trajectories[i]);
		remove(string(dir_to_temp_file_trajectories + TypesChange::IntToString(i) + ".txt").c_str());
	}
	fclose(file_united_trajectories);
}

/**
* @brief Закрытие всех вспомогательных файлов
*/
void Controller::CloseAuxFiles() {
	for (int i=0; i<work_threads; i++)
		fclose(file_trajectories[i]);
	fclose(file_united_trajectories);	
	fclose(file_parameters);
	fclose(file_start_points);
	fclose(file_all_stationary_points);
}

Controller::~Controller() {
}