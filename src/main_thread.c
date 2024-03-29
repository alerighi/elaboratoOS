#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
#include <pthread.h>

#include "include/file.h"
#include "include/io.h"
#include "include/math.h"

/**
 * Programma che utilizza i thread al posto di processi ed IPC
 *
 * @file main_thread.c
 * @author Alessandro Righi
 */

/** puntatore all'area di memoria della matrice A */
static int (*matrixA)[];

/** puntatore all'area di memoria della matrice B */
static int (*matrixB)[];

/** puntatore all'area di memoria della matrice C */
static int (*matrixC)[];

/** contiene la somma degli elementi della matrice risultante */
static int sum;

/** ordine delle matrici */
static int N;

/** mutex per proteggere la variabile sum */
static pthread_mutex_t mutex;

/** 
 * Struttura dati degli argomenti passati ai thread
 */
struct thread_args {
	/** indica la riga della matrice */
	int i;
	/** indica la colonna della matrice */
	int j;
};

/**
 * Thread worker per la moltiplicazione
 *
 * @param thread_args argomenti da passare al thread
 */
static void *worker_thread_mul(void *thread_arg) 
{
	struct thread_args *args;
	
	args = (struct thread_args *) thread_arg; 
	
	// esegue la moltiplicazione 
	product_row_column(N, matrixA, matrixB, matrixC, args->i, args->j);

	// libero memoria argomenti
	free(args);

	return NULL;
}


/**
 * Thread worker per la somma
 *
 * @param thread_args argomenti da passare al thread
 */
static void *worker_thread_sum(void *thread_arg) 
{
	struct thread_args *args;

	int tmp;
	
	args = (struct thread_args *) thread_arg; 
	
	tmp = sum_row(N, matrixC, args->i);

	if (pthread_mutex_lock(&mutex))
		die("Errore lock mutex");
			
	sum += tmp;

	if (pthread_mutex_unlock(&mutex))
		die("Errore unlock mutex");

	// libero memoria argomenti
	free(args);

	return NULL;
}

/**
 * Funzione main() del programma
 *
 * @param argc numero di argomenti da riga di comando
 * @param argv vettore degli argomenti da riga di comando
 */
int main(int argc, char *argv[])
{
	int i, j, k;
	struct thread_args *args;
	pthread_t *threads;

	/* controlla che gli argomenti siano in numero corretto */
	if (argc != 5)
		usage();

	N = atoi(argv[4]);

	if (N <= 0)
		die("Parametro N non valido!");

	check_output_file(argv[3]);
	
	println("Esecuzione programma");

	if (pthread_mutex_init(&mutex, NULL))
		die("Errore inizializzazione mutex");

	// alloca memoria dinamica
	matrixA = malloc(N * N * sizeof(int));
	matrixB = malloc(N * N * sizeof(int));
	matrixC = malloc(N * N * sizeof(int));
	threads = malloc(N * N * sizeof(pthread_t));

	load_matrix(argv[1], N, matrixA);
	load_matrix(argv[2], N, matrixB);
	
	println("Avvio thread worker moltiplicazione");
	
	// avvia i worker per la moltiplicazione 
	k = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{			
			args = malloc(sizeof(struct thread_args));
			args->i = i;
			args->j = j;
		   	
			if (pthread_create(&threads[k++], NULL, worker_thread_mul, (void *) args))
				die("Errore pthread_create");
		}
	}

	println("Attendo terminazione thread worker moltiplicazione");
	for (i = 0; i < N*N; i++)
		if (pthread_join(threads[i], NULL))
			die("Errore pthread_join");

	
	println("Avvio thread worker somma");
	// avvia i worker per la somma

	for (i = 0; i < N; i++)
	{
		args = malloc(sizeof(struct thread_args));
		
		args->i = i;

		if (pthread_create(&threads[i], NULL, worker_thread_sum, (void *) args))
			die("Errore pthread_create");

	}

	println("Attendo terminazione thread worker somma");
	for (i = 0; i < N; i++)
		if (pthread_join(threads[i], NULL))
			die("Errore pthread_join");

	// stampa il risultato

	println("Matrice risultante:");
	save_matrix(argv[3], N, matrixC);
	print_matrix(N, matrixC);

	println("Somma risultante: %d", sum);

	free(matrixA);
	free(matrixB);
	free(matrixC);
	free(threads);

	return 0;
}
