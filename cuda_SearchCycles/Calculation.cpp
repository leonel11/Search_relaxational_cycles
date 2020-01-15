#include "calculation.h"

__device__ RelaxarionOscillations::RelaxarionOscillations(int dim = 1, float *solution = NULL, float par_d = 0.0, float par_alpha = 2.1, float par_beta = 1.0, float epsilon = 0.1, float h = 0.1) {
	dimension = dim;
	InitPointCoordinates(sln);
	for (int i = 1; i <= dimension; i++)
		sln[i] = solution[i];
	InitPointCoordinates(sln_plus_zero);
	InitPointCoordinates(sln_alpha_plus_zero);
	d = par_d;
	alpha = par_alpha;
	beta = par_beta;
	eps = epsilon;
	period = CalcPeriod();
	calc_acc = h;
}

__device__ bool RelaxarionOscillations::IsEqualPoints(float *prev) {
	for (int i = 1; i <= dimension; ++i)
		if ((fabs(prev[i] - sln[i]) >= eps))
			return 0;
	return 1;
}

__device__ void RelaxarionOscillations::SetSolution(float *solution = NULL) {
	for (int i = 1; i <= dimension; i++)
		sln[i] = solution[i];
}

__device__ float RelaxarionOscillations::CalcPeriod() {
	return alpha + 1.0 + (beta+1.0)/(alpha-beta-1.0);
}

__device__ int RelaxarionOscillations::GetDimension() {
	return dimension;
}

__device__ void RelaxarionOscillations::InitPointCoordinates(float *solution) {
	for (int i = 0; i < MAX_DIMENSION; i++)
		solution[i] = 0.0;
}

__device__ void RelaxarionOscillations::MakeImpetusToPointCoordinates(float phase) {
	for (int j = 1; j <= dimension; j++) {
		if (phase-calc_acc <= 0.0 && phase >= 0.0) { // phase = +0
			sln_plus_zero[j] = (alpha-1)/(alpha-beta-1)*sln[j];
			sln[j] = sln_plus_zero[j];
		}
		if (phase-calc_acc <= 1.0 && phase >= 1.0) { // phase = 1+0
			sln[j] = sln[j] - alpha/(alpha-1)*sln_plus_zero[j];
		}
		if (phase-calc_acc <= alpha && phase >= alpha) { // phase = alpha+0
			sln_alpha_plus_zero[j] = (1+beta)*sln[j];
			sln[j] = sln_alpha_plus_zero[j];
		}
		if (phase-calc_acc <= alpha+1 && phase >= alpha+1) { // phase = alpha+1+0
			sln[j] = sln[j] - alpha/(beta+1)*sln_alpha_plus_zero[j];
		}
	}
}

__device__ void RelaxarionOscillations::RungeKuttaMethod() {
	float k1, k2, k3, k4; // вспомогательные слагаемые для метода Рунге-Кутты
	for (int j = 1; j <= dimension; j++) {
		k1 = calc_acc*d*(exp(sln[j+1])+exp(-sln[j])-exp(sln[j])-exp(-sln[j-1]));
		k2 = calc_acc*d*(exp(sln[j+1]+calc_acc*k1/2)+exp(-sln[j]+calc_acc*k1/2)-exp(sln[j]+calc_acc*k1/2)-exp(-sln[j-1]+calc_acc*k1/2));
		k3 = calc_acc*d*(exp(sln[j+1]+calc_acc*k2/2)+exp(-sln[j]+calc_acc*k2/2)-exp(sln[j]+calc_acc*k2/2)-exp(-sln[j-1]+calc_acc*k2/2));
		k4 = calc_acc*d*(exp(sln[j+1]+calc_acc*k3)+exp(-sln[j]+calc_acc*k3)-exp(sln[j]+calc_acc*k3)-exp(-sln[j-1]+calc_acc*k3));
		sln[j] = sln[j] + (1./6)*(k1+2*k2+2*k3+k4);
	}
}

__device__ bool RelaxarionOscillations::IsPassedSwitchPoint(float phase) {
	// точки переключения: 0, 1, alpha, alpha+1
	if ((phase-calc_acc <= 0.0 && phase >= 0.0) || (phase-calc_acc <= 1.0 && phase >= 1.0) || (phase-calc_acc <= alpha && phase >= alpha) || (phase-calc_acc <= alpha+1 && phase >= alpha+1)) 
		return true;	
	return false;
}

__device__ void RelaxarionOscillations::CalcSolutionCoordinates() {
	FILE *output_file = fopen("all_relaxostillations.txt", "wt"); // файл, в который будет выводиться информация
	fprintf(output_file, "Calculation results for each iteration:\n\n\t Number \t\t Phase \t\t\t\t Result\n\n");
	int num_iter = 0;
	float phase = -calc_acc/2;
	fprintf(output_file, "\t %4d \t\t\t %f \t\t\t ", num_iter, phase);
	OutputPointCoordinates(output_file, 1, dimension);
	num_iter++;
	phase += calc_acc;
	while ((period - phase) > 0.0) {
		if ((phase - period) >= 0.0) // ограничение, чтобы при расчёте не выйти за пределы рассматриваемого периода
			phase = period;
		if (IsPassedSwitchPoint(phase))
			MakeImpetusToPointCoordinates(phase);
		else 
			RungeKuttaMethod();
		fprintf(output_file, "\t %4d \t\t\t %f \t\t\t ", num_iter, phase);
		OutputPointCoordinates(output_file, 1, dimension);
		num_iter++;
		phase += calc_acc;
	}
	fclose(output_file);
}

__device__ void RelaxarionOscillations::OutputPointCoordinates(FILE *output_file, int first, int last) {
	fprintf(output_file, "(");
	for (int i = first; i <= last; i++) {
		fprintf(output_file, " %f", sln[i]);
		if (i != last)
			fprintf(output_file, ",");
	}
	fprintf(output_file, " )\n");
}

__device__ RelaxarionOscillations::~RelaxarionOscillations() {}