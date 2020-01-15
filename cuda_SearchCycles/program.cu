#pragma region Формулировка задачи

	// Дан многомерный кубик. С помощью определенных входных параметров опредляются точки, из которых будут испускаться последовательности решений математического 
	// отображения. Требуется найти всевозможные цикловые решения без повторений, не превышающие определенной длины, в многомерном кубике.
	//
	// Вход: в файле data.config содержатся данные для работы программы
	//		dimension  (n, size) - размерность многомерного кубика;
	//		Далее идут dimension отрезков-проекций вида [a_i, b_i] многомерного куба на оси координат (полностью определяют расположение куба в пространстве) 
	//		(initset_proections);
	//		discr (d) - параметр дискретизации, расстояние между 2 соседними точками, из которых будут испускаться последовательности решений
	//					(параметр дискретизации определяет сетку (grid), которая накладывается на многомерный куб. Из каждой точки сетки испускаются решения);
	//		skip_iter (N, skip, amt_skip_iter) - количество пропускаемых значений для того, чтобы приблизиться к месту нахождения возможных циклов;
	//		max_iter (M, max_amt_iter) - ограничение сверху на количество итераций (максимальное количество подсчитанных значений в последовательности решений);
	//		max_cycle_len (K, lim_cycle_len) - максимально возможное значение длины цикла;
	//		T (check) - количество проверок цикла на устойчивость;
	//		eps - точность цикла (если какие-нибудь 2 значения в последовательности решений отличаются друг от друг не более чем на eps по модулю, 
	//			  то существует подозрение, что в этой последовательности решений есть цикл);
	//		tau - точность для проверки циклов на равенства (если 2 цикла одинаковой длины отличаются друг от друга по определенной выбранной норме не более чем на
	//			  tau по модулю, то эти циклы считаются равными);
	//		name_math_mapping - использумое математическое отображение;
	//		math_mapping_params - входные параметры математического изображения, НЕОБЯЗАТЕЛЬНЫЙ ПАРАМЕТРЫ (по умолчанию все входные параметры для математического
	//							  отображения - нулевые).
	//
	// Выход: в файле all_cycles.txt содержатся все возможные циклы в данном многомерном кубе;
	//		  в файле different_cycles.txt содержатся все различные циклы.

#pragma endregion 

#include "program.h" // вспомогательный заголовочный .h файл

int NumberMathMapping(char name_math_mapping[MAX_ARRAY_SIZE]) {
	string string_math_mapping(name_math_mapping, 0, strlen(name_math_mapping)-1);
	if (string_math_mapping == "ROTATION")
		return 1;
	if (string_math_mapping == "BERNULLI")
		return 2;
	if (string_math_mapping == "LOGICAL")
		return 3;
	if (string_math_mapping == "FILTERING")
		return 4;
	if (string_math_mapping == "RELAX")
		return 5;
	return 0;
}

int NumberGridKind(char name_grid[MAX_ARRAY_SIZE]) {
	string string_name_grid(name_grid, 0, strlen(name_grid)-1);
	if (string_name_grid == "UNIFORM")
		return 1;
	if (string_name_grid == "RANDOM")
		return 2;
	return 0;
}

bool AreParseDataFromConfig(string configfile, string readsection) { // распарсивание данных из файла конфигураций с помощью скрипта, написанного на Питоне
	FILE *script_file;
	char buffer[MAX_ARRAY_SIZE]; // выходящая информация из скрипта
	string callcommand; // строка вызова для функции _popen
	callcommand = "python " + pyscriptname + " " + configfile + " " + readsection; // формирование вызываемой в функции _popen строки
	if (!(script_file = _popen(callcommand.c_str(), "r"))) { // в том случае, если скрипт не выполнился
		printf("Error in launch Python script!");
		return false;
	}
	while (fgets(buffer, sizeof(buffer), script_file) != NULL) // выгрузка выходящей из скрипта информации о его работе
		printf("%s", buffer);
	_pclose(script_file);
	return true;
}

