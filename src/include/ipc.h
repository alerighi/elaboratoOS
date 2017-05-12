#ifndef _IPC_H
#define _IPC_H

#include <stdlib.h>

/**
 * @file ipc.h
 * @brief Contiene le definizioni per il file ipc.c
 * @author Alessandro Righi
 */

/**
 * Struttura di un messaggio per IPC
 */
struct message {
	long type;
	int i;
	int j;
};

#define MSG_OK 1
#define MSG_MUL 2
#define MSG_SUM 3

// shm
int create_shm(char *filename, int id, size_t size);
void delete_shm(int shmid);
void* attach_shm(int shmid);
void detach_shm(void *ptr);

// sem
int create_sem(char *filename, int id);
void delete_sem(int semid);
void sem_P(int semid);
void sem_V(int semid);

int create_msg(char *filename, int id);
void delete_msg(int msgid);
void msg_snd(int msgid, struct message *msg);
void msg_rcv(int msgid, int type, struct message *msg);

#endif