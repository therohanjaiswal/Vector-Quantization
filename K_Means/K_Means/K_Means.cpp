// K_Means.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;


void read_universe(double *universe, int universe_size, int vector_size) 
{
	ifstream file;
    file.open ("Universe.csv");   // open file with filename as argument
	if (!file.is_open()) {    
        std::cerr << "error: file open failed.\n";
        return;
	}

	int row = 0;
	int col = 0;
    string line = "", val = ""; 
    while (row < universe_size) {        // read each line
		getline (file, line);
        stringstream s(line);   // stringstream line 
		col = 0;
		while (col < vector_size) {
			getline (s, val, ',');
			*(universe + row*vector_size + col) = stof(val);
			++col;
		}  
		++row;
    }
	file.close();
	/*
	for(row = 0; row < universe_size; row++) 
	{
		for(col = 0; col < vector_size; col++) 
		{
			printf("%lf  ", *(universe + row*vector_size + col));
		}
		printf("\n");
	}
	printf("--------------------------------------------------------------------\n");
	*/
}

void create_codebook(double *universe, double *codebook, int k, int universe_size, int vector_size) 
{
	// TODO : generate unique random numbers
	int row = 0, col = 0;
	int num = 0;
	int lower = 0;
	int upper = universe_size  - 1;
    for (row = 0; row < k; row++) 
	{
        num = lower + (rand() % (upper - lower + 1));
		for(col = 0; col < vector_size; col++) 
		{
			 *(codebook + row*vector_size + col) = *(universe + num*vector_size + col);	
		}
    }
}

/* This function finds tokhura distance between test file and one vowel's reference file */
double find_tokhura_distance(double universe[6340][12], double codebook[8][12], int x, int y, int vector_size) {
    double weight[12] = { 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};	// Tokhhura weights 
    int i = 0;							// iterator
    double tokhura_distance = 0;		// store Tokhura distance considering 12 c(r) values of one frame at a time 
    double ct = 0;						// c(t) value
    double cr = 0;						// c(r) value
	for(i = 0; i < vector_size; i++) {
		ct = universe[x][i];
		cr = codebook[y][i];
		// printf("ct - %lf  cr - %lf\n", ct, cr);
        tokhura_distance += weight[i]*pow(ct - cr, 2);	// compute Tokhura Distance by standard formula
    }
    return tokhura_distance;	// return average tokhura distance
}

void k_means(double universe[6340][12], double codebook[8][12], int k, int universe_size, int vector_size) 
{
	int i = 0, j = 0;
	double **new_codebook = new double*[k];
	for(i = 0; i < k; i++) {
		new_codebook[i] = new double[vector_size];
	}

	int *cell = new int[universe_size]; 
	int *cell_count = new int[k];
	
	// classification
	double distortion = INT_MAX;
	double prev_distortion = 0.0;
	double delta = 0.00001;

	while(abs(distortion - prev_distortion) > delta) {
		prev_distortion = distortion;
		distortion = 0.0;

		// initialise
		for(i = 0; i < k; i++) {
			cell_count[i] = 0;
			for (j = 0; j<vector_size; j++) {
				new_codebook[i][j] = 0.0;
			}
		}

		double min_dist = INT_MAX;
		double tokhura_dist = 0;
		for(i = 0; i < universe_size; i++) {
			// printf("for i = %d -------------------------\n", i);
			min_dist = INT_MAX;
			for(j = 0; j < k; j++) {
				tokhura_dist = find_tokhura_distance(universe, codebook, i, j, vector_size);
				// printf("tokura dist %lf\n", tokhura_dist);
				if(tokhura_dist < min_dist) 
				{
					min_dist = tokhura_dist;
					cell[i] = j;
					// printf("cell %d = %d\n", i, cell[i]);
				}
			}
			distortion += min_dist;
			for(j = 0; j < vector_size; j++) {
				new_codebook[cell[i]][j] += universe[i][j];
			}
			++cell_count[cell[i]];
			// printf("%d \n", *(cell+i));
		}

		// centroid
		for(i = 0; i<k; i++) {
			for(j = 0; j<vector_size; j++) {
				codebook[i][j] = new_codebook[i][j]/cell_count[i];
			}
		}
		distortion /= universe_size;
		printf("%lf\n", distortion);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int universe_size = 6340;
	int vector_size = 12;
	int k = 8;
	double universe[6340][12];
	read_universe(universe[0], universe_size, vector_size);
	
	double codebook[8][12];
	srand(time(0));
	create_codebook(universe[0], codebook[0], k, universe_size, vector_size);
	
	k_means(universe, codebook, k, universe_size, vector_size);
	return 0;
}

