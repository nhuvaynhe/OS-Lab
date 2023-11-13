/*
 *  Fork join framework
 *  Date: 10/11/2023
 * 
 * 	Calculate sum from 0 to 1M
 */

#include "forkjoin.h"

/* Recursive task here */
int func(void * arg) {

	return RecursiveTask(arg);
}

int main(int argc, char * argv[]) {

	int tskID[15];  
	int wrkID[15];
	unsigned int arg[15];
	int ret;

	tskID_seed = 0;
	wrkID_curr = 0;
	tsk_sz = 0;

	ret = wrk_init();

	if (ret != 0)
		return -1;

	int minSum = 0;
	int maxSum = 1e6;
	arg[0] = minSum;		
	arg[1] = maxSum;		
	tsk_init( & tskID[0], & func, (void * ) & arg);

	wrkID[1] = wrk_get_worker();
	ret = wrk_get_task(tskID[0], wrkID[1]);
	if (ret != 0)
		printf("assign_task_failed tskID=%d wrkID=%d\n", tskID[0], wrkID[1]);

	wrk_signal_task(wrkID[1]);
	/* Wait for the worker done its work */
	while(worker[wrkID[1]].func != NULL);
	wrkID_busy[wrkID[1]] = 0;
	
	/* Caculate sum from the return value */
    printf("Main task done its job,  sum equals %lld\n", worker[wrkID[1]].result);
    long long sum = worker[wrkID[1]].result;
    worker[wrkID[1]].result = 0;

	fflush(stdout);
	sleep(1);

	return 0;
}