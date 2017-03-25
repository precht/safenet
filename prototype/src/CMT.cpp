#include "CMT.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <utility>

#define PI 3.14159265358979323846

//chaotic matrix C
std::vector<std::vector<std::pair<double, int>>> CMatrix;

//x0, y0 and control parameter alpha
double x, y, alpha;
//coefficients to change initial values and parameters
double H, G1, G2;
//chaotic matrix
//double** CMatrix;

const int WIDTH = 4;
const int HEIGHT = 4;



void print_values() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			std::cout << CMatrix[j][i].first << " ";
		}
		std::cout << std::endl << std::endl;
	}
}

void print_indexes() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			std::cout << CMatrix[j][i].second << " ";
		}
		std::cout << std::endl << std::endl;
	}
}


//computing x(i+1)
double compute_x(double x, double y) {
	double next_x = alpha * (sin(PI * y) + 3.0) * x * (1.0 - x);
	return next_x;
}

//computing y(i+1)
double compute_y(double x, double y) {
	double next_y = alpha * (sin(PI * x) + 3.0) * y * (1.0 - y);
	return next_y;
}

//summing x(i+1) with y(i+1)
double next_matrix_value(double x, double y) {
	double x_component = compute_x(x, y);
	double y_component = compute_y(x_component, y);
	return x_component + y_component;
}


//initializing x0, y0, alpha
void initialize() {
	//values from security key
	x = 0.9380;
	y = 0.7006;
	alpha = 0.4902;
	G1 = 1144200;
	H = 0.1318;

	x = fmod((x + G1 * H), 1);
	std::cout << "x: " << x << std::endl;


	y = fmod((y + G1 * H), 1);
	std::cout << "y: " << y << std::endl;

	alpha = 0.9 + fmod((alpha + G1 * H), 0.1);
	std::cout << "alpha: " << alpha << std::endl;

}



void CMT() {

	for (int i = 0; i < HEIGHT; i++) {
		CMatrix.push_back(std::vector<std::pair<double, int>>(WIDTH));
	}


	//filling chaotic matrix C with random values
	initialize();
	fillMatrixC(0, 0, x, y);

	std::cout << "------------------------" << std::endl;
	std::cout << "Original matrix" << std::endl;
	print_values();


	//sorting
	for (int i = 0; i < HEIGHT; i++) {
		std::sort(CMatrix[i].begin(), CMatrix[i].end());
	}

	std::cout << "------------------------" << std::endl;
	std::cout << "Sorted matrix" << std::endl;
	print_values();

	std::cout << "------------------------" << std::endl;
	std::cout << "Index matrix" << std::endl;
	print_indexes();

	pixel_shuffling();

}

void fillMatrixC(int i, int j, double x, double y) {

	if (i > 3) {
		return;
	}
	
	double x_next = compute_x(x, y);
	double y_next = compute_y(x_next, y);
	CMatrix[i][j].first = x_next + y_next;
	CMatrix[i][j].second = j;

	j++;
	if (j > 3) {
		j = 0;
		i++;
	}
	fillMatrixC(i, j, x_next, y_next);
}


void pixel_shuffling() {

	std::cout << "------------------------" << std::endl;
	std::cout << "Table before" << std::endl;
	int tab[WIDTH][HEIGHT] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16} };

	int values_to_remember[WIDTH][HEIGHT];
	std::copy(&tab[0][0], &tab[0][0] + WIDTH*HEIGHT, &values_to_remember[0][0]);

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			std::cout << tab[i][j] << " ";
		}
		std::cout << std::endl << std::endl;
	}


	int current_index = 0;
	int next_index = 0;
	int shift = 0;
	int value_to_remember = 0;


	for (int i = 0; i < WIDTH; i++) {

		for (int j = 0; j < HEIGHT; j++) {

			current_index = CMatrix[j][i].second;
			shift = (j + 1 + i) % WIDTH;
			next_index = CMatrix[shift][i].second;

			int value_to_remember = values_to_remember[next_index][shift];
			tab[next_index][shift] = values_to_remember[current_index][j];
		}
	}
	std::cout << "------------------------" << std::endl;
	std::cout << "Table after" << std::endl;

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			std::cout << tab[i][j] << " ";
		}
		std::cout << std::endl << std::endl;
	}

}