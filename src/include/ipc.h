#ifndef _IPC_H
#define _IPC_H

#include <stdlib.h> /* per size_t */

/**
 * Contiene le definizioni delle funzioni per la comunicazione fra processi (IPC)
 *  
 * @file ipc.h
 * @author Alessandro Righi
 */

/**
 * Struttura di un messaggio per IPC
 */
struct message {

	/** tipo del messaggio */
	long type; 

	/** indica la riga della matrice per messaggi di tipo MSG_MUL o MSG_SUM, o l'id del processo per messaggi di tipo MSG_OK */
	int i; 

	/** indica la colonna della matrice se messaggi di tipo MSG_MUL */
	int j; 
};

/** indica un messaggio di OK inviato da figlio al padre quando il figlio ha completato l'operazione corrente */
#define MSG_OK 1 

/** indica un messaggio di comando di moltiplicazione per il figlio */
#define MSG_MUL 2

/** indica un messaggio di comando di somma per il figlio */
#define MSG_SUM 3

int create_shm(size_t size);
void delete_shm(int shmid);
void* attach_shm(int shmid);
void detach_shm(void *ptr);

int create_sem(void);
void delete_sem(int semid);
void sem_P(int semid);
void sem_V(int semid);

int create_msg(void);
void delete_msg(int msgid);
void msg_snd(int msgid, struct message *msg);
void msg_rcv(int msgid, int type, struct message *msg);

#endif