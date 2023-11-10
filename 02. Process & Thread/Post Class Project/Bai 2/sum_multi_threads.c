#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arguments {
    int id;
    int lval;
    int rval;
    int sum;
} thread_params;

void *calculate_sum(void *arguments)
{
    thread_params* params = arguments;
    int lval = params->lval;
    int rval = params->rval;
    long sum = 0;


    for(int num=lval; num<=rval; num++) 
        sum += num; 

    //printf("Thread id: %d\n", params->id);
    //printf("---------> L val: %d | R val: %d | Sum: %ld\n", lval, rval, sum);
    pthread_exit((void*) sum); } 

int main(int argc, char *argv[]) 
{ 
    long MAX_THREADS = atol(argv[1]); 
    long MAX_NUMBER = atol(argv[2]); 
    // Check if the input value is within the range of the int data type.
    if (MAX_NUMBER < 0 || MAX_NUMBER > INT_MAX) {
        printf("INVALID INPUT!\n");
        exit(1);
    }
    if (MAX_THREADS <= 0 || MAX_THREADS > MAX_NUMBER/2) {
        printf("THREAD ONLY BETWEEN 1 and \"MAX NUMBER\" / 2!\n");
        exit(1);
    }
    int step =  MAX_NUMBER/MAX_THREADS; 
    void *returnValue;

    /* Create thread parameters */
    thread_params params[MAX_THREADS];
    //params[0].lval = 1;
    //params[0].rval = step; 
    //params[0].id = 0;
   
    /* Create an array of threads */
    pthread_t threadID[MAX_THREADS]; 
    //pthread_create(&threadID[0], NULL, &calculate_sum, &params[0]); 
    for(int i=0; i<MAX_THREADS; i++) { 
        params[i].lval = (step * i) + 1;
        params[i].rval = step * (i + 1); 
        params[i].id = i;

        pthread_create(&threadID[i], NULL, &calculate_sum, &params[i]);
    }

    /* Wait for all threads to finish */
    unsigned long long sum_of_all_threads = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threadID[i], &returnValue);
        sum_of_all_threads += (long)returnValue;
    }

    printf("\n------------------------------------------\n");
    printf("Max num: %ld | Max threads: %ld\n", MAX_NUMBER, MAX_THREADS);
    printf("SUM OF ALL THREADS: %lld\n", sum_of_all_threads); 

    return 0;
}
