#ifndef _IPC_H
#define _IPC_H

#include <stdlib.h>

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

struct message {
    long type;
    int i;
    int j;
};

#define MSG_OK 1
#define MSG_MUL 2
#define MSG_SUM 3

#endif