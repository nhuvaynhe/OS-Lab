#include "forkjoin.h"

int tsk_init(unsigned int* taskID, void* func, void* arg)
{
	/* Create a list of task */
	Task_* new_task = malloc(sizeof(Task_));

	*taskID = ++tskID_seed;
	++tsk_sz;

	new_task->arg = arg;
	new_task->func =func;
	new_task->tskID = *taskID;

	new_task->nextTsk = taskList;
	taskList = new_task;
}

Task_* tsk_by_id(unsigned int tskID)
{
	/* Get task by ID */
	Task_* task = taskList;

	if(tskID < 0 || tskID > tsk_sz)
		return NULL;

	if(task == NULL)
		return NULL;

	while(task != NULL) {
		if (task->tskID == tskID)
		return task;

		task = task->nextTsk;
	}

	return NULL;
}