// функция считывания входных параметров для решения задачи
bool ReadData(string configfile, string readsection, int *dimension, float *a, float *b, int *num_points_source, float *discr, int *skip_iter, int *max_iter, int *max_cycle_len, int *T, float *eps, float *tau, int *num_math_mapping, float *math_mapping_params) { 
	if (!AreParseDataFromConfig(configfile, readsection))
		return false;
	FILE *input_file; // вспомогательный файл, с которого будет считываться информация
	input_file = fopen("input.txt", "rt");
	// считывание данных и проверка правильности считанных значений относительно ограничений
	fscanf(input_file, "%d", dimension);
	if (*dimension > MAX_DIMENSION) {
		printf("Too much dimension of the multidimensional cube! Please reduce it...\n");
		return false;
	}
	for (int i=0; i<MAX_DIMENSION; i++)
		a[i] = b[i] = 0.;
	for (int i = 0; i < *dimension; i++)
		fscanf(input_file, "%f %f", &a[i], &b[i]);
	char ch = NULL;
	fscanf(input_file, "%c", &ch);
	char name_points_source[MAX_ARRAY_SIZE];
	fgets(name_points_source, MAX_ARRAY_SIZE, input_file);
	*num_points_source = NumberGridKind(name_points_source);
	fscanf(input_file, "%f", discr);
	if (*discr > MAX_DISCR) {
		printf("Too high a value of the discretization! Please reduce it...\n");
		return false;
	}
	fscanf(input_file, "%d %d %d %d", skip_iter, max_iter , max_cycle_len, T);
	if (*max_cycle_len > LIM_MAX_CYCLE_LENGTH) {
		printf("Too high a value of cycle length! Please reduce it...\n");
		return false;
	}
	fscanf(input_file,"%f %f", eps, tau);
	memset(math_mapping_params, 0, MAX_ARRAY_SIZE);
	fscanf(input_file, "%c", &ch);
	char name_math_mapping[MAX_ARRAY_SIZE];
	fgets(name_math_mapping, MAX_ARRAY_SIZE, input_file);
	*num_math_mapping = NumberMathMapping(name_math_mapping);
	int idx = 0; 
	fscanf(input_file, "%d", &idx);
	for (int i=0; i<idx; i++)
		fscanf(input_file, "%f", &math_mapping_params[i]);
	printf("Program data is READ!\n"); // вывод сообщения об удачном считывании входных данных для работы программы
	fclose(input_file);
	remove("input.txt"); // удаление вспомогательного файла
	return true;
}

void OutputAllCycles(float a[MAX_ARRAY_SIZE], float discr, dim3 grid, int start_points, CycleData *cycle_list_host) { // вывод всех циклов в файл
	FILE *output_file; // файл для вывода информации
	output_file = fopen("all_cycles.txt", "wt");
	fprintf(output_file, "Start point \t\t\t\t Cyclelength \t\t\t Cycle\n");
	for (int i = 0; i < start_points; ++i) { // вывод координат точек, из которых испускались решения
		fprintf (output_file, "\n(%f,%f) \t\t cyclelength: %d \t\t", a[0]+(i%grid.x)*discr, a[1]+i/grid.y*discr, cycle_list_host[i].length);
		for (int j = 0; j < cycle_list_host[i].length; ++j) // вывод координат цикла
			fprintf (output_file, " (%f,%f)  ", cycle_list_host[i].points[j].x,  cycle_list_host[i].points[j].y) ;
	}
	fprintf (output_file, "\n\nAll start points: %d ", start_points) ;
	fclose(output_file);
	printf("Results of work program WRITE to output files!\n"); // вывод сообщения об удачном выводе данных в файл
}

void CudaOutputErrors() { // функция, выводящая информацию об ошибках при работе с инструментом CUDA
	printf("CUDA errors: ");
	printf(cudaGetErrorString(cudaGetLastError()));
	printf("\n\n");
}

void ResultsOfWorkProgram(bool state) { // вывод информации о завершающих этапах работы программы
	if (!state)
		printf("Program is interrupted!\n");
	else {
		printf("Work with GPU are OVER!\nProgram has successfully completed its work!\n");
		CudaOutputErrors();
	}
}

bool IsCudaDataCreated(void *cuda_pointer) { // функция, проверяющая была ли выделена память на GPU
	if (cuda_pointer == NULL) { // если память под указатель не была выделена на GPU
		printf("\nData for device is not created!\n");
		CudaOutputErrors();
		return 0;
	}
	return 1;
}

// функция очистки памяти от динамических объектов
void FreeUsedDynamicObjects(CycleData *cycle_list_host, CycleData *cycle_list_device, float *a_device, float *math_mapping_params, float2 *point_seq_device) { 
	free(cycle_list_host);
	cudaFree(cycle_list_device);
	cudaFree(a_device);
	cudaFree(math_mapping_params);
	cudaFree(point_seq_device);
}

