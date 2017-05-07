#ifndef _IO_H
#define _IO_H

void usage(void);
void print(const char *message, ...);
void println(const char *message, ...);
void print_matrix(int n, int matrix[n][n]);
void msg_error(int status, const char *file, int line, const char *message, ...);

#define die(message, ...) msg_error(1, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define err(message, ...) msg_error(0, __FILE__, __LINE__, message, ##__VA_ARGS__)

#endif