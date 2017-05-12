#include "include/math.h"

/**
 * @file math.c
 * @brief Contiene le funzioni che eseguono le operazioni matematiche su matrice
 * @author Alessandro Righi
 */

/**
 * Esegue il prodotto della riga i della matrice A con 
 * la colonna j della matrice B e ritorna il risultato
 *
 * @param n ordine delle matrici
 * @param matrixA prima matrice
 * @param matrixB seconda matrice
 * @param matrixC matrice in cui salvare il risultato
 * @param i indice riga
 * @param j indice colonna
 */
void product_row_column(int n, int matrixA[n][n], int matrixB[n][n], int matrixC[n][n], int i, int j)
{
	int k;
	int result = 0;

	for (k = 0; k < n; k++)
		result += matrixA[i][k] * matrixB[k][j];

	matrixC[i][j] = result;
}

/**
 * Esegue la somma di tutti i valori in una riga
 *
 * @param n ordine della matrice
 * @param matrix matrice
 * @param k indice riga
 * @return il valore della somma
 */
int sum_row(int n, int matrix[n][n], int k)
{
	int i;
	int result = 0;

	for (i = 0; i < n; i++)
		result += matrix[k][i];

	return result;
}
