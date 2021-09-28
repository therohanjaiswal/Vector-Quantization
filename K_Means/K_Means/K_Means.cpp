// K_Means.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

/* This function reads the Universe.csv and store it into universe 2D array */
double** read_universe(int universe_size, int p) 
{
	double **universe = new double*[universe_size];		// 2D array to store the codevectors from Universe.csv
	int i = 0, j = 0;						// iterators
	FILE * file = NULL;						// file pointer
	char * line = new char[1000];			// read each p dimentional codevector
	char * val = new char[100];				// read values from each p dimention codevector

	// create 2D array
	for(i = 0; i < universe_size; i++) 
		*(universe + i) = new double[p];

    file = fopen ("Universe.csv", "r");   // open file with filename as argument
	
	// if file is not present in the directory
	if (file == NULL) 
	{    
        printf("error: file open failed.\n");
        return NULL;
	}

	i = 0;
	// store all the codevector from universe into universe array
    while (i < universe_size) 
	{        
		fgets(line, 1000, file);		// read each codevector
		j = 0;					
		val = strtok(line,",");			// read value from each codevector
		while (j < p) 
		{
			*(*(universe + i) + j) = atof(val); //store the value in universe
			++j;
			val = strtok(NULL,","); 
		}  
		++i;
    }

	fclose(file);// open file with filename as argument
	return universe;	// return universe 
}

/* This function prints the codebook */
void print_codebook(double** codebook, int K, int p) 
{
	int i = 0, j = 0;	// iterators

	// print codebook
	for(i = 0; i < K; i++) 
	{
		for(j = 0; j < p; j++) 
			printf("%lf  ",*(*(codebook + i) + j));
		printf("\n");
	}
}

double** create_initial_codebook(double **universe, double **codebook, int K, int universe_size, int p) 
{
	codebook = new double*[K];		// 2D codebook array 
	int i = 0, j = 0;				// iterators
	int num = 0;					// store random codevector number from universe
	int lower = 0;					// lower bound of random numbers
	int upper = universe_size  - 1;	// upper bound of rand numbers

	// create 2D codebook array
	for(i = 0; i < K; i++) 
		*(codebook + i) = new double[p];

	srand(time(0));		// set seed

	// create the initial codebook by taking eight random codevectors from universe
    for (i = 0; i < K; i++) 
	{
        num = lower + (rand() % (upper - lower + 1));	// find random codevector number from universe
		for(j = 0; j < p; j++) 
			 *(*(codebook + i) + j) = *(*(universe + num) + j);	
    }

	return codebook; // return codebook
}

/* This function finds tokhura distance between codevectors from universe and codevector from codebook */
double find_tokhuras_distance(double **universe, double **codebook, int x, int y, int p) 
{
    double weight[12] = { 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};	// Tokhhura weights 
    double tokhuras_distance = 0;		// store Tokhuras distance 
    double ct = 0;						// c(t) value
    double cr = 0;						// c(r) value
	int i = 0;							// iterator

	// compute Tokhuras Distance by standard formula
	for(i = 0; i < p; i++) 
	{
		ct = *(*(universe + x) + i);
		cr = *(*(codebook + y) + i);
        tokhuras_distance += *(weight + i) * pow(ct - cr, 2);	
    }

    return tokhuras_distance;	// return Tokhuras distance
}

double** K_Means(double** universe, double** codebook, int K, int universe_size, int p) 
{
	double delta = 0.00001;					// threshold
	double **new_codebook = new double*[K];	// temporary 2D array
	int *cell = new int[universe_size];		// store the cell number(Region number) of all the codevectors in the universe
	int *cell_count = new int[K];			// store the number of codevectors present in each cell (region) 
	
	double distortion = INT_MAX;			// current distortion
	double prev_distortion = 0.0;			// previous distortion
	double min_tokhuras_dist = INT_MAX;		// minimum Tokhuras distance
	double tokhuras_dist = 0;				// Tokhuras distance
	int i = 0, j = 0, m = 0;				// iterators

	// create Initial codebook with 1 codevector(centroid of the universe)
	codebook = create_initial_codebook(universe, codebook, K, universe_size, p);
	
	printf("\nInitial Codebook\n");	
	print_codebook(codebook, K, p);	// print initial codebook
	printf("\n");	// newline

	// create 2D codebook array and 2D new_codebook array
	for(i = 0; i < K; i++) 
		*(new_codebook +i)  = new double[p];

	// iterate until abs(distortion - prev_distortion) > delta
	for(m = 0; abs(distortion - prev_distortion) > delta; m++){
		prev_distortion = distortion;
		distortion = 0.0;

		// initialise cell_count and new_codebook with 0
		for(i = 0; i < K; i++) {
			*(cell_count + i) = 0;
			for (j = 0; j<p; j++) {
				*(*(new_codebook + i) + j) = 0.0;
			}
		}

		// classification
		tokhuras_dist = 0;		// initialize tokhura_dist
		for(i = 0; i < universe_size; i++) 
		{
			min_tokhuras_dist = INT_MAX;	// initialize min_tokhuras_dist
			for(j = 0; j < K; j++) 
			{
				// find Tokhuras distance for ith codevector from universe and jth codevector from codebook 
				tokhuras_dist = find_tokhuras_distance(universe, codebook, i, j, p);	
				
				// find minimum Tokhuras distance among them
				if(tokhuras_dist < min_tokhuras_dist)	
				{
					min_tokhuras_dist = tokhuras_dist;	// update min_tokhuras_distance
					*(cell + i) = j;					// label ith codevector from universe with cell j(region j)
				}
			}

			distortion += min_tokhuras_dist;	// compute distortion by adding all the minimum Tokhuras distance

			// classify ith codevector from universe into cell j(region j)
			for(j = 0; j < p; j++) 
				*(*(new_codebook + cell[i]) + j) += *(*(universe + i) + j);
			
			++cell_count[cell[i]];	// increase the cell count
		}

		// finding centroid of each cell(region) and creating the codebook
		for(i = 0; i < K; i++) {
			for(j = 0; j < p; j++) {
				*(*(codebook + i) + j) = *(*(new_codebook + i) + j)/cell_count[i];
			}
		}

		distortion /= universe_size;	// computing average distortion
		printf("Distortion at iteration %d - %lf\n", m, distortion);	// print distortion 
	}
	
	printf("\nFinal Codebook\n");
	print_codebook(codebook, K, p);	// print final codebook

	return codebook;	// return final codebook
}

int _tmain(int argc, _TCHAR* argv[])
{
	int universe_size = 6340;	// number of codevectors in the universe
	int p = 12;					// dimension of a vector
	int K = 8;					// number of codevectors in the final codebook
	int i = 0, j = 0;			// iterators
	double **universe = NULL;	// double pointer to store codevectors of universe
	double **codebook = NULL;	// double pointer to store codevectors of codebook

	universe = read_universe(universe_size, p);	// read the universe.csv file and store it into universe array
	codebook = K_Means(universe, codebook, K, universe_size, p);	// run K Means algorithm

	return 0;
}

