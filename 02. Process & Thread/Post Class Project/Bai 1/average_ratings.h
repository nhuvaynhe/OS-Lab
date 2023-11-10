#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

/*
 * Read two files and compute rating of movies in the file 
 */

#define ROWS    943
#define COLUMNS 1682
#define SHM_KEY 0x123

/* Create matrix with 943 rows and 1682 columns to store rating */
typedef struct matrix {
    int rating[ROWS][COLUMNS];
} rateMatrix;


void read_file(char* file, rateMatrix* mat_ptr);
int calculate_average_ratings(int movieID, rateMatrix* mat_ptr);

