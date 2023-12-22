#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;

#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;

	for (i = 0; i < MAX_PRIO; i++) {
		mlq_ready_queue[i].size = 0;
        mlq_ready_queue[i].curr_slot = MAX_PRIO - i;
	}
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
/* 
 *  Stateful design for routine calling
 *  based on the priority and our MLQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
int decrease_mlq_slot(struct pcb_t * proc) {
	int prio = proc->prio;
	mlq_ready_queue[prio].curr_slot--;

	if(mlq_ready_queue[prio].curr_slot == 0)
		return -1;

	return 0;
}

struct pcb_t * get_mlq_proc(void) {
	/*TODO: get a process from PRIORITY [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
#ifdef SYNCH
	pthread_mutex_lock(&queue_lock);
#endif
	struct pcb_t * proc = NULL;
	int processFound = 0;
	int noProcess = 1;

	while (!processFound) {
		for (int prio = 0; prio < MAX_PRIO; prio++) {
			if (mlq_ready_queue[prio].size > 0) {
				// If queue size > 0 -> Has process in queue
				noProcess = 0;
				if (mlq_ready_queue[prio].curr_slot > 0)  
				{
					proc = dequeue(&mlq_ready_queue[prio]);
					if (proc != NULL) {
						processFound = 1;
						break;
					}
				}
			}
		}

		if (noProcess) {
			pthread_mutex_unlock(&queue_lock);
			return NULL;
		}
		
		/* Handle the case when all process has no 
		 * CPU slot but still not complete its task.
		 * Just restart the CPU slot 
		 * */
		if (proc == NULL) {
			for (int prio = 0; prio < MAX_PRIO; prio++) {
				// Check if the queue is not empty
				if (mlq_ready_queue[prio].size > 0)
					mlq_ready_queue[prio].curr_slot = MAX_PRIO - prio;
			}
		}

	}

#ifdef SYNCH 
	pthread_mutex_unlock(&queue_lock);
#endif

	return proc;
}

void put_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(void) {
	return get_mlq_proc();
}

void put_proc(struct pcb_t * proc) {
	return put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;
	/*TODO: get a process from [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	return proc;
}

void put_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif


