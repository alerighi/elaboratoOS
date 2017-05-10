#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>

#include "include/worker.h"
#include "include/io.h"
#include "include/ipc.h"
#include "include/math.h"
#include "include/util.h"

/**
 * Notifica al padre il termine di un operazione
 *
 * @param n numero del worker
 */
static void notify_parent(int n)
{
    struct message message;

    message.type = MSG_OK;
    message.i = n;
    
    if (msgsnd(msgid, &message, sizeof(struct message) - sizeof(long), 0) == -1)
        die("Errore di invio messaggio notifica al padre!");

}

/**
 * Esegue il processo worker 
 *
 * @param pipe_fd file descriptor della pipe 
 * @param n numero di sequenza del worker da cui leggere i comandi del padre
 */
void run_worker(int pipe_fd, int n)
{
    pid_t pid;
    struct message message;

    int (*matrixA)[N];
    int (*matrixB)[N];
    int (*matrixC)[N];
    long *sum;

    int res;

    // resetto i gestori di segnale
    signal(SIGINT, NULL);
    signal(SIGTERM, NULL);

    pid = getpid();

    matrixA = attach_shm(shmidA);
    matrixB = attach_shm(shmidB);
    matrixC = attach_shm(shmidC);
    sum = attach_shm(shmidS);

    while (read(pipe_fd, &message, sizeof(message)) > 0)
    {
        switch (message.type)
        {
            case MSG_MUL:
                println("[F] Il figlio pid = %d esegue moltiplicazione (%d, %d)", getpid(), message.i, message.j);
                res = product_row_column(N, matrixA, matrixB, message.i, message.j);
                matrixC[message.i][message.j] = res;
                break;
            case MSG_SUM:
                println("[F] Il figlio pid = %d esegue somma %d", getpid(), message.i);
                res = sum_row(N, matrixC, message.i);
                sem_P(semid);
                *sum += res;
                sem_V(semid);
                break;
            default:
                die("Comando %d ricevuto da worker pid = %d non valido!", message.type, pid); 
        }
        notify_parent(n);
    }

    detach_shm(matrixA);
    detach_shm(matrixB);
    detach_shm(matrixC);
    detach_shm(sum);

    println("[F] Terminazione worker pid = %d", pid);
    exit(0);
}