float RandomBetweenTwoValues(float p, float q) {
	float rand_num = ((float)rand()) / (float)RAND_MAX;
	float diff = q - p;
	float r = rand_num * diff;
	return p + r;
}

bool MakeGrid(int num_points_source, dim3 *grid, float a[MAX_ARRAY_SIZE], float b[MAX_ARRAY_SIZE], float discr) { // формирование сетки для GPU
	if (num_points_source == 0) { // не был определён способ формирования сетки с начальными точками
		printf("\nGrid was not formed!\n");
		CudaOutputErrors();
		return false;
	}
	if (num_points_source == 1) {
		grid->x = ((b[0] - a[0]) / discr) + 1;
		grid->y = ((b[1] - a[1]) / discr) + 1;
	}
	if (num_points_source == 2) {
		grid->x = RandomBetweenTwoValues(a[0], b[0]);
		grid->y = RandomBetweenTwoValues(a[1], b[1]);
	}
	return true;
}

__device__ void SkipIterations(float2 *point, int num_iter, int skip_iter, int num_math_mapping, float math_mapping_params[MAX_ARRAY_SIZE]) { // пропуск первых N итераций
	for (int i = 0; i < skip_iter; ++i)
		*point = MathMapping(*point, num_iter+i, num_math_mapping, math_mapping_params);
}

__device__ bool IsEqualPoints(float2 first_point, float2 second_point, float eps) { // эквивалентные точки с точностью до eps
	if ((fabs(first_point.x - second_point.x) < eps) && (fabs(first_point.y - second_point.y) < eps))
		return 1;
	return 0;
}

float hostEuclideanDifference(float2 first_point, float2 second_point) { // разница между 2 циклами, посчитанная с помощью второй (евклидовой) нормы
	return sqrt(powf(fabs(first_point.x - second_point.x), 2) + powf((fabs(first_point.y - second_point.y)), 2));
}

int GetAmountAllCycles(int start_points, CycleData *cycle_list_host) {
	int cycles = 0; // количество циклов
	for (int i = 0; i < start_points; i++) {
		if (cycle_list_host[i].length > 0)
			cycles++;
	}
	return cycles;
}

// формирование списка различных циклов
void FormDifferentCyclesList(float *a, float discr, dim3 grid, int start_points, vector <DifferingCycleInfo> *differentCycles, CycleData *cycle_list_host, float tau) {
	bool is_add_cycle = false;
	for (int i = 0; i < start_points; ++i) {
		is_add_cycle = false;
		for (int j = 0; j < differentCycles->size(); ++j) {
			if (cycle_list_host[i].length == 0) { // циклы длины 0 не добавляются в вектор различных циклов
				is_add_cycle = true;
				break;
			}
			// добавление циклов в зависимости от значения евклидовой нормы разности точек
			int h = 0;
			for ( ; h < (*differentCycles)[j].elements.size(); ++h) 
				if (hostEuclideanDifference((*differentCycles)[j].elements[h], cycle_list_host[i].points[h]) >= tau)
					break;
			if (h == cycle_list_host[i].length) {
				(*differentCycles)[j].startpoints.push_back(make_float2(a[0]+(i%grid.x)*discr, a[1]+i/grid.y*discr));
				is_add_cycle = true;
				break;
			}
		}
		if (!is_add_cycle && cycle_list_host[i].length) { // добавление цикла в вектор
			DifferingCycleInfo ci;
			for (int j = 0; j < cycle_list_host[i].length; ++j)
				ci.elements.push_back(cycle_list_host[i].points[j]);
			ci.startpoints.push_back(make_float2(a[0]+(i%grid.x)*discr, a[1]+i/grid.y*discr));
			differentCycles->push_back(ci);
		}
	}
}

