/*
 *  Fork join framework
 *  Date: 10/11/2023
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_WORKER 10
#define WRK_THREAD 1
#define STACK_SIZE 4096
#define THRESHOLD 125e3
#define SIG_DISPATCH SIGUSR1

typedef enum {
    NOT_STARTED = 0,
    IN_PROGRESS = 1,
    COMPLETED = 2,
} status_t;

typedef struct fj_tsk {
    void* arg;
    void(* func)(void* arg);
    unsigned int tskID;
    struct fj_tsk* nextTsk;  
}Task_;

typedef struct fj_worker{
    void* arg;
    void(* func)(void* arg);
    int wrkID;
    unsigned int tskID;
    long long result;
} Worker_;

Task_* taskList;
Worker_ worker[MAX_WORKER];

/* Worker module */ 
int wrkID_threadID[MAX_WORKER];
int wrkID_busy[MAX_WORKER];
int wrkID_curr;

void* wrk_create(void * arg);
int wrk_init();
int wrk_get_task(unsigned int tskID, unsigned int wrkid);
int wrk_get_worker();
int wrk_signal_task(unsigned int wrkid);

/* tsk module */
int tskID_seed;
int tsk_sz;

int tsk_init(unsigned int* tskID, void* func, void* arg);
Task_* tsk_by_id(unsigned int tskID);

/* ForkJoin framework */
int SubtskID_seed;
pthread_mutex_t mutex;

long long calculateSum(int min, int max);
long long RecursiveTask(void * arg);
long long compute(int min, int max, void (* func));
