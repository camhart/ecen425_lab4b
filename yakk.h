#include "yakc.c"

typedef enum {
	DELAYED, BLOCKED, RUNNING, READY
} State;

typedef struct {
	int count;
} YKSEM;

typedef struct {
	int something;
} YKQ;

typedef struct {
	int something;
} YKEVENT;

typedef struct TCB {
	unsigned short int context[5];//SS, SP, IP, CS and Flags register will be saved here.
	State state;// an enumerated type. blocked, delayed, ready
	struct TCB * previous; //used when inserting and removing from queue (our queue will be a double linked list)
	struct TCB * next; // same as above
	unsigned char priority; //priority value of the current task
	void (* taskFunction)(void);
} TCB;



void YKInitialize(void);
	//Create Idle task and add it to the ready queue

void YKEnterMutex(void);
	//Disable Interrupts

void YKExitMutex(void);
	//Enable Interrrupts

void YKIdleTask(void);
	/*while(true)
	*	increment YKIdleCount
	* 	do extra stuff
	*/

void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);
	//Creates the TCB for the task and adds it to the task queue
	//Also initializes all values in the TCB

void YKRun(void);
	//Calls the scheduler and begins the operation of the program

void YKDelayTask(unsigned count);
	//if (count > 0)Sets the state of the TCB to delayed and then calls the scheduler. 

void YKEnterISR(void);
	//Increment call depth

void YKExitISR(void);
	//Decrement call depth
	// if(call depth is 0)
	//	call scheduler

void YKScheduler(void);
	//Peek the top ready task, if different from current task
	//call dispatcher

void YKDispatcher(void);
	//save contexts of current task
	//restore context of next task
	//run next task

YKSEM* YKSemCreate(int initialValue);
	//Create YKSEM

void YKSemPend(YKSEM *semaphore);
	//if(semaphore > 0)
	//	decrement semaphore
	// 	return
	//else
	//	decrement semaphore
	//	block task
	// 	call scheduler


void YKSemPost(YKSEM *semaphore);
	//increment semaphore
	//unblock highest priority blocked task
	//call scheduler


YKQ *YKQCreate(void **start, unsigned size);
	//create Q
	//return &Q

void *YKQPend(YKQ *queue);
	//if(Q is empty)
	//	block calling task
	//	call scheduler
	//else
	//	pop message
	//	return &message

int YKQPost(YKQ *queue, void *msg);
	//if(Q is full)
	//	return 0
	// else
	//	push msg onto Q
	//	unblock highest priority task waiting for message
	//	if(task is unblocked)
	//		call scheduler
	//	else
	//		return 1

YKEVENT *YKEventCreate(unsigned initialValue);
	//Create YKEVENT
	//return &YKEVENT

unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);
	//if flag conditions met
	//	return
	//block task
	//call scheduler
 
void YKEventSet(YKEVENT *event, unsigned eventMask);
	//check blocked tasks
	//unblock certain tasks
	//if tasks were unblocked
	//	call scheduler
	//else 
	//	return

void YKEventReset(YKEVENT *event, unsigned eventMask);
	//reset bits to 0