#ifndef _WORKER_H
#define _WORKER_H

/**
 * Contiene le definizione delle funzioni eseguite dal processo worker
 *
 * @file worker.h
 * @author Alessandro Righi
 */

void run_worker(int pipe_fd, int n);

#endif