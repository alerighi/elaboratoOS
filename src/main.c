#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"
#include "include/ipc.h"

/**
 * Programma principale che utilizza processi separati ed IPC systemV
 *
 * @file main.c
 * @author Alessandro Righi
 */

// id delle varie IPC
int shmidA = -1;
int shmidB = -1;
int shmidC = -1;
int shmidS = -1;
int semid = -1;
int msqid = -1;

int N;
int P;

struct worker *workers;

/**
 * Funzione main() del programma
 *
 * @param argc numero di argomenti da riga di comando
 * @param argv vettore degli argomenti da riga di comando
 */
int main(int argc, char *argv[])
{
	int i, j, n, k;
	void *ptr;
	struct message message;

	// setta i gestori dei segnali sigint e sigterm
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigterm_handler);

	/* controlla che gli argomenti siano in numero corretto */
	if (argc != 6)
	    usage();

	N = atoi(argv[4]);
	P = atoi(argv[5]);

    if (N <= 0)
		die("Parametro N non valido!");
	
	if (P <= 0)
		die("Parametro P non valido!");
	
	check_output_file(argv[3]);

	println("[P] Esecuzione programma");

	// creare la memoria condivisa
	shmidA = create_shm(N * N * sizeof(int));
	shmidB = create_shm(N * N * sizeof(int));
	shmidC = create_shm(N * N * sizeof(int));
	shmidS = create_shm(sizeof(int));

	// creare semafori
	semid = create_sem();

	// creare code di messaggi 
	msqid = create_msg();

	// leggo matrice A
	ptr = attach_shm(shmidA);
	load_matrix(argv[1], N, ptr);
	detach_shm(ptr);

	// leggo matrice B
	ptr = attach_shm(shmidB);
	load_matrix(argv[2], N, ptr);
	detach_shm(ptr);
	
	// inizializza array worker
	workers = malloc(P * sizeof(struct worker));

	println("[P] Avvio proecssi worker");

	// crea i processi 
	for (i = 0; i < P; i++)
		create_worker(i);

	println("[P] Eseguo moltiplicazione");
	
	// avvia i worker per la moltiplicazione 
	k = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (k < P)
				n = k++;
			else 
				n = wait_process();
			
			// manda comando al worker
			message.type = MSG_MUL;
			message.i = i;
			message.j = j;

			if (write(workers[n].pipe_fd, &message, sizeof(struct message)) != sizeof(struct message))
				die("Errore invio messaggio al figlio pid = %d", workers[n].pid);
		}
	}

	println("[P] Attendo figli");
	for (i = 0; i < P && i < N*N; i++)
		wait_process();

	
	println("[P] Eseguo somma");
	// avvia i worker per la somma

	k = 0;
	for (i = 0; i < N; i++)
	{
		if (k < P)
			n = k++;
		else 
			n = wait_process();
					
		// manda comando al worker
		message.type = MSG_SUM;
		message.i = i;

		if (write(workers[n].pipe_fd, &message, sizeof(struct message)) != sizeof(struct message))
			die("Errore invio messaggio al figlio pid = %d", workers[n].pid);
	}

	println("[P] Attendo figli");
	for (i = 0; i < P && i < N; i++)
		wait_process();

	// stampa il risultato

	ptr = attach_shm(shmidC);
	println("[P] Matrice risultante:");
	print_matrix(N, ptr);
	save_matrix(argv[3], N, ptr);
	detach_shm(ptr);

	ptr = attach_shm(shmidS);
	println("[P] Somma risultante: %d", *((int*) ptr));
	detach_shm(ptr);

	// pulisce tutte le risorse utilizzate
	cleanup();

	return 0;
}
