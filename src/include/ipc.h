#ifndef _IPC_H
#define _IPC_H

#include <stdlib.h>

int create_shm(char *filename, size_t size);
void delete_shm(int shmid);
void* attach_shm(int shmid);
void detach_shm(void *ptr);

#endif