#include <stdlib.h>

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"
#include "include/ipc.h"


// descrittori aree memoria condivisa
int shmidA;
int shmidB;
int shmidC;
int shmidS;

int main(int argc, char *argv[])
{
	int n;
	
	void *ptr;

	// setta i gestori dei segnali sigint e sigterm

	/* controlla che gli argomenti siano in numero corretto */
	if (argc != 6)
	    usage();

	n = atoi(argv[5]);

    if (n <= 0)
		die("Parametro N non valido!");

	print("Esecuzione programma\n");

	// creare la memoria condivisa
	shmidA = create_shm(argv[1], n * n * sizeof(int));
	shmidB = create_shm(argv[2], n * n * sizeof(int));
	shmidC = create_shm(argv[3], n * n * sizeof(int));
	shmidS = create_shm("Somma", sizeof(long));

	// leggo matrice A
	ptr = attach_shm(shmidA);
	load_matrix(argv[1], n, ptr);
	detach_shm(ptr);

	// leggo matrice B
	ptr = attach_shm(shmidB);
	load_matrix(argv[2], n, ptr);
	detach_shm(ptr);

	
	// avvia i worker per la moltiplicazione 

	// avvia i worker per la somma

	// stampa il risultato

	ptr = attach_shm(shmidC);
	println("Matrice risultante:");
	print_matrix(n, ptr);
	save_matrix(argv[4], n, ptr);
	detach_shm(ptr);

	ptr = attach_shm(shmidS);
	println("Somma risultante: %d", *((long*) ptr));
	detach_shm(ptr);

	// pulisce tutte le risorse utilizzate
	cleanup();

	return 0;
}