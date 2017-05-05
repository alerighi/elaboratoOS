#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#include "include/util.h"
#include "include/file.h"
#include "include/io.h"

/**
 * Stampa su standard error un messaggio di errore, 
 * quindi termina con stato di uscita 1
 *
 * @param message messaggio di errore da stampare
 */
void die(const char *message, ...)
{
    char buffer[4096];
    va_list args;

    va_start(args, message);
    
    vsnprintf(buffer, 4096, message, args);
    strcat(buffer, "\n");
    
    write(2, buffer, strlen(buffer));
    
    va_end(args);
    
    cleanup();
    exit(1);
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
    
    vsnprintf(buffer, 4096, message, args);
    strcat(buffer, "\n");

    if (write(1, buffer, strlen(buffer)) == -1)
        die("Errore stampa messaggio");
    
    va_end(args);
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

    if (write(1, buffer, strlen(buffer)) == -1)
        die("Errore stampa messaggio");
    
    va_end(args);
}

/**
 * Stampa un messaggo di help relativo all'utilizzo del programma
 */
void usage()
{
    const char *message = 
        "Usage: program <MatA> <MatB> <MatC> <N> <P>\n"
        "    - MatA    file prima matrice da moltiplicare\n"
        "    - MatB    file seconda matrice da moltiplicare\n"
        "    - MatC    file dove salvare la matrice risultante\n"
        "    - N       ordine della matrice\n"
        "    - P       numero di processi worker da creare\n";
    die(message);
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
