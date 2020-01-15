#include "parser.h"

DataParser::DataParser() {
	dim = 1;
	wave_type = "";
	points_source = "";
	discr = 1;
	steps = 0;
	sample = 0;
	d = 0.0; 
	alpha = 0.0; 
	beta = 0.0;
	max_iter = 0;
	eps = 1;
	h = 1;
	reading_state = false; // ���������� �������������, ��� ������ �� ����������������� ����� �� ��������� ���������
						   // �� ������ ������ ��� �� ��������� ������
	PointOperations::ZeroizePoint(start_point);
}

/**
* @brief ������������� ������ �� ����������������� ����� � ������� �������, ����������� �� Python
* @return true, ���� �� ������ ������, false, ���� ������ �� ����������
*/
bool DataParser::IsParseData() {
	string launch_params = configfile + " " + readsection;
	bool res = ProgramOperations::RunPythonScript(pyscriptname, launch_params);
	return res;
}

/**
* @brief ���������� ������� ���������� ��������� ��� ������� �������� � �� ��������������� ��������� �� ������������
* @param aux_input_file - ��������������� ����, ���������������� � ���������� �������������, ������ ������� ��������� ������� ��������� ������
*/
void DataParser::ReadData(FILE *aux_input_file) {
	if (aux_input_file == NULL)
		return;
	// ���������� ������ � �������� ������������ ��������� �������� ������������ ����������� ������
	fscanf(aux_input_file, "%d", &dim);
	if (dim < 1 || dim > MAX_DIM) {
		cout << "Parameter 'dim' is outside the valid interval! Please reduce it..." << endl;
		return;
	}
	char ch = NULL;
	fscanf(aux_input_file, "%c", &ch);
	/*char wave_direct[MAX_ARRAY_SIZE];
	fgets(wave_direct, MAX_ARRAY_SIZE, aux_input_file);
	wave_direction = TypesChange::CharArrayToString(wave_direct);
	if (wave_direction != "UNI" && wave_direction != "BI") {
		cout << "Parameter 'wave_direction' don't have available value! Please change it..." << endl;
		return;
	}
	char bound_conds[MAX_ARRAY_SIZE];
	fgets(bound_conds, MAX_ARRAY_SIZE, aux_input_file);
	boundary_conditions = TypesChange::CharArrayToString(bound_conds);
	if (boundary_conditions != "NEUMAN" && boundary_conditions != "PERIODIC") {
		cout << "Parameter 'boundary_conditions' don't have available value! Please change it..." << endl;
		return;
	}*/
	char wave[MAX_ARRAY_SIZE];
	fgets(wave, MAX_ARRAY_SIZE, aux_input_file);
	wave_type = TypesChange::CharArrayToString(wave);
	if (wave_type != "UNIWAVE" && wave_type != "NEUMAN" && wave_type != "PERIODIC" && wave_type != "TWOCLUSTERS") {
		cout << "Parameter 'wave_type' don't have available value! Please change it..." << endl;
		return;
	}
	char comp_meth[MAX_ARRAY_SIZE];
	fgets(comp_meth, MAX_ARRAY_SIZE, aux_input_file);
	computing_method = TypesChange::CharArrayToString(comp_meth);
	if (computing_method != "ANALYTIC" && computing_method != "NUMERIC") {
		cout << "Parameter 'computing_method' don't have available value! Please change it..." << endl;
		return;
	}
	char points_src[MAX_ARRAY_SIZE];
	fgets(points_src, MAX_ARRAY_SIZE, aux_input_file);
	points_source = TypesChange::CharArrayToString(points_src);
	if (points_source != "GENERATE_GRID" && points_source != "GENERATE_RANDOM" && points_source != "FROM_FILE") {
		cout << "Parameter 'points_source' don't have available value! Please change it..." << endl;
		return;
	}
	fscanf(aux_input_file, "%f", &d);
	if (d < 0) {
		cout << "Parameter 'd' can not be negative! Please reduce it..." << endl;
		return;
	}
	fscanf(aux_input_file, "%f", &alpha);
	if (alpha <= 0.0) {
		cout << "Parameter 'alpha' can not be non-negative! Please reduce it..." << endl;
		return;
	}
	fscanf(aux_input_file, "%f", &beta);
	if (beta <= 0.0) {
		cout << "Parameter 'beta' can not be non-negative! Please reduce it..." << endl;
		return;
	}
	if (alpha-beta-1.0 <= 0.0) {
		cout << "Parameters 'alpha' & 'beta' are not satisfy alpha-beta-1 <= 0! Please reduce them..." << endl;
		return;
	}
	fscanf(aux_input_file, "%f", &h);
	if (h <= 0.0) {
		cout << "Parameter 'h' can not be non-negative! Please reduce it..." << endl;
		return;
	}
	fscanf(aux_input_file, "%d", &max_iter);
	if (max_iter < 0) {
		cout << "Parameter 'max_iter' can not be negative! Please reduce it..." << endl;
		return;
	}
	fscanf(aux_input_file, "%f", &eps);
	if (eps < 0.0) {
		cout << "Parameter 'eps' can not be negative! Please reduce it..." << endl;
		return;
	}
	// ���������� �������������� ���������� � ����������� �� ������� ��������� ��������� �����
	if (points_source != "FROM_FILE") {
		fscanf(aux_input_file, "%f", &discr);
		fscanf(aux_input_file, "%d", &steps);
		if (steps < 0) {
			cout << "Parameter 'steps' can not be negative! Please reduce it..." << endl;
			return;
		}
		if (points_source == "GENERATE_RANDOM")
			fscanf(aux_input_file, "%d", &sample);
		for (int i = 1; i <= dim; i++)
			fscanf(aux_input_file, "%f", &start_point[i]);
	}
	reading_state = true;
}

/**
* @brief ����������� ������� ���������� ��������� ��� ������� ��������
*/
void DataParser::PullData() {
	if (!IsParseData())	
		return;
	FILE *aux_input_file = fopen("in.txt", "rt"); // ��������������� ����, � �������� ����� ����������� ����������.
												  // ���� ���� ��������� � ���������� ������ ������������ �������
	ReadData(aux_input_file);
	fclose(aux_input_file);
	remove("in.txt"); // �������� ���������������� �����
	return;
}

DataParser::~DataParser() {
}