#ifndef _FILE_H
#define _FILE_H

/**
 * Contiene le definizione delle funzioni per la lettura/scrittura di file
 *
 * @file file.h
 * @author Alessandro Righi
 */

void load_matrix(const char *filename, int n, int matrix[n][n]);
void save_matrix(const char *filename, int n, int matrix[n][n]);
void check_output_file(char *filename);

#endif