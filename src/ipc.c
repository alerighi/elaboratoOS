#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>

#include "include/io.h"

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

/**
 * Elimina un area di memoria condivisa
 *
 * @param shmid id dell'area di memoria da eliminare
 */
void delete_shm(int shmid)
{
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
		err("Errore eliminazione area di memoria condivisa id %d", shmid);
}

/**
 * Collega un segmento di area di memoria condivisa
 *
 * @param shmid id dell'area di memoria condivisa
 */
void* attach_shm(int shmid)
{
	void *ptr;

	if ((ptr = shmat(shmid, NULL, 0)) == (void*) -1)
		die("Errore collegamento ad area di memoria condivisa %d", shmid);

	return ptr;
}

/**
 * Distacca un segmento di memoria condivisa
 *
 * @param ptr puntatore al segmento dell'area da distaccare
 */
void detach_shm(void *ptr)
{
	if (shmdt(ptr) == -1)
		die("Errore deatach memoria condivisa");
}
