#ifndef _IPC_H
#define _IPC_H

int create_shm(char *filename, size_t size);
void delete_shm(int shmid);

#endif