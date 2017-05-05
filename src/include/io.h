#ifndef _IO_H
#define _IO_H

void usage(void);
void print(const char *message, ...);
void println(const char *message, ...);
void die(const char *message, ...);
void print_matrix(int n, int matrix[n][n]);

#endif