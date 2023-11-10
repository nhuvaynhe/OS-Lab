#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define PERMS 0666

int main()
{
    /* define file name and size */
    const int SIZE = 4096;
    const char* fname = "mmap";
    const char* message = "Hello World!\n";

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    shm_fd = shm_open(fname, O_CREAT | O_RDWR, PERMS);
    ftruncate(shm_fd, SIZE);    // configure the size
    
    /* send the message */
    ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    strcpy(ptr, message);
    printf("\nSEND PROCESS\n");
    printf("message send: %s\n", message);


    /* unmap the file from the process's address space */
    munmap(ptr, 4096);

    return 0;
}

