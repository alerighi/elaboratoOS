#include <stdlib.h>

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"


// descrittori aree memoria condivisa
int shmidA;
int shmidB;
int shmidC;
int shmidS;


int main(int argc, char *argv[])
{
	int n = 4;
	int *matrixA;
	int *matrixB;

	// setta i gestori dei segnali sigint e sigterm

	/* controlla che gli argomenti siano in numero corretto */
	if (argc != 6)
	    usage();

	n = atoi(argv[5]);

    if (n <= 0)
		die("Parametro N non valido!");

	print("Esecuzione programma\n");

	// creare la memoria condivisa

	matrixA = malloc(sizeof(int)*n*n);
	matrixB = malloc(sizeof(int)*n*n);

	// legge i file delle matrici

	load_matrix(argv[1], n, matrixA);
	//load_matrix(argv[2], matrixB, n);
	
	// avvia i worker per la moltiplicazione 

	// avvia i worker per la somma

	// stampa il risultato

	print_matrix(n, matrixA);

	// salva la matrice risultato in output
	save_matrix(argv[2], n, matrixA);

	// pulisce tutte le risorse utilizzate
	cleanup();

	return 0;
}