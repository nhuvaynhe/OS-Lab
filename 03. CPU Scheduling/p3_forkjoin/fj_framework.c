#include "forkjoin.h"

long long calculateSum(int min, int max)
{
    long long sum = 0;
    for (int i = min; i <= max; i++)
        sum += i;

#ifdef DEBUG
    printf("CalculateSum -> Return val %lld with min %d and max %d\n", sum, min, max);
#endif

    return sum;
}

long long RecursiveTask(void * arg)
{
    int* array_ptr = (int*) arg;
	int min = *array_ptr;
	int max = *(array_ptr +1);

    if (max - min <= THRESHOLD) {
        return calculateSum(min ,max);
    } else {
#ifdef DEBUG
        printf("RecursiveTask     ->  min %d max %d\n", min, max);
#endif
        long long sum = compute(min, max, &RecursiveTask);
        
        return sum;
    }
}

long long compute(int min, int max, void (* func))
{   
    int ret;

    int tskID[2];
    int wrkID[2];
    int mid = min + (max -min)/2;

	unsigned int leftSum[2];
	leftSum[0] = min;		
	leftSum[1] = mid;	
	tsk_init( & tskID[0], func, (void * ) & leftSum);

    unsigned int rightSum[2];
	rightSum[0] = mid +1;		
	rightSum[1] = max;	
	tsk_init( & tskID[1], func, (void * ) & rightSum);

    wrkID[0] = wrk_get_worker();
    ret = wrk_get_task(tskID[0], wrkID[0]);
    if (ret != 0) 
		printf("assign_task_failed tskID=%d wrkID=%d\n", tskID[0], wrkID[0]);

    wrkID[1] = wrk_get_worker();
    ret = wrk_get_task(tskID[1], wrkID[1]);
    if (ret != 0) 
		printf("assign_task_failed tskID=%d wrkID=%d\n", tskID[1], wrkID[1]);

    wrk_signal_task(wrkID[0]);
	while(worker[wrkID[0]].func != NULL);

    wrk_signal_task(wrkID[1]);
	while(worker[wrkID[1]].func != NULL);

	/* Wait for the worker done its work */
	wrkID_busy[wrkID[0]] = 0;
    wrkID_busy[wrkID[1]] = 0;
    
    /* Caculate sum from the return value */
#ifdef DEBUG
    printf("compute     -> SubtaskID %d done its job, return value %lld\n\n", tskID[0], worker[wrkID[0]].result);
    printf("compute     -> SubtaskID %d done its job, return value %lld\n\n", tskID[1], worker[wrkID[1]].result);
#endif

    long long sum = worker[wrkID[0]].result + worker[wrkID[1]].result;

    worker[wrkID[0]].result = 0;
    worker[wrkID[1]].result = 0;

    return sum;
}

