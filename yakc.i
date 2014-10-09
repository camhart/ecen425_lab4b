# 1 "yakc.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "yakc.c"
# 1 "yakk.h" 1




unsigned int YKCtxSwCount = 0;

unsigned int YKIdleCount = 0;

unsigned int YKTickNum = 0;

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
 unsigned short int context[5];
 State state;
 struct TCB * previous;
 struct TCB * next;
 unsigned char priority;
 void (* taskFunction)(void);
} TCB;



void YKInitialize(void);


void YKEnterMutex(void);


void YKExitMutex(void);


void YKIdleTask(void);





void YKNewTask(void (* task)(void), void *taskStack, unsigned char priority);



void YKRun(void);


void YKDelayTask(unsigned count);


void YKEnterISR(void);


void YKExitISR(void);




void YKScheduler(void);



void YKDispatcher(void);




YKSEM* YKSemCreate(int initialValue);


void YKSemPend(YKSEM *semaphore);
# 93 "yakk.h"
void YKSemPost(YKSEM *semaphore);





YKQ *YKQCreate(void **start, unsigned size);



void *YKQPend(YKQ *queue);







int YKQPost(YKQ *queue, void *msg);
# 122 "yakk.h"
YKEVENT *YKEventCreate(unsigned initialValue);



unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);





void YKEventSet(YKEVENT *event, unsigned eventMask);







void YKEventReset(YKEVENT *event, unsigned eventMask);
# 2 "yakc.c" 2




TCB* readyHead, blockedHead, delayedHead;

TCB tcbArray[10];

unsigned int tcbCount = 0;

void YKInitialize(void){



 tcbArray[tcbCount].state = READY;




}

void YKIdleTask(void){




 while(1){
  if(2 > 1)
   YKIdleCount++;
 }
}
addToQueue(TCB * tcb, TCB* listHead){

 TCB * pos = listHead;
 if(listHead == 0)
  listHead = tcb;
 else{
  while(tcb->priority < pos->priority){
   pos = pos->next;
   if(pos == 0){
    pos->next = tcb;
    tcb->previous = pos;
    tcb->next = 0;
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
# 64 "yakc.c"
 tcbCount++;

 tcbArray[tcbCount-1].priority = priority;
 tcbArray[tcbCount-1].state = READY;
 tcbArray[tcbCount-1].taskFunction = task;
 tcbArray[tcbCount-1].context[0] = 0;
 tcbArray[tcbCount-1].context[1] = taskStack;
 tcbArray[tcbCount-1].context[2] = task;
 tcbArray[tcbCount-1].context[3] = 0;
 tcbArray[tcbCount-1].context[4] = 0x100;

 addToQueue(&tcbArray[tcbCount-1], readyHead);

}
void YKRun(void){

}
void YKDelayTask(unsigned count){

}

void YKEnterISR(void){

}
void YKExitISR(void){



}
void YKScheduler(void){


}
void YKDispatcher(void){



}
YKSEM* YKSemCreate(int initialValue){

}
void YKSemPend(YKSEM *semaphore){







}

void YKSemPost(YKSEM *semaphore){



}

YKQ *YKQCreate(void **start, unsigned size){


}
void *YKQPend(YKQ *queue){






}
int YKQPost(YKQ *queue, void *msg){
# 143 "yakc.c"
}
YKEVENT *YKEventCreate(unsigned initialValue){


}
unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode){




 }
void YKEventSet(YKEVENT *event, unsigned eventMask){






}
void YKEventReset(YKEVENT *event, unsigned eventMask){

}
