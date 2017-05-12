#ifndef _WORKER_H
#define _WORKER_H

#include <stdlib.h> 

/**
 * @file worker.h
 * @brief Contiene le definizioni del per worker.c
 * @author Alessandro Righi
 */

void run_worker(int pipe_fd, int n);

#endif