void OutputDifferentCycles(float *a, float d, dim3 grid, int start_points, CycleData *cycle_list_host, float tau) { // вывод различных циклов в файл
	vector <DifferingCycleInfo> differentCycles; // вектор, содержащий различные циклы
	int cycles = GetAmountAllCycles(start_points, cycle_list_host);
	FormDifferentCyclesList(a, d, grid, start_points, &differentCycles, cycle_list_host, tau);
	// вывод различных циклов
	FILE *output_file; // файл для вывода информации
	output_file = fopen("different_cycles.txt", "wt");
	fprintf(output_file, "All start points: \t%d\n", start_points);
	fprintf(output_file, "All cycles: \t\t%d\n", cycles);
	fprintf(output_file, "\nDifferent cycles \t%d\t:\n",  differentCycles.size());
	for (int i = 0; i < differentCycles.size(); ++i) {
		fprintf(output_file, "\n------------------------------------------------");
		fprintf(output_file, "\nStart points \t%d\t:\t", differentCycles[i].startpoints.size());
		for (int j = 0; j < differentCycles[i].startpoints.size(); ++j)
			fprintf(output_file, "(%f, %f)\t", differentCycles[i].startpoints[j].x, differentCycles[i].startpoints[j].y);
		fprintf(output_file, "\nCycle elements \t%d\t:\t", differentCycles[i].elements.size());
		for (int j = 0; j < differentCycles[i].elements.size(); ++j)
			fprintf(output_file, "(%f, %f)\t", (differentCycles[i].elements[j]).x, differentCycles[i].elements[j].y);	
	}
	fclose(output_file);
}

bool IsPointSequenceOver(float2 point) { // проверка на окончание последовательности-решения
	float2 stop_point = make_float2(FLT_MAX, FLT_MAX); // точка с очень большими координатами, символизирующими конец последовательности-решения
	if (point.x != stop_point.x || point.y != stop_point.y )
		return false;
	else
		return true;
}

void OutputAllPointSequences(float2 point_seq[MAX_ARRAY_SIZE], int start_points, int max_cycle_len) { // вывод всех последовательностей-решений в файл
	FILE *output_file;
	output_file = fopen("all_pointsequences.txt", "w");
	fprintf(output_file, "Start point \t\t\t\t Points of the sequence\n");
	for (int i = 0; i < start_points; ++i) {
		fprintf(output_file, "\n(%f, %f) \t\t", point_seq[i*max_cycle_len].x, point_seq[i*max_cycle_len].y);
		for (int j = 1; j < max_cycle_len; ++j) {
			if (!IsPointSequenceOver(point_seq[i*max_cycle_len+j]))
				fprintf(output_file, "(%f, %f) ", point_seq[i*max_cycle_len+j].x, point_seq[i*max_cycle_len+j].y);
			else
				break;
		}
	}
	fclose(output_file);
}

// избавление от значений вида -0.000000 при выводе в файл (подобного рода значения возникают из-за машинного нуля)
void DeleteNegativeZeros(int start_points, CycleData *cycle_list_host) {
	for (int i = 0; i < start_points; ++i)
		for (int j = 0; j < cycle_list_host[i].length; ++j) {
			if (cycle_list_host[i].points[j].x == 0)
				cycle_list_host[i].points[j].x = 0;
			if (cycle_list_host[i].points[j].y == 0)
				cycle_list_host[i].points[j].y = 0;
		}
}

__device__ float deviceEuclideanDifference(float2 first_point, float2 second_point) { // аналог функции hostEuclideanDifference, выполняется на GPU
	return __fsqrt_rn(__powf(fabs(first_point.x-second_point.x), 2) + __powf(fabs(first_point.y-second_point.y), 2));
}

// Проверка цикла на "устойчивость"
__device__ bool IsCyclePersisting(float2 first_point, int num_iter, int cycle_len, int T, float tau, int num_math_mapping, float math_mapping_params[MAX_ARRAY_SIZE]) { 
	float2 second_point = first_point;
	SkipIterations(&second_point, num_iter, cycle_len, num_math_mapping, math_mapping_params);
	num_iter+=cycle_len;
	for (int i = 0; i < T*cycle_len; ++i) {
		float diff = deviceEuclideanDifference(first_point, second_point);
		if (diff > tau)
			return 0;
		first_point = MathMapping(first_point, num_iter-cycle_len, num_math_mapping, math_mapping_params);
		second_point = MathMapping(second_point, num_iter, num_math_mapping, math_mapping_params);
		num_iter++;
	}
	return 1;
}

__device__ void AddCycleAtList(int idx, int cycle_len, float2 first_point, int num_iter, CycleData *cycle_list, int num_math_mapping, float math_mapping_params[MAX_ARRAY_SIZE]) { 
	// добавить в список по индексу idx k-элементов, начиная с first_point
	for (int i = 0; i < cycle_len; ++i) {
		cycle_list[idx].points[i] = first_point;
		first_point = MathMapping(first_point, num_iter, num_math_mapping, math_mapping_params);
		num_iter++;
	}
	cycle_list[idx].length = cycle_len;
}

