#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/file.h"
#include "include/io.h"

/**
 * @file file.c
 * @brief Contiene funzioni per la lettura/scrittura su file
 * @author Alessandro Righi
 */

/**
 * Legge un carattere da un file. Ignora gli spazi.
 *
 * @param fd file descriptor da cui leggere
 * @return un carattere letto dal file, 0 altrimenti
 */
static char read_char(int fd)
{
	char c = 0;

	do
		if (read(fd, &c, 1) != 1)
			die("Errore nella lettura dal file descriptor %d", fd);
	while (c == ' ');

	return c;
}

/**
 * Legge una matrice da un file csv. 
 *
 * @param filename nome del file da cui legger
 * @param matrix matrice in cui leggere
 * @param n ordine della matrice
 */
void load_matrix(const char *filename, int n, int matrix[n][n])
{
	int fd, i, j, k;
	char buff[64];


	println("Caricamento file %s", filename);

	if ((fd = open(filename, O_RDONLY)) == -1)
		die("Impossibile aprire il file %s", filename);
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++) 
		{
			for (k = 0; (buff[k] = read_char(fd)); k++)
				if ((buff[k] < '0' || buff[k] > '9') && buff[k] != '-')
					break;

			if (buff[k] != ',' && buff[k] != '\n')
				die("Errore nel file %s riga %d: carattere '%c' non riconosciuto", filename, j, buff[k]);

			if (buff[k] == '\n' && j != n-1)
				die("Errore nel file %s: la matrice del file non è %dx%d", filename, n, n);

			buff[k] = '\0';
	
			matrix[i][j] = atoi(buff);
		}
	}

	if (close(fd) == -1)
		die("Errore chiusura file %s", filename);
}

/**
 * Scrive una matrice su un file csv
 *
 * @param filename file su cui scrivere la matrice
 * @param matrix matrice da scrivere
 * @return n ordine della matrice 
 */
void save_matrix(const char *filename, int n, int matrix[n][n])
{
	int fd; 
	int i, j;
	char buff[20];

	println("Salvataggio matrice su file %s", filename);

	if ((fd = open(filename, O_WRONLY|O_TRUNC)) == -1)
		die("Errore apertura file %s", filename);

	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < n; j++) 
		{
			snprintf(buff, sizeof(buff), "%d%c", matrix[i][j], j == n-1 ? '\n': ',');
			
			if (write(fd, buff, strlen(buff)) == -1)
				die("Errore di scrittura sul file %s", filename);
		}
	}   

	if (close(fd) == -1)
		die("Errore chiusura file %s", filename);
}

/** 
 * Funzione per controllare la validità del file di output:
 * - se il file non esiste, lo crea
 * - se il file esiste, controlla che sia scrivibile
 *
 * @param filename nome del file
 */
void check_output_file(char *filename)
{
	// se il file non esiste, provo a crearlo
	if (access(filename, F_OK) == -1)
		if (creat(filename, 0644) == -1) 
			die("Errore creazione file output %s", filename);
		else 
			println("Creato file di output %s", filename);
	else if (access(filename, W_OK) == -1) // se esiste, controllo se posso leggerlo
		die("Errore file output %s", filename);
}