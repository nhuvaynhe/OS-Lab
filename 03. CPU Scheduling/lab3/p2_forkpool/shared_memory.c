#include "bktpool.h"


void create_shared_memory(char* process)
{
  /* Apply shared memory for each process to update value */
  ShmID = shmget(IPC_PRIVATE, sizeof(wrk_ptr), IPC_CREAT | 0664);
  if (ShmID < 0) {
    printf("shmget\n");
    exit(1);
  }
  printf("%s    -> Created shared memory segment %d\n", process, ShmID);

}

void attach_shared_memory(char* process)
{
  wrk_ptr = (struct bkworker_t *)shmat(ShmID, NULL, 0);
  if(wrk_ptr == (struct bkworker_t *) -1) {
    printf("shmat\n");
    exit(1);
  }
  
  printf("%s    -> Attached the shared memory %d\n", process, ShmID);
}

void fill_shared_memory_data(char* process, int wrkid)
{ 
  wrk_ptr->bktaskid = worker[wrkid].bktaskid;
  wrk_ptr->func = worker[wrkid].func;
  wrk_ptr->arg = worker[wrkid].arg;
  
  printf("%s    -> Fill shared memory data with arg %d\n", process, *(int *)wrk_ptr->arg);
}

void detach_shared_memory(char* process, struct bkworker_t* shmPTR)
{
  if (shmdt(shmPTR) == -1){
    perror("shmdt\n");
  }else
    printf("%s    -> Detach from shared memory\n", process);
}
