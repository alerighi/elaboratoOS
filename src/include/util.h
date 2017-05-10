#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>

// variabili risorse condivise
extern int shmidA;
extern int shmidB;
extern int shmidC;
extern int shmidS;
extern int semid;
extern int msgid;

// parametri del programma
extern int N;
extern int P;

/** 
 * struttura di un processo worker
 */
struct worker {
    pid_t pid;
    int pipe_fd;
};

// struttura esterna dei worker
extern struct worker *workers;

// metodi del programma
void cleanup(void);
void create_worker(int n);
void handle_sigusr1(int signal);
int wait_process(void);
void sigint_handler(int signal);
void sigterm_handler(int signal);

#endif