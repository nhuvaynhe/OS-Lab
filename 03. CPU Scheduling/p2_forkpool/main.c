#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "bktpool.h"

#undef STRESS_TEST
#define SHARED_MEMORY
#undef DEBUG

int func(void * arg) {
  int processID = * ((int * ) arg);

  printf("Task func -> Hello from %d\n", processID);
  wrk_ptr->arg = NULL;
  wrk_ptr->bktaskid = -1;
  wrk_ptr->func = NULL;

  fflush(stdout);
  return 0;
}

int main(int argc, char * argv[]) {
  int taskID[15];  
  int workerID[15];
  int id[15];
  int ret;

  /* Arguments that pass to the function */
  id[0] = 1;
  id[1] = 2;
  id[2] = 5;

  taskid_seed = 0;
  wrkid_cur = 0;
  bktask_sz = 0;

  /*NOTE:
  *   All initialization of variable that may use in child process
  *   must be init before fork().
  */

#ifdef SHARED_MEMORY
  create_shared_memory("Parent");
  attach_shared_memory("Parent");
#endif

  /* Create worker using fork() */
  ret = bktpool_init();
  if (ret != 0)
    return -1;

  /* Initialize tasks for each worker*/
  bktask_init( & taskID[0], & func, (void * ) & id[0]);
  bktask_init( & taskID[1], & func, (void * ) & id[1]);
  bktask_init( & taskID[2], & func, (void * ) & id[2]);

  /*
  *   Assign task to the first
  *   free worker.
  */

  workerID[1] = bkwrk_get_worker();
  ret = bktask_assign_worker(taskID[0], workerID[1]);  
  if (ret != 0)
    printf("assign_task_failed taskID=%d workerID=%d\n", taskID[0], workerID[1]);
  else
    printf("Parent    -> Assign tsk %d wrk %d \n", worker[workerID[1]].bktaskid, workerID[1]);

#ifdef SHARED_MEMORY
  fill_shared_memory_data("Parent", workerID[1]);
#endif 
  
  /* Wake up the worker */
#ifdef DEBUG
  printf("Parent    -> Start dispatching with pid: %d\n", wrkid_tid[workerID[1]]);
#endif
  bkwrk_dispatch_worker(workerID[1]);
  
  /* Wait for the worker done its work */
  while(wrk_ptr->func != NULL);
  wrkid_busy[workerID[1]] = 0;

  /*
  *   Create another 2 workers which
  *   do the tasks asynchronously
  */
  printf("Parent    -> Continue with tsk 1 and 2...\n\n");

  workerID[0] = bkwrk_get_worker();
  ret = bktask_assign_worker(taskID[1], workerID[0]);
  if (ret != 0)
    printf("assign_task_failed taskID=%d workerID=%d\n", taskID[1], workerID[0]);

  workerID[2] = bkwrk_get_worker();
  ret = bktask_assign_worker(taskID[2], workerID[2]);
  if (ret != 0)
    printf("assign_task_failed taskID=%d workerID=%d\n", taskID[2], workerID[2]);

#ifdef SHARED_MEMORY
  fill_shared_memory_data("Parent", workerID[0]);
  fill_shared_memory_data("Parent", workerID[2]);
  //detach_shared_memory("Parent", wrk_ptr);
#endif 

#ifdef DEBUG
  printf("Parent    -> Start dispatching with pid: %d\n", wrkid_tid[workerID[0]]);
  printf("Parent    -> Start dispatching with pid: %d\n", wrkid_tid[workerID[2]]);
#endif 

  bkwrk_dispatch_worker(workerID[0]);
  bkwrk_dispatch_worker(workerID[2]);
  
  /* Wait for the worker done its work */
  while(wrk_ptr->func != NULL);
  wrkid_busy[workerID[0]] = 0;
  wrkid_busy[workerID[2]] = 0;

#ifdef DEBUG
  printf("Parent    -> Done work, worker id %d is FREE\n", workerID[0]);
  printf("Parent    -> Done work, worker id %d is FREE\n", workerID[2]);
#endif

  fflush(stdout);
  sleep(1);
  
#ifdef SHARED_MEMORY
  detach_shared_memory("Parent", wrk_ptr);
#endif

#ifdef STRESS_TEST
  int i = 0;
  for (i = 0; i < 15; i++) {
    id[i] = i;
    bktask_init( & taskID[i], & func, (void * ) & id[i]);

    workerID[i] = bkwrk_get_worker();
    ret = bktask_assign_worker(taskID[i], workerID[i]);

    if (ret != 0)
      printf("assign_task_failed taskID=%d workerID=%d\n", taskID[i], workerID[i]);

    bkwrk_dispatch_worker(workerID[i]);
  }

  sleep(3);
#endif

  return 0;
}