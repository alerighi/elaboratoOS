#include <stdlib.h>

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"

int main(int argc, char *argv[])
{
	int n = 4;
	int *matrixA;
	int *matrixB;

	/* controlla che gli argomenti siano in numero corretto */
	if (argc != 6)
	    usage();

	n = atoi(argv[5]);

    if (n <= 0)
		die("Parametro N non valido!");

	print("Esecuzione programma\n");

	matrixA = malloc(sizeof(int)*n*n);
	matrixB = malloc(sizeof(int)*n*n);

	load_matrix(argv[1], n, matrixA);
	//load_matrix(argv[2], matrixB, n);
	
	print_matrix(n, matrixA);

	save_matrix(argv[2], n, matrixA);

	return 0;
}