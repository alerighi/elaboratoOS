#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"
#include "include/ipc.h"

// id delle varie IPC
int shmidA;
int shmidB;
int shmidC;
int shmidS;
int semid;
int msgid;

int N;
int P;

struct worker *workers;

/**
 * Stampa un messaggo di help relativo all'utilizzo del programma
 * e termina con stato d'uscita 1
 */
static void usage()
{
	const char *message = 
		"Usage: program <MatA> <MatB> <MatC> <N> <P>\n"
		"    - MatA    file prima matrice da moltiplicare\n"
		"    - MatB    file seconda matrice da moltiplicare\n"
		"    - MatC    file dove salvare la matrice risultante\n"
		"    - N       ordine della matrice\n"
		"    - P       numero di processi worker da creare";
	println(message);
	exit(0);
}

/**
 * Programma principale
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

	print("[P] Esecuzione programma\n");

	// creare la memoria condivisa
	shmidA = create_shm(argv[1], 10, N * N * sizeof(int));
	shmidB = create_shm(argv[2], 20, N * N * sizeof(int));
	shmidC = create_shm(argv[1], 30, N * N * sizeof(int));
	shmidS = create_shm(argv[1], 40, sizeof(long));

	// creare semafori
	semid = create_sem(argv[1], 50);

	// creare code di messaggi 
	msgid = create_msg(argv[1], 60);

	// leggo matrice A
	ptr = attach_shm(shmidA);
	load_matrix(argv[1], N, ptr);
	detach_shm(ptr);

	// leggo matrice B
	ptr = attach_shm(shmidB);
	load_matrix(argv[2], N, ptr);
	detach_shm(ptr);
	
	// inizializza array worker
	workers = malloc(P*sizeof(struct worker));

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
	println("[P] Somma risultante: %d", *((long*) ptr));
	detach_shm(ptr);

	// pulisce tutte le risorse utilizzate
	cleanup();

	return 0;
}
