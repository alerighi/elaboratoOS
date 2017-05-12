#ifndef _FILE_H
#define _FILE_H

/**
 * @file file.h
 * @brief Contiene ile definizioni per il file file.c
 * @author Alessandro Righi
 */


void load_matrix(const char *filename, int n, int matrix[n][n]);
void save_matrix(const char *filename, int n, int matrix[n][n]);
void check_output_file(char *filename);

#endif