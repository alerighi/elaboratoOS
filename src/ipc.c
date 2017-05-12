#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h> 
#include <sys/types.h>
#include <stdlib.h>

#include "include/io.h"
#include "include/ipc.h"

/**
 * @file ipc.c
 * @brief Contiene funzioni riguardanti le IPC
 * @author Alessandro Righi
 */

/**
 * Crea un area di memoria condivisa il cui id viene 
 * generato a partire dal nome del file
 * 
 * @param size dimensione dell'area di memoria condivisa
 * @return id memoria condivisa
 */ 
int create_shm(size_t size)
{
	int shmid;

	if ((shmid = shmget(IPC_PRIVATE, size, IPC_CREAT|0600)) == -1)
		die("Errore creazione area memoria condivisa");

	println("Creata area di memoria condivisa size = %dB, shmid = %d", size, shmid);

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
 * @return semid creato
 */
int create_sem()
{
	int semid;
	union semun {
        int val;
    } arg = {.val = 1};
	
	if ((semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1)
		die("Errore creazione semafori semget()");
	
	if (semctl(semid, 0, SETVAL, arg) == -1)
		die("Errore semctl()");

	println("Creato semaforo semid = %d", semid);

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
 * @return msgid della coda creata
 */
int create_msg()
{
	int msqid;

	// TODO: assicurarsi che la coda sia nuova!
	if ((msqid = msgget(IPC_PRIVATE, IPC_CREAT|0666)) == -1)
		die("Errore creazione coda di messaggi msgget()");

	println("Creata coda di messaggi msqid = %d", msqid);

	return msqid;
}

/**
 * Elimina una coda di messaggi 
 *
 * @param msgid id della cosa di messaggi da eliminare
 */
void delete_msg(int msqid)
{
	if (msgctl(msqid, IPC_RMID, NULL) == -1)
		die("Errore rimozione coda di messaggi msgid = %d", msqid);
	
	println("Eliminata coda di messaggi msqid = %d", msqid);
}

/**
 * Riceve un messaggio da una coda di messaggi
 * 
 * @param msgid id della coda di messaggi
 * @param type tipo del messaggio da ricevere
 * @param message memoria dove salvare il messaggio 
 */
void msg_rcv(int msgid, int type, struct message *message)
{
	if (msgrcv(msgid, message, sizeof(struct message) - sizeof(long), MSG_OK, 0) == -1)
		die("Errore msgrcv");
}

/**
 * Invia un messaggio ad una coda di messaggi
 *
 * @param msgid id della coda di messaggi
 * @param message messaggio da inviare
 */
void msg_snd(int msgid, struct message *message)
{
	if (msgsnd(msgid, message, sizeof(struct message) - sizeof(long), 0) == -1)
		die("Errore di invio messaggio notifica al padre!");
}
