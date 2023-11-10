#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define MAX_WORKER 10

int func(void * arg) {
  int id = * ((int * ) arg);

  printf("Task func - Hello from %d\n", id);
  fflush(stdout);

  return 0;
}

void create_worker(int wkrid)
{
    pid_t pid;

    if(wkrid > 0)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            printf("Worked created with id: %d\n", wkrid);
        }
        else if(pid > 0)
        {
            //parent
            create_worker(wkrid - 1);
        }
    }
}

int main()
{

    create_worker(MAX_WORKER);
    getc(stdin);

    return 0;
}
