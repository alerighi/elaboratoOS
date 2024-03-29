#ifndef _IO_H
#define _IO_H

/**
 * Contiene le definizioni delle funzioni per l'input/output
 *
 * @file io.h
 * @author Alessandro Righi
 */

void print(const char *message, ...);
void println(const char *message, ...);
void print_matrix(int n, int matrix[n][n]);
void msg_error(int status, const char *file, int line, const char *func, const char *message, ...);
void usage(void);

/**
 * Stampa su stderr un messaggio di errore e termina con stato 1
 */
#define die(message, ...) msg_error(1, __FILE__, __LINE__, __func__, message, ##__VA_ARGS__)

/**
 * Stama su stderr un messaggio di errore e non termina
 */
#define err(message, ...) msg_error(0, __FILE__, __LINE__, __func__, message, ##__VA_ARGS__)

#endif