__device__ void SearchMinPointInCycle(int idx, float2 *min, int *minPointIdx, CycleData *cycle_list) { // поиск минимальной точки в цикле и ее индекса
	for (int i = 0; i < cycle_list[idx].length ; ++i){
		if (cycle_list[idx].points[i].x < min->x) {
			*minPointIdx = i;
			*min = cycle_list[idx].points[i];
		}
		if (cycle_list[idx].points[i].x == min->x && cycle_list[idx].points[i].y < min->y) {
			*minPointIdx = i;
			*min = cycle_list[idx].points[i];
		}
	}
}

// циклический поворот (сдвиг) цикла относительно его минимального элемента
__device__ void TurnCycleRelativeHisMinElement(CycleData *cycle_list, int idx, int minPointIdx) { 
	float2 v[128];
	for (int i = 0; i < minPointIdx; ++i)
		v[i] = cycle_list[idx].points[i];
	for (int i = 0; i < cycle_list[idx].length-minPointIdx; ++i )
		cycle_list[idx].points[i] = cycle_list[idx].points[minPointIdx+i];
	for (int i = cycle_list[idx].length-minPointIdx; i < cycle_list[idx].length; ++i)
		cycle_list[idx].points[i] = v[i-cycle_list[idx].length+minPointIdx];
}

__global__ void KernelShiftCycle(CycleData * cycle_list) { // ядро, осуществляющее смещение (поворот) цикла
	int idx = blockIdx.x + blockIdx.y*gridDim.x;
	float2 min;
	if (cycle_list[idx].length != 0)
		min = cycle_list[idx].points[0];
	else
		return;
	int minPointIdx = 0; // индекс (номер) минимального элемента в цикле
	SearchMinPointInCycle(idx, &min, &minPointIdx, cycle_list);
	TurnCycleRelativeHisMinElement(cycle_list, idx, minPointIdx);
}

// ядро, осуществляющее поиск циклов
__global__ void KernelSearchCycles(float2 *point_seq, float a[MAX_ARRAY_SIZE], float discr, int skip_iter, int max_cycle_len, int T, CycleData *cycle_list, float eps, float tau, int num_math_mapping, float math_mapping_params[MAX_ARRAY_SIZE]) {
	float x = a[0] + blockIdx.x*discr; // x-координата текущей стартовой точки (из начального множества) 
	float y = a[1] + blockIdx.y*discr; // y-координата	
	int idx = blockIdx.x + blockIdx.y*gridDim.x;
	float2 first_point, second_point;
	first_point = make_float2(x, y);
	point_seq[idx*max_cycle_len+0]=first_point;
	SkipIterations(&first_point, 0, skip_iter, num_math_mapping, math_mapping_params); 
	int num_iter=skip_iter+1;
	second_point = MathMapping(first_point, skip_iter, num_math_mapping, math_mapping_params); // следующая точка из последовательности-решения
	point_seq[idx*max_cycle_len+1]=second_point;
	if( num_math_mapping==5 )
		max_cycle_len=math_mapping_params[4]+1;
	for (int i = 0; i < max_cycle_len-2; ++i) {
		if (IsEqualPoints(first_point, second_point, eps)) // сработало условие - значит first и second близки 
			if (IsCyclePersisting(first_point, skip_iter, i+1, T, tau, num_math_mapping, math_mapping_params)) { // если цикл сохраняется - добавляем элементы в список и выходим.
				if( num_math_mapping==5 )
					AddCycleAtList(idx, 1, first_point, skip_iter, cycle_list, num_math_mapping, math_mapping_params);
				else
					AddCycleAtList(idx, i+1, first_point, skip_iter, cycle_list, num_math_mapping, math_mapping_params);
				// поскольку в CUDA невозможно ни хранить зубчатые массивы, ни работать с vector, для окончания последовательности-решения, исходящего из данной 
				// начальной точки в её конец специально добавляется точка с очень большими координатами, символизирующее окончание работы с данной 
				// последовательностью-решением
				point_seq[idx*max_cycle_len+2+i]=make_float2(FLT_MAX,FLT_MAX);
				return;
			} // не сработал return, значит ищем цикл большей длины.
		if( num_math_mapping==5 )
			first_point=second_point;
		second_point = MathMapping(second_point, num_iter+i, num_math_mapping, math_mapping_params);
		point_seq[idx*max_cycle_len+2+i]=second_point;
	}
	cycle_list[idx].length = 0; // в случае если цикл так и не нашли
}

