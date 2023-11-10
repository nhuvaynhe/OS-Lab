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
    /* define file name and read buffer size */
    const char* fname = "mmap";
    char readBuffer[1024];

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void* ptr;

    /* create the shared memory object */
    shm_fd = shm_open(fname, O_RDONLY, PERMS);
    
    /* receive the message */
    ptr = mmap(0, 4096, PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("\nRECEIVE PROCESS\n");
    printf("message received: %s\n", (char*)ptr);

    /* unmap the file from the process's address space */
    munmap(ptr, 4096);

    /* remove the shared memory object */
    shm_unlink(fname);

    return 0;
}

