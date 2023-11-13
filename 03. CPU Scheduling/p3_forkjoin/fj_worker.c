#include "forkjoin.h"
#include <signal.h>
#include <stdio.h>

#define _GNU_SOURCE
#include <linux/sched.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>

void* wrk_create(void * arg) {
    sigset_t set;
    int sig;
    int s;
    int i = * ((int * ) arg); // Default arg is integer of workid

    Worker_* wrk = & worker[i];
    wrk->wrkID = i;

    /* Taking the mask for waking up */
    sigemptyset( & set);
    sigaddset( & set, SIGUSR1);
    sigaddset( & set, SIGQUIT);

    while (1) {
    /* wait for signal */
    s = sigwait( & set, & sig);
    if (s != 0)
      continue;

#ifdef DEBUG
    fprintf(stderr, "Child     -> worker wake %d up\n", i);
#endif

    /* Busy running */
    if (wrk -> func != NULL)
    {  
      worker[i].result = ((long long (*)(void *))(wrk->func))(wrk->arg);
      printf("Child     -> Return value %lld at taskID %d with wrkID %d\n", worker[i].result, worker[i].tskID, i);
    }
    else
      printf("Child     -> NO TASK\n");
      
    /* Advertise I DONE WORKING */
    worker[i].func = NULL;
    worker[i].arg = NULL;
    worker[i].tskID = -1;
  }
}

int wrk_init()
{
    unsigned int i;

    for(i = 0; i < MAX_WORKER; i++)
    {
        void ** child_stack = (void ** ) malloc(STACK_SIZE);
        unsigned int wrkid = i;
        pthread_t threadID;

        sigset_t set;
        int s;

        sigemptyset( & set);
        sigaddset( & set, SIGQUIT);
        sigaddset( & set, SIGUSR1);
        sigprocmask(SIG_BLOCK, & set, NULL);

        /* Stack grow down - start at top*/
        void * stack_top = child_stack + STACK_SIZE;

        wrkID_threadID[i] = clone( & wrk_create, stack_top,
        CLONE_VM | CLONE_FILES,
        (void * ) & i);
        
        fprintf(stderr, "WorkerPool got worker %u\n", wrkID_threadID[i]);
        usleep(100);
    }

    return 0;
}

int wrk_get_task(unsigned int tskID, unsigned int wrkid) {
  if (wrkid < 0 || wrkid > MAX_WORKER)
    return -1;

  Task_ * tsk = tsk_by_id(tskID);

  if (tsk == NULL)
    return -1;

  /* Advertise I AM WORKING */
  wrkID_busy[wrkid] = 1;

  worker[wrkid].func = tsk -> func;
  worker[wrkid].arg = tsk -> arg;
  worker[wrkid].tskID = tskID;

#ifdef DEBUG
  printf("Assign tsk %d wrk %d with arg %d\n", 
        worker[wrkid].tskID, wrkid, *(int *) worker[wrkid].arg);
#endif

  return 0;
}

int wrk_get_worker() {
  for(int i = 0; i < MAX_WORKER; i++) {
    if(wrkID_busy[i] == 0) {
      printf("Parent    -> Get worker %u\n", i);
      return i;
    }
  }

  return -1;
}

int wrk_signal_task(unsigned int wrkid) {

  unsigned int tid = wrkID_threadID[wrkid];

  /* Invalid task */
  if (worker[wrkid].func == NULL)
    return -1;

#ifdef DEBUG
  fprintf(stderr, "brkwrk dispatch wrkid %d - send signal %u \n", wrkid, tid);
#endif

  syscall(SYS_tkill, tid, SIG_DISPATCH);
}