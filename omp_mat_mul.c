/*
   This file is modified from the source code on the text book, but the function has changed to realize the multipulation of matrices.
   This file has modified by Runqian FANG, Shandong University.
   E-mail:fangrq@mail.sdu.edu.cn
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

/* Serial functions */
void Get_args(int argc, char* argv[], int* thread_count_p);
void Usage(char* prog_name);
void Read_matrixA(char* prompt, int A[], int m, int n);
void Read_matrixB(char* prompt, int B[], int m, int n);
void Print_matrix(char* title, int C[], int m, int n);

/* Parallel function */
void Omp_mat_vect(int A[], int B[], int C[],
		int m, int n, int p, int thread_count);

/* Timer */
struct timeval begin,end;

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	int     thread_count;
	int     m, n, p;
	int* A;
	int* B;
	int* C;

	gettimeofday(&begin,NULL);



	Get_args(argc, argv, &thread_count);

	m=1000;
	n=1000;
	p=1000;

	A = malloc(m*n*sizeof(int));
	B = malloc(n*p*sizeof(int));
	C = malloc(m*p*sizeof(int));

	Read_matrixA("Enter the matrix", A, m, n);
	Read_matrixB("Enter the matrix", B, n, p);

	Omp_mat_vect(A, B, C, m, n, p, thread_count);

	Print_matrix("Result", C, m, p);

	free(A);
	free(B);
	free(C);

	gettimeofday(&end,NULL);

	double timeUse = end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)/1000000.0;
	printf("Elapsed time = %e\n", timeUse);

	return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:  Get_args
 * Purpose:   Get command line args
 * In args:   argc, argv
 * Out args:  thread_count_p
 */
void Get_args(int argc, char* argv[], int* thread_count_p)  {

	if (argc != 2) Usage(argv[0]);
	*thread_count_p = strtol(argv[1], NULL, 10);
	if (*thread_count_p <= 0) Usage(argv[0]);

}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
	fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
	exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrixA
 * Purpose:     Read in the matrix
 * In args:     prompt, m, n
 * Out arg:     A
 */
void Read_matrixA(char* prompt, int A[], int m, int n) {
	int             i, j;

	FILE* ain=fopen("a.txt","r");
	int tmp=0;

	for (i = 0; i < m; i++) 
		for (j = 0; j < n; j++)
			tmp=fscanf(ain,"%d", &A[i*n+j]);
	fclose(ain);
}  
/* Read_matrixA */

/*------------------------------------------------------------------
 * Function:    Read_matrixB
 * Purpose:     Read in the matrix
 * In args:     prompt, m, n
 * Out arg:     B
 */
void Read_matrixB(char* prompt, int B[], int m, int n) {
	int             i, j;
	int tmp;


	FILE* bin=fopen("b.txt","r");
	for (i = 0; i < m; i++) 
		for (j = 0; j < n; j++)
			tmp=fscanf(bin,"%d", &B[j*n+i]);
	fclose(bin);
}  /* Read_matrixB */

/*------------------------------------------------------------------
 * Function:  Omp_mat_vect
 * Purpose:   Multiply an mxn matrix by an nxp matrix
 * In args:   A, x, m, n, p, thread_count
 * Out arg:   y
 */
void Omp_mat_vect(int A[], int B[], int C[],
		int m, int n, int p, int thread_count) {
	int i, j, k;


#  pragma omp parallel for num_threads(thread_count) default(none) private(i, j, k)  shared(A, B, C, m, n, p)
	for (i = 0; i < m; i++) 
		for (j = 0; j< p; j++){
			int tmp= 0;
			for (k = 0; k < n; k++)
				tmp += A[i*n+k]*B[j*n+k];
			C[i*n+j] =tmp;
		}
	return;

}  /* Omp_mat_vect */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix( char* title, int C[], int m, int n) {
	int   i, j;

	//printf("%s\n", title);
	FILE* cout=fopen("c.out","w");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			fprintf(cout,"%d ", C[i*n + j]);
		fprintf(cout,"\n");
	}
	fclose(cout);
}  /* Print_matrix */
