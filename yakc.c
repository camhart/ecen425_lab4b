#include "yakk.h"
#include "clib.h"

#define VAR 1
#define NUM_TCBS 10
#define IDLE_STACK_SIZE 256

unsigned YKCtxSwCount; 
unsigned YKIdleCount; 
unsigned YKTickNum; 

int idleStk[IDLE_STACK_SIZE];

int firstRun = 1;

unsigned callDepth;

TCB* readyHead = null; 
TCB* blockedHead = null;
TCB* delayedHead = null;
TCB* curTCB = null;

TCB tcbArray[NUM_TCBS];

unsigned int tcbCount = 0;

void YKInitialize(void){
	YKEnterMutex();
	//Create Idle task and add it to the ready queue
	//printString("YKInitialize\n");
	YKNewTask(&YKIdleTask, (void *)&idleStk[IDLE_STACK_SIZE], 100);
	//new task adds to queue for us
}

void YKIdleTask(void){
	int a = 0;
	// printString("YKIdleTask\n");
	while(1){
		// printString("I'm in the idle task\n");
		a++;
		a = a * a;
		YKIdleCount++;
	}
}

void removeFromQueue(TCB* tcb){
	if(tcb->previous != null)
		tcb->previous->next = tcb->next;
	if(tcb->next != null)
		tcb->next->previous = tcb->previous;
	tcb->next = null;
	tcb->previous = null;
	return tcb;
}

TCB * addToQueue(TCB* tcb, TCB* listHead){
	//Go down the queue and check priority of each task
	TCB * pos = listHead;//listHead;	

	if(listHead == null) {
		listHead = tcb;
	}
	else {
		if(listHead->priority > tcb->priority){
			listHead = tcb;
			(tcb->previous) = pos->previous;
			(tcb->next) = pos;
			(pos->previous) = tcb;
			return listHead;
		}

		while(tcb->priority < pos->priority){
			if(pos->next != null)			
				pos = pos->next;
			else{
				tcb->previous = pos;
				pos->next = tcb;
				return listHead;
			}
		}
		(pos->previous->next) = tcb;
		(tcb->previous) = pos->previous;
		(tcb->next) = pos;
		(pos->previous) = tcb;	
	}
	return listHead;
}

void printTasks(){
	TCB * pos = readyHead;
	//printString("Here are all the tasks: \n");
	while(pos != null){
		printInt(pos->priority);
		//printString("\n");

		// printString("\n");
		pos = pos->next;
	}
}

void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority){
	//printString("YKNewTask\n");
	//Creates the TCB for the task and adds it to the task queue
	//Also initializes all values in the TCB
	tcbCount++;	

	tcbArray[tcbCount-1].next = null;
	tcbArray[tcbCount-1].previous = null;
	tcbArray[tcbCount-1].priority = priority;
	tcbArray[tcbCount-1].state = READY;
	tcbArray[tcbCount-1].taskFunction = task;
	tcbArray[tcbCount-1].context[0] = 0;
	tcbArray[tcbCount-1].context[1] = (unsigned short)taskStack;
	tcbArray[tcbCount-1].context[2] = 0;
	tcbArray[tcbCount-1].runCount = 0;

	readyHead = addToQueue(&tcbArray[tcbCount-1], readyHead);
	// printTasks();	
	if(!firstRun) {
		YKScheduler();
	}
}

void YKDispatcher(int first){
	if(first == 1){
		restoreContext();
	} else if (first == 2) {
		saveAndFirstRestoreContext();
	}
	else {
		saveAndRestoreContext();
	}
}

void YKRun(void){
	//printString("YKRun\n");
	//Calls the scheduler and begins the operation of the program

	//printTasks();
	firstRun = 0;
	YKScheduler();
}

void printTCB(TCB* tcb) {
	printString("{ priority = ");
	printInt(tcb->priority);
	printString(", delay = ");
	printInt(tcb->delay);
	printString("}\n");	
}

void printQueue(TCB* head, char* st) {
	TCB* cur = head;
	printString("Printing Queue: ");
	printString(st);
	printString("\n");

	while(cur != null) {
		printString(".");
		printTCB(cur);
		if(cur->next == null)
			printString("its null!\n");
		cur = cur->next;
	}

	printString("Done\n");
}

void YKDelayTask(unsigned count){
	//printString("YKDelayTask\n");
	//if (count > 0)Sets the state of the TCB to delayed and then calls the scheduler. 
	printQueue(delayedHead, "Delayed");
	if(count > 0) {
		curTCB->delay = count;
		curTCB->state = DELAYED;
		removeFromQueue(curTCB);
		delayedHead = addToQueue(curTCB, delayedHead);
		YKScheduler();
	}
}

void YKEnterISR(void){
	//printString("YKEnterISR\n");
	//Increment call depth
	callDepth++;
}


void YKExitISR(void){
	//printString("YKExitISR\n");
	//Decrement call depth
	callDepth--;
	// if(call depth is 0)
	//	call scheduler
	if(callDepth == 0 && curTCB != null) {
		YKScheduler();
	}
}

void YKScheduler(void){
	//printString("YKScheduler\n");
	//Peek the top ready task, if different from current task
	//call dispatcher

	if(curTCB == null) {
		YKCtxSwCount++;
		YKDispatcher(1); //first run
	}
	else if(curTCB != readyHead) {
		YKCtxSwCount++;
		if(curTCB->runCount == 0) {
			// curTCB->runCount = curTCB->runCount + 1;
			curTCB->runCount = 1;
			YKDispatcher(2);
		} else {
			// curTCB->runCount = curTCB->runCount + 1;
			curTCB->runCount = 1;
			YKDispatcher(0);
		}
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

// typedef struct TCB {
// 	unsigned short int context[3];//SS, SP, CS register will be saved here.
// 	State state;// an enumerated type. blocked, delayed, ready
// 	struct TCB * previous; //used when inserting and removing from queue (our queue will be a double linked list)
// 	struct TCB * next; // same as above
// 	unsigned delay; //How much time to delay
// 	unsigned char priority; //priority value of the current task
// 	void (* taskFunction)(void);
// 	unsigned char runCount;
// } TCB;

void YKTickHandler() {
	TCB* cur = delayedHead;
	printQueue(delayedHead, " Delayed (before YKTickHandler)");
	while(cur != null) {

		cur->delay--;
		if(cur->delay <= 0) {
			cur->state = READY;

			removeFromQueue(cur);

			readyHead = addToQueue(cur, readyHead);

		}
		cur = cur->next;
	}
	printQueue(delayedHead, " Delayed (after YKTickHandler)");
}
