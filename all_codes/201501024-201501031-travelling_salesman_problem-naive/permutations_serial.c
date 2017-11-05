#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include <time.h>

int n;
int p;
double globalMinCost = 1e18;
double** adjacencyMatrix;

#define CLK CLOCK_MONOTONIC
struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if((end.tv_nsec-start.tv_nsec)<0){
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void swap(int* a, int* b) 
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

double calculateCostOfCylce(int* path)
{
	int i;
	double costOfCycle = 0.0;
	for(i = 0; i < n; i++)
		costOfCycle += adjacencyMatrix[path[i]][path[(i + 1) % n]];
	return costOfCycle;
}

void updateMinPath(int* currentPath, int* minPath)
{
	int i;
	for(i = 0; i < n; i++)
		minPath[i] = currentPath[i];
	return;
}

void printArray(int* arr)
{
	int i;
	for(i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

void heapOrderPermutation(int* array, int size, int* minPath)
{
	int i;
	if(size == 1) {
		/* Calculate Min Cost for the Hamiltonian cycle associated with the 
		 * permutation of vertices number stored in array */
		double totalCost = calculateCostOfCylce(array);
		
		if(totalCost < globalMinCost) {
			globalMinCost = totalCost;
			updateMinPath(array, minPath);
		}
		return;
	}
	for(i = 1; i < size + 1; i++) {
		heapOrderPermutation(array, size - 1, minPath);
		if(size % 2 == 1) 
			swap(array + 1, array + size);
		else
			swap(array + i, array + size);
	}
}

int main(int argc, char* argv[])
{
	struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
	/* Should start before anything else */
	clock_gettime(CLK, &start_e2e);

	/* Check if enough command-line arguments are taken in. */
	if(argc < 3){
		printf( "Usage: %s n p \n", argv[0] );
		return -1;
	}
	n = atoi(argv[1]);	/* size of input array */
	p = atoi(argv[2]);	/* number of processors*/

	int i, j;
	int globalMinPath[n];
	adjacencyMatrix = malloc(n * sizeof(double *));
	
	for(i = 0; i < n; i++) 
	{
		adjacencyMatrix[i] = malloc(n * sizeof(double));
		for(j = 0; j < n; j++)	
			scanf("%lf", &adjacencyMatrix[i][j]);
	}
	
	char *problem_name = "travelling_salesman_problem";
	char *approach_name = "naive";

  	char outputFileName[50];		
	sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);	

	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	double start_time = omp_get_wtime();
	
	int sourceVertex = 0;
	
	/*----------------------Core algorithm starts here----------------------------------------------*/
	
	int permutationArray[n];
	for(i = 0; i < n; i++)
		permutationArray[i] = i;
	swap(permutationArray, permutationArray + sourceVertex);
	
	heapOrderPermutation(permutationArray, n - 1, globalMinPath);
	
	/*----------------------Core algorithm finished--------------------------------------------------*/
	
	double end_time = omp_get_wtime();
	clock_gettime(CLK, &end_alg);	/* End the algo timer */

	/* Should end before anything else (printing comes later) */
	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, p, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
	// printf("Total Cities:%d \nNumber of processors used:%d \nMinimum cost Hamiltonian Cycle:%lf \n\
	Time taken by serial code:%lf\n", n, p, globalMinCost, end_time - start_time);
	// printf("Following cycle has the minimum cost\n");
	// printArray(globalMinPath);
	return 0;
}

