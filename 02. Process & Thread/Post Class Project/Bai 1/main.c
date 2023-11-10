#include "average_ratings.h"

int main(int argc, char* argv[])
{
    pid_t child_a, child_b;

    rateMatrix *mat_ptr;
    int shmid;
    
    char* file_name_1 = "movie-100k_1.txt";
    char* file_name_2 = "movie-100k_2.txt";
    
    /*
     * Create the shared memory segment
     */

    shmid = shmget(IPC_PRIVATE, sizeof(rateMatrix), 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    } else {
        printf("Shared memory ID: %d\n", shmid);
    }

    /*
     * Fork the children processes
     * and attach them to the shared segment
     */

    (child_a = fork()) && (child_b = fork());
    /*
     * Child A process code goes here 
     */
    if (child_a == 0) {
        printf("Child A process: %d\n", getpid());

        /* Attach child A to shared memory */
        mat_ptr = (rateMatrix*) shmat(shmid, NULL, 0);
        if (mat_ptr == (rateMatrix*) -1) {
            perror("shmat");
            exit(1);
        } 
        printf("Shared memory mm: %p\n", mat_ptr);

        /* Start reading file 1 and append to matrix */
        read_file(file_name_1, mat_ptr);

        /* Detach child A from shared memory */
        if (shmdt(mat_ptr) == -1){
            perror("shmdt returned -1\n");
        }else
            printf("Process A detached\n");

        exit(0);
    /*
     * Child B process code goes here 
     */
    } else if (child_b == 0) {
        printf("\nChild B process: %d\n", getpid());

        /* Attach child B to shared memory */
        mat_ptr = (rateMatrix*) shmat(shmid, NULL, 0);
        if (mat_ptr == (rateMatrix*) -1) {
            perror("shmat");
            exit(1);
        } 
        printf("Shared memory mm: %p\n", mat_ptr);

        /* Start reading file 2 and append to matrix */
        read_file(file_name_2, mat_ptr);

        /* Detach child B from shared memory */
        if (shmdt(mat_ptr) == -1){
            perror("shmdt returned -1\n");
        }else
            printf("Process B detached\n");

        exit(0);
    } 

    /* Wait for the child processes to terminate */
    while (wait(NULL) != -1) {}

    /*
     * Parent process code goes here
     */
    mat_ptr = (rateMatrix*) shmat(shmid, NULL, 0);
    printf("\nParent process: %d\n", getpid());
    printf("Shared memory mm: %p\n", mat_ptr);
    
    for(int col=0; col<COLUMNS; col++) 
        calculate_average_ratings(col, mat_ptr);

    /* Detach parent from shared memory */
    if (shmdt(mat_ptr) == -1){
        perror("shmdt returned -1\n");
    }else
        printf("Parent detached\n");

}
