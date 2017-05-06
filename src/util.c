#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#include "include/ipc.h"
#include "include/util.h"

/**
 * Pulisce tutte le risorse inizializzate dal programma
 */
void cleanup(void)
{
	// terminazione di tutti i processi figli

	// rimozione memoria condivisa
	delete_shm(shmidA);
	delete_shm(shmidB);
	delete_shm(shmidC);
	delete_shm(shmidS);
	
	// rimozione semafori

	// eliminazione fifo 

}