int main() {
	printf("Stages of program:\n\n"); // этапы выполнения программы
	int num_math_mapping = 0; // число, соответствующее математическому отображению
	int num_points_source = 0; // число, соответствующее разновидности генерации сетки
	int dimension = 0, skip_iter = 0, max_iter = 0, max_cycle_len = 0, T = 0;
	float eps = 0.0, tau = 0.0, discr = 0.0;
	float a[MAX_ARRAY_SIZE], b[MAX_ARRAY_SIZE]; // массивы для хранения концов отрезков-проекций многомерного куба на оси координат
	float math_mapping_params[MAX_ARRAY_SIZE]; // массив для хранения параметров математического отображения
	float2 point_seq[MAX_ARRAY_SIZE]; // массив для хранения последовательности-решения для каждой итерации
	string configfile = "data.config"; // конфигурационный файл, в котором хранятся входные параметры программы
	string readsection = "data_program"; // секция, определяющая какие данные следует считывать из конфига
	if (!ReadData(configfile, readsection, &dimension, a, b, &num_points_source, &discr, &skip_iter, &max_iter, &max_cycle_len, &T, &eps, &tau, &num_math_mapping, math_mapping_params)) { // в случае ошибки при считывании входных параметров программы
		ResultsOfWorkProgram(false);
		return 0;
	}
	dim3 grid; // сетка из начальных точек, поступающих на вход ядру
	if (!MakeGrid(num_points_source, &grid, a, b, discr)) {
		ResultsOfWorkProgram(false);
		return 0;
	}
	int start_points = grid.x*grid.y; // количество начальных точек, из которых будут испускаться решения
	CycleData *cycle_list_device = NULL, *cycle_list_host = NULL;
	float2 *point_seq_device = NULL;
	cycle_list_host = (CycleData*) malloc (start_points*sizeof(CycleData));
	cudaMalloc(&cycle_list_device, start_points*sizeof(CycleData));
	if (!IsCudaDataCreated((void*) cycle_list_device))
		return 0;
	float *a_device = NULL, *math_mapping_params_device = NULL;
	cudaMalloc(&a_device, dimension*sizeof(float));
	cudaMalloc(&math_mapping_params_device, MAX_ARRAY_SIZE*sizeof(float));
	cudaMalloc( &point_seq_device, MAX_ARRAY_SIZE*sizeof(float2));
	if (!IsCudaDataCreated((void*) a_device))
		return 0;
	if (!IsCudaDataCreated((void*) math_mapping_params_device))
		return 0;
	cudaEvent_t start_time, finish_time;
	cudaEventCreate(&start_time);
	cudaEventCreate(&finish_time);
	cudaEventRecord(start_time, 0);
	cudaMemcpy(a_device, a, dimension*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(math_mapping_params_device, math_mapping_params, MAX_ARRAY_SIZE*sizeof(float), cudaMemcpyHostToDevice);
	KernelSearchCycles<<<grid, 1>>>(point_seq_device, a_device, discr, skip_iter, max_cycle_len, T, cycle_list_device, eps, tau, num_math_mapping, math_mapping_params_device);
	cudaThreadSynchronize(); // синхронизация всех нитей (следующая функция программы не будет выполнена до тех пор, пока последняя нить не окончит свою работу)
	KernelShiftCycle<<<grid, 1>>>(cycle_list_device);
	cudaThreadSynchronize();
	cudaMemcpy(cycle_list_host, cycle_list_device, start_points*sizeof(CycleData), cudaMemcpyDeviceToHost);
	cudaMemcpy(point_seq, point_seq_device, MAX_ARRAY_SIZE*sizeof(float2), cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();
	cudaEventRecord(finish_time, 0);
	cudaEventSynchronize(finish_time);
	float work_time = 0.0;
	cudaEventElapsedTime(&work_time, start_time, finish_time);
	printf("Work time on GPU: %4.0f ms\n", work_time); // вывод времени работы программы на GPU 
	cudaEventDestroy(start_time);
	cudaEventDestroy(finish_time);
	OutputAllPointSequences(point_seq, start_points, max_cycle_len);
	DeleteNegativeZeros(start_points, cycle_list_host);
	OutputAllCycles(a, discr, grid, start_points, cycle_list_host);
	OutputDifferentCycles(a, discr, grid, start_points, cycle_list_host, tau);
	ResultsOfWorkProgram(true);
	FreeUsedDynamicObjects(cycle_list_host, cycle_list_device, a_device, math_mapping_params_device, point_seq_device);	
	return 0;
}