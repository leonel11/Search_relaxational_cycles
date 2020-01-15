#include "controller.h"

Controller::Controller() {
	config.PullData();
	AllocWorkThreads();
}

/**
* @brief ��������� ������� ������� ��� ����������
*/
void Controller::AllocWorkThreads() {
	work_threads = 1;
	if (config.Computing_Method() == "NUMERIC")
		work_threads = MAX_THREADS;
}

/**
* @brief ������ ������ �����������.
* ���������� ���������������, � ������ ������� ��������� �������, ����������� �������� ���������� ������ ���������
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
* @brief �������� ���� ��������������� ������
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
* @brief ����� ������������� ��������� ��� ������ ����� (solution) � �� ���������� � ������ ��������� ����� (filter)
* @param solution - �����, ��� ������� ����� �������������� �����
* @param filter - ������ ��������� �����
*/
void Controller::SearchStationaryPoint(float *solution, FilterPoints *filter) {
	int iter = 0;
	int dim = config.Dim();
	RelaxarionOscillations calculator(config, solution);
	int cur_thread = omp_get_thread_num();
	fprintf(file_trajectories[cur_thread], "\n");
	if (dim != 1)
		fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(solution, 1, dim).c_str());
	float *prev_sln = new float[MAX_DIM]; // �������, ���������� ��  ���������� ��������
	PointOperations::InitPoint(prev_sln, solution); // � ��������� ������ ������� ���������, ��� ����� ���������� ����� �� ��������
	for (iter = 0; iter < config.Max_Iter(); ++iter) {
		// �� ���� ���������, ����� ������, ����� ������� ���������� � �������� ��������� ����� �������, ���������� �� ������� ����
		if (iter != 0)
			calculator.SetSolution(prev_sln);
		calculator.CalculateSolution();
		float *cur_sln = new float[MAX_DIM];
		PointOperations::InitPoint(cur_sln, calculator.GetSolution());
		if (dim == 1) { // � ������ ���������� ������
			// ���������� ������������ ��������� ����� ���� (y_n, y_n+1), "������" 2 ����������, ���������� �� ���������� � ������� �������� 
			float *sln_2d = new float[MAX_DIM];
			PointOperations::InitPoint(sln_2d, PointOperations::GluePoint(prev_sln, dim, cur_sln, dim));
			// ����� "���������" ��������� ����� � ���� ����������
			fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(sln_2d, 1, 2*dim).c_str());
		}
		else
			fprintf(file_trajectories[cur_thread], " %s\n", TypesChange::PointCoordinatesToString(cur_sln, 1, dim).c_str()); // ����� ����� � ���� ����������
		// ���� ���������� ����� ��������� ������������, �.�. ����� �������������� ��������� ���������� ����� ���������� ��������
		if (PointOperations::IsEqualPoints(prev_sln, cur_sln, config.Eps())) { 
			// ���������� ���������� ������������ ����� � ������-��������� ���� ������������ �����
			filter->AddPoint(cur_sln);
			// ����� ������������ ����� � ����������� �����
			fprintf(file_trajectories[cur_thread], "\n");
			// ������ ��������� ��������� � �������� ����� ��������������� �����������
			cout << TypesChange::PointToString(solution, 1, dim) << "\t=>\t" << TypesChange::PointToString(cur_sln, 1, dim) << endl;
			fprintf(file_all_stationary_points, "%s\n", TypesChange::PointCoordinatesToString(cur_sln, 1, dim).c_str());
			break;
		}
		PointOperations::SetPoint(prev_sln, cur_sln);
	}
	if (iter == config.Max_Iter()) { // � ��� ������ ���� ���������� ������������ ��������, �� ������������ ����� ��� � �� �������
		//cout << "Max iteration was reached!"; // ����� ���������������� ���������
		cout << TypesChange::PointToString(solution, 1, dim) << "\t=>\tMax iteration was reached!" << endl;
		filter->AddPoint(prev_sln);
		fprintf(file_trajectories[cur_thread], "\n");
		fprintf(file_all_stationary_points, "%s\n", TypesChange::PointCoordinatesToString(prev_sln, 1, dim).c_str());
	}
}

/**
* @brief ������ ���������� ��� ������ ���� ���������� ���������
*/
void Controller::RunCalculations() {
	int trajectories = 0; // ���������� ���������� �������� ��������
	int dim = config.Dim();
	// ������ ��������� �����-�������, ������� ������ ���������� ������ �� ������������� �����������
	RelaxarionOscillations::DisplayTheoreticalSolutions(config);
	srand(time(NULL));
	// ��������� ������������ �����������
	cout << "Calculations:" << endl << endl;
	FilterPoints filter(config);
	float *sln = new float [MAX_DIM]; // ������ ��� �������� ��������� �������
	PointOperations::InitPoint(sln, config.Start_Point());
	if (config.Points_Source() == "FROM_FILE") 
		// ���� �����, ��� ������� �������������� �������������� ��������� �������� � �����
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
		// ���� �����, ��� ������� �������������� �������������� ��������� ������������ � �������� �����
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
		// ���� �����, ��� ������� �������������� �������������� ��������� ������������ ��������� �������
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
	// ����� ����������� ���������� ������ �� ��������������� ����
	fprintf(file_parameters, "%s %s %s %s %s %s %s\n", TypesChange::IntToString(config.Dim()).c_str(), TypesChange::FloatToString(config.Alpha()).c_str(), 
			TypesChange::FloatToString(config.Beta()).c_str(), TypesChange::FloatToString(config.D()).c_str(), TypesChange::FloatToString(config.H()).c_str(), 
			TypesChange::FloatToString(config.Eps()).c_str(), TypesChange::IntToString(trajectories).c_str());
	filter.ExcludeEqualPoints();
	cout << "\n";
}

/**
* @brief ��������� �������� �������� � ������� ������� ��������� ��������
*/
void Controller::DrawPhasePortrait() {
	int dim = config.Dim(); // ������ ��������� ������ ������� �� ���������� ������, ������������� ������������� �������� ��������
	if ((dim == 1) || (dim == 2)) // � ������ ���������� ��� ��������� ������
		ProgramOperations::RunExeFile(path_to_app_risovalka);
	if (dim == 3) { // � ������ ���������� ������
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
* @brief �������� ���� ��������������� ������
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