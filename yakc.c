#include "yakk.h"
#include "clib.h"

#define VAR 1
#define NUM_TCBS 10
#define DEFAULT_FLAGS 0x100
#define IDLE_STACK_SIZE 256

unsigned YKCtxSwCount; //Type: unsigned int This is an unsigned int that must be incremented each time a context switch occurs, defined as the dispatching of a task other than the task that ran most recently.
unsigned YKIdleCount; // Type: unsigned int This is an unsigned int that must be incremented by the idle task in its while(1) loop. If desired, the user code can use this value to determine CPU utilization. See the section on YKIdleTask, above, to see how to prevent overflow of YKIdleCount.
unsigned YKTickNum; //Type:

int idleStk[IDLE_STACK_SIZE];

unsigned callDepth;

TCB* readyHead, *blockedHead, *delayedHead;

TCB* curTCB;

TCB tcbArray[NUM_TCBS];

unsigned int tcbCount = 0;

void YKInitialize(void){
	//Create Idle task and add it to the ready queue
	printString("YKInitialize\n");
	YKNewTask(&YKIdleTask, (void *)idleStk[IDLE_STACK_SIZE], 100);
	curTCB = &tcbArray[0];
	//new task adds to queue for us
}

void YKIdleTask(void){
	printString("YKIdleTask\n");
	/*while(true)
	*	increment YKIdleCount
	* 	do extra stuff
	*/
	while(1){
		if(2 > 1)
			if(2 > 1)
				if(2 > 1)
					YKIdleCount++;
	}
}

void addToQueue(TCB* tcb, TCB* listHead){
	//Go down the queue and check priority of each task

	TCB * pos = listHead;	

	printString("addToQueue\n");

	if(listHead == NULL) {
		listHead = tcb;
	}
	else {
		while(tcb->priority < pos->priority){
			pos = pos->next;
			if(pos == NULL){
				pos->next = tcb;
				tcb->previous = pos;
				tcb->next = NULL;
				return;
			}
		}
		pos->previous->next = tcb;
		tcb->previous = pos->previous;
		tcb->next = pos;
		pos->previous = tcb;	
	}
}
void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority){
	printString("YKNewTask\n");
	//Creates the TCB for the task and adds it to the task queue
	//Also initializes all values in the TCB
	tcbCount++;	

	tcbArray[tcbCount-1].priority = priority;
	tcbArray[tcbCount-1].state = READY;
	tcbArray[tcbCount-1].taskFunction = task;
	tcbArray[tcbCount-1].context[0] = 0;
	tcbArray[tcbCount-1].context[1] = (unsigned short)taskStack;
	tcbArray[tcbCount-1].context[2] = (unsigned short)task;
	tcbArray[tcbCount-1].context[3] = 0;
	tcbArray[tcbCount-1].context[4] = DEFAULT_FLAGS;

	addToQueue(&tcbArray[tcbCount-1], readyHead);
	
}

void YKRun(void){
	printString("YKRun\n");
	//Calls the scheduler and begins the operation of the program
	YKScheduler();
}

void YKDelayTask(unsigned count){
	printString("YKDelayTask\n");
	//if (count > 0)Sets the state of the TCB to delayed and then calls the scheduler. 
	if(count > 0) {
		curTCB->delay = count;
		curTCB->state = DELAYED;
		addToQueue(curTCB, delayedHead);
		YKScheduler();
	}
}

void YKEnterISR(void){
	printString("YKEnterISR\n");
	//Increment call depth
	callDepth++;
}


void YKExitISR(void){
	printString("YKExitISR\n");
	//Decrement call depth
	callDepth--;
	// if(call depth is 0)
	//	call scheduler
	if(callDepth == 0) {
		YKScheduler();
	}
}

void YKScheduler(void){
	printString("YKScheduler\n");
	//Peek the top ready task, if different from current task
	//call dispatcher
	if(curTCB != readyHead) {
		// curTCB = readyHead;
		YKDispatcher();
	}	

}

YKSEM* YKSemCreate(int initialValue){
	//Create YKSEM
}

void YKSemPend(YKSEM *semaphore){
	//if(semaphore > 0)
	//	decrement semaphore
	// 	return
	//else
	//	decrement semaphore
	//	block task
	// 	call scheduler
}

void YKSemPost(YKSEM *semaphore){
	//increment semaphore
	//unblock highest priority blocked task
	//call scheduler
}

YKQ *YKQCreate(void **start, unsigned size){
	//create Q
	//return &Q
}
void *YKQPend(YKQ *queue){
	//if(Q is empty)
	//	block calling task
	//	call scheduler
	//else
	//	pop message
	//	return &message
}
int YKQPost(YKQ *queue, void *msg){
	//if(Q is full)
	//	return 0
	// else
	//	push msg onto Q
	//	unblock highest priority task waiting for message
	//	if(task is unblocked)
	//		call scheduler
	//	else
	//		return 1
}
YKEVENT *YKEventCreate(unsigned initialValue){
	//Create YKEVENT
	//return &YKEVENT
}
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode){
	//if flag conditions met
	//	return
	//block task
	//call scheduler
 }
void YKEventSet(YKEVENT *event, unsigned eventMask){
	//check blocked tasks
	//unblock certain tasks
	//if tasks were unblocked
	//	call scheduler
	//else 
	//	return
}
void YKEventReset(YKEVENT *event, unsigned eventMask){
	//reset bits to 0
}
