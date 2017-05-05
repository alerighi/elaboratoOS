#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/file.h"
#include "include/io.h"

/**
 * Legge un carattere da un file
 *
 * @param fd file descriptor da cui leggere
 * @return un carattere letto dal file, 0 altrimenti
 */
static char read_char(int fd)
{
	char c = 0;

	if (read(fd, &c, 1) != 1)
		die("Errore nella lettura dal file descriptor %d", fd);
	
	return c;
}

/**
 * Legge un intero da un file descriptor specificato. 
 * Ignora i caratteri non numerici
 *
 * @param fd file descriptor da cui leggere
 * @return numero letto
 */
static int read_integer(int fd)
{
	int n, i;
	char buff[1024];

	i = 0;
	while (buff[i] = read_char(fd))
	{
		if (buff[i] < '0' || buff[i] > '9')
			break;
		i++;
	}
	buff[i] = '\0';

	if (i == 0)
		die("Errore parsing del file descriptor %d", fd);

	n = atoi(buff);
	
	return n;
}

/**
 * Legge una matrice da un file csv
 *
 * @param filename nome del file da cui legger
 * @param matrix matrice in cui leggere
 * @param n ordine della matrice
 */
void load_matrix(const char *filename, int n, int matrix[n][n])
{
	int fd;
	int i, j;

	println("Caricamento file %s", filename);

	if ((fd = open(filename, O_RDONLY)) == -1)
		die("Impossibile aprire il file %s", filename);
	
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = read_integer(fd);

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

	if ((fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1)
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


