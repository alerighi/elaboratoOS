#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>

/**
 * @file util.h
 * @brief Contiene le definizioni per worker.h
 * @author Alessandro Righi
 */

/** id memoria condivisa matrice A */
extern int shmidA; 

/** id memoria condivisa matrice B */
extern int shmidB; 

/** id memoria condivisa matrice C */
extern int shmidC; 

/** id memoria condivisa risultato */
extern int shmidS;

/** id semaforo */
extern int semid; 

/** id coda di mesdsaggi */
extern int msqid; 

/** ordine della matrice */
extern int N; 

/** numero di processi worker */
extern int P; 

/** 
 * struttura di un processo worker
 */
struct worker {
	/** pid del processo worker */
	pid_t pid;

	/** desctrittore della pipe del processo worker */
	int pipe_fd; 
};

/** array dei processi worker */
extern struct worker *workers; 

void cleanup(void);
void create_worker(int n);
int wait_process(void);
void sigint_handler(int signal);
void sigterm_handler(int signal);

#endif