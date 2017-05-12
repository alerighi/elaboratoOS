#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <signal.h> 
#include <sys/wait.h>

#include "include/ipc.h"
#include "include/util.h"
#include "include/worker.h"
#include "include/io.h"

/**
 * @file util.c
 * @brief Contiene funzioni di utilità
 * @author Alessandro Righi
 */

/**
 * Pulisce tutte le risorse inizializzate dal programma
 */
void cleanup(void)
{
	int i;

	// terminazione di tutti i processi figli
	if (workers)
	{	
		for (i = 0; i < P; i++)
		{
			if (kill(workers[i].pid, SIGTERM) == -1)
				err("Errore kill");

			if (wait(NULL) == -1)
				err("Errore wait(NULL)");
		}
	}

	// rimozione memoria condivisa
	if (shmidA != -1)
		delete_shm(shmidA);
	if (shmidB != -1)
		delete_shm(shmidB);
	if (shmidC != -1)
		delete_shm(shmidC);
	if (shmidS != -1)
		delete_shm(shmidS);

	// rimozione semafori
	if (semid != -1)
		delete_sem(semid);

	// eliminazione coda di messaggi
	if (msqid != -1)
		delete_msg(msqid);
}

/**
 * Crea un processo worker ed una pipe per inviargli comandi
 *
 * @param worker puntatore alla struttura descrittore del worker
 * @param n numero di sequenza del worker
 */
void create_worker(int n)
{
	int fdes[2];
	pid_t pid;

	if (pipe(fdes) == -1)
		die("Errore creazione pipe");
	
	pid = fork();

	if (pid == -1)
		die("Errore fork()");

	if (pid == 0) // processo figlio
	{
		if (close(fdes[1]) == -1) // chiudo la parte di scrittura della pipe
			die("Errore close(fdes[1]) pipe figlio");
		run_worker(fdes[0], n);
	}
	
	if (close(fdes[0]) == -1) // chiudo estremità lettura della pipe
		die("Errore close(fdes[0]) della pipe padre");
	
	workers[n].pipe_fd = fdes[1];
	workers[n].pid = pid;

	println("[P] Creato processo worker pid = %d, pipe fd = %d", pid, workers[n].pipe_fd);

}

/**
 * Attende il messaggio di terminazione di un worker, arrivato
 * il messaggio ne ritorna il suo id
 *
 * @return id del worker che ha finito
 */
int wait_process()
{
	struct message message;

	msg_rcv(msqid, MSG_OK, &message);
		
	return message.i;
}

/**
 * Handler del segnale SIGINT (CTRL-C)
 *
 * @param signal numero del segnale
 */
void sigint_handler(int signal)
{
	println("Ricevuto CTRL-C: terminazione...");
	cleanup();
	exit(0);
}

/**
 * Handler del segnale SIGTERM
 *
 * @param signal numero del segnale
 */
void sigterm_handler(int signal)
{
	println("Ricevuto SIGTERM: terminazione...");
	cleanup();
	exit(0);
}