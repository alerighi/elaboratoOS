#ifndef _MATH_H
#define _MATH_H

/**
 * Contiene le definizioni delle funzioni che eseguono i calcoli su matrici
 *  
 * @file math.h
 * @author Alessandro Righi
 */

void product_row_column(int n, int matrixA[n][n], int matrixB[n][n], int matrixC[n][n], int i, int j);
int sum_row(int n, int matrix[n][n], int k);

#endif