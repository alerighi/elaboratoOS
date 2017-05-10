#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h> 
#include <sys/types.h>
#include <stdlib.h>

#include "include/io.h"

/**
 * Crea un area di memoria condivisa il cui id viene 
 * generato a partire dal nome del file
 * 
 * @param filename nome del file da passare a ftok()
 * @param id id da passare ad ftok()
 * @param size dimensione dell'area di memoria condivisa
 * @return id memoria condivisa
 */ 
int create_shm(char *filename, int id, size_t size)
{
	int shmid;
	key_t key;

	if ((key = ftok(filename, id)) == -1)
		die("Errore creazione key shm con ftok()");

	if ((shmid = shmget(key, size, IPC_CREAT|0600)) == -1)
		die("Errore creazione area memoria condivisa");

	println("Creata area di memoria condivisa size = %dB, key = %#010x, shmid = %d", size, key, shmid);

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
	
	println("Eliminata area di memoria condivisa shmid = %d", shmid);
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
		die("Errore mapping area di memoria condivisa %d", shmid);

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

/**
 * Crea il semaforo usato dall'applicazione
 *
 * @param filename file da passare a ftok()
 * @param id id da passare ad ftok()
 * @return semid creato
 */
int create_sem(char *filename, int id)
{
	key_t key;
	int semid;
	union semun arg;

	if ((key = ftok(filename, id)) == -1)
		die("Errore creazione key sem con ftok()");
	
	if ((semid = semget(key, 1, IPC_CREAT|0666)) == -1)
		die("Errore creazione semafori semget()");
	
	arg.val = 1;
	if (semctl(semid, 0, SETVAL, arg) == -1)
		die("Errore semctl()");

	println("Creato semaforo key = %#010x, semid = %d", key, semid);

	return semid;
}

/** 
 * Elimina un semaforo
 * 
 * @param semid id del semaforo da eliminare
 */
void delete_sem(int semid)
{
	if (semctl(semid, 0, IPC_RMID) == -1)
		die("Errore eliminazione semaforo semid = %d", semid);
	
	println("Eliminato semaforo semid = %d", semid);
}

/**
 * Esegue l'operazione P (wait) sul semaforo indicato
 * 
 * @param semid id del semaforo su cui fare la P
 */
void sem_P(int semid)
{
	struct sembuf sops = {0, -1, 0};

	if (semop(semid, &sops, 1) == -1)
		die("Errore semop P semaforo semid = %d", semid);
}

/**
 * Esegue l'operazione V (signal) sul semaforo indicato
 *
 * @param semid id del semaforo su cui fare la V
 */
void sem_V(int semid)
{
	struct sembuf sops = {0, 1, 0};

	if (semop(semid, &sops, 1) == -1)
		die("Errore semop V semaforo semid = %d", semid);
}

/**
 * Crea una coda di messaggi
 * 
 * @param filename filename da passare ad ftok()
 * @param id id da passare ad ftok();
 * @return msgid della coda creata
 */
int create_msg(char *filename, int id)
{
	int msgid;
	key_t key;

	if ((key = ftok(filename, id)) == -1)
		die("Errore creazione key msg con ftok()");

	// TODO: assicurarsi che la coda sia nuova!
	if ((msgid = msgget(key, IPC_CREAT|0666)) == -1)
		die("Errore creazione coda di messaggi msgget()");

	return msgid;
}

/**
 * Elimina una coda di messaggi 
 *
 * @param msgid id della cosa di messaggi da eliminare
 */
void delete_msg(int msgid)
{
	if (msgctl(msgid, IPC_RMID, NULL) == -1)
		die("Errore rimozione coda di messaggi msgid = %d", msgid);
}
