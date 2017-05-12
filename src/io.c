#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifndef THREAD
	#include "include/util.h"
#endif

#include "include/file.h"
#include "include/io.h"

/**
 * @file io.c
 * @brief Contiene funzioni di input/output
 * @author Alessandro Righi
 */


/**
 * Stampa su standard error un messaggio di errore. Da usare con le apposite 
 * macro die() e err()
 *
 * @param status se > 0, termina con codice di uscita stato, se = 0, non termina
 * @param file file in cui si verifica l'errore
 * @param line linea in cui si verifica l'errore
 * @param message messaggio di errore da stampare
 */
void msg_error(int status, const char *file, int line, const char *func, const char *message, ...)
{
	char buffer[1024];
	char buffer2[2014];
	va_list args;

	va_start(args, message);
	
	vsnprintf(buffer, sizeof(buffer), message, args);
	if (errno)
		snprintf(buffer2, sizeof(buffer2), "[%s %d %s] %s: %s\n", file, line, func, buffer, strerror(errno));
	else 
		snprintf(buffer2, sizeof(buffer2), "[%s %d %s] %s\n", file, line, func, buffer);
	
	write(2, buffer2, strlen(buffer2));
	
	va_end(args);
	
	if (status)
	{
#ifndef THREAD
		cleanup();
#endif
		exit(status);
	}
}

/**
 * Stampa su standard output un messaggio di log con ritorno a capo
 *
 * @param message messaggio da stampare a video
 */
void println(const char *message, ...)
{
	char buffer[4096];
	va_list args;

	va_start(args, message);
	
	vsnprintf(buffer, sizeof(buffer), message, args);
	strcat(buffer, "\n");

	va_end(args);


	if (write(1, buffer, strlen(buffer)) == -1)
		die("Errore stampa messaggio");
}

/**
 * Stampa su standard output un messaggio di log
 *
 * @param message messaggio da stampare a video
 */
void print(const char *message, ...)
{
	char buffer[4096];
	va_list args;

	va_start(args, message);
	
	vsnprintf(buffer, 4096, message, args);

	va_end(args);

	if (write(1, buffer, strlen(buffer)) == -1)
		die("Errore stampa messaggio");
	
}

/**
 * Stampa una matrice su standard output
 *
 * @param n dimensione della matrice
 * @param matrix matrice da stampare
 */
void print_matrix(int n, int matrix[n][n])
{
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			print("%d\t", matrix[i][j]);
		
		print("\n");
	}
}

/**
 * Stampa un messaggo di help relativo all'utilizzo del programma
 * e termina con stato d'uscita 1
 */
void usage()
{
	const char *message = 
		"Usage: program <MatA> <MatB> <MatC> <N> <P>\n"
		"    - MatA    file prima matrice da moltiplicare\n"
		"    - MatB    file seconda matrice da moltiplicare\n"
		"    - MatC    file dove salvare la matrice risultante\n"
		"    - N       ordine della matrice\n"
#ifndef THREAD
		"    - P       numero di processi worker da creare\n"
#endif
		;
	print(message);
	exit(0);
}
