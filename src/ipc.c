#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define PROJ_ID 42

/**
 * Crea un area di memoria condivisa il cui id viene 
 * generato a partire dal nome del file
 * 
 * @param filename nome del file da passare a ftok()
 * @param size dimensione dell'area di memoria condivisa
 * @return id memoria condivisa
 */ 
int create_shm(char *filename, size_t size)
{
	int shmid;
	key_t key;

	if ((key = ftok(filename, PROJ_ID)) == -1)
		die("Errore creazione key con ftok()");

	if ((shmid = shmget(key, size, IPC_CREAT|0600)) == -1)
		die("Errore creazione area memoria condivisa");

	return shmid;
}

void delete_shm(int shmid);
