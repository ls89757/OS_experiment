#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#define INT_MAX 2147483647
#define ThreadNum 10
#define Ready 0 
#define Waiting 1
#define Finish 2

typedef struct VirtualPCB
{
	int tid;
	int waitTime;
	int runTime;
	int arriveTime;
	int beginTime;
	int interuptTime;
	int endTime;
	int turnaroundTime;
	int state;
}VirtualPCB;

typedef struct VPCBnode
{
	VirtualPCB PCB;
	struct VPCBnode* next;
}VPCBnode;

struct VPCBnode* head;
struct VPCBnode* tail;

/*
   init 10 SIMULATED threads
 */
void init()
{
	int n;
	srand(time(NULL));
	head = malloc(sizeof(struct VPCBnode));
	head->next = NULL;
	tail = head;
	for(n=0;n<ThreadNum;n++)
	{
		// insert every PCBNode to linked list tail
		struct VPCBnode *node = malloc(sizeof(VPCBnode));
		node->PCB.tid = n;
		node->PCB.runTime = rand()%50;
		node->PCB.waitTime = 0;
		node->PCB.arriveTime = rand()%20;
		node->PCB.beginTime = 0;
		node->PCB.interuptTime = 0;
		node->PCB.endTime = 0;
		node->PCB.turnaroundTime = 0;
		node->PCB.state = Ready;
		tail->next = node;
		tail = node;
		printf("thread %d has created,runTime=%d,arriveTime=%d\n",n,node->PCB.runTime,node->PCB.arriveTime);
	}

}


bool empty(){
    return ThreadNum==0;
}
void swap(int* a,int* b){
    int temp=*a;
    *a=*b;
    *b=temp;
}
void sortByArriveTime()
{
	if(empty()||ThreadNum==1)
		return;
	VPCBnode temp;
	VPCBnode* prev;
	VPCBnode* cur;
	for (int i = 0; i < ThreadNum-1; ++i)
	{
		prev = head->next;
		cur = prev->next;
		for (int j = 0; j < ThreadNum-i-1; ++j)
		{
			if(prev->PCB.arriveTime>cur->PCB.arriveTime)
			{
				swap(&(prev->PCB.tid),&(cur->PCB.tid));
				swap(&(prev->PCB.waitTime),&(cur->PCB.waitTime));
				swap(&(prev->PCB.runTime),&(cur->PCB.runTime));
				swap(&(prev->PCB.arriveTime),&(cur->PCB.arriveTime));
				swap(&(prev->PCB.beginTime),&(cur->PCB.beginTime));
				swap(&(prev->PCB.interuptTime),&(cur->PCB.interuptTime));
				swap(&(prev->PCB.endTime),&(cur->PCB.endTime));
				swap(&(prev->PCB.turnaroundTime),&(cur->PCB.turnaroundTime));
				swap(&(prev->PCB.state),&(cur->PCB.state));
			}
			prev = prev ->next;
			cur = cur ->next;
		}
		
	}
}
void PCBstore(VPCBnode* source, VPCBnode * store)
{
	VPCBnode * cur = source->next;
	VPCBnode * temp = store;
	for(int n=0;n<ThreadNum;n++)
	{
		// insert every PCBNode to linked list tail
		struct VPCBnode *node = malloc(sizeof(VPCBnode));
		memset(node,0,sizeof(VPCBnode));
		node->PCB.tid =  cur->PCB.tid;
		node->PCB.runTime = cur->PCB.runTime;
		node->PCB.waitTime =  cur->PCB.waitTime;
		node->PCB.arriveTime = cur->PCB.arriveTime;
		node->PCB.beginTime = cur->PCB.beginTime;
		node->PCB.interuptTime = cur->PCB.interuptTime;
		node->PCB.endTime = cur->PCB.endTime;
		node->PCB.turnaroundTime = cur->PCB.turnaroundTime;
		node->PCB.state = cur->PCB.state;
		temp->next = node;
		temp = temp->next;
		cur = cur->next;
	}

}
void PCBrestore(VPCBnode* store,VPCBnode* restore)
{
	VPCBnode * cur = store->next;
	VPCBnode * temp = restore->next;
	for(int n=0;n<ThreadNum;n++)
	{
		temp->PCB.tid = cur->PCB.tid;
		temp->PCB.runTime = cur->PCB.runTime;
		temp->PCB.waitTime = cur->PCB.waitTime;
		temp->PCB.arriveTime = cur->PCB.arriveTime;
		temp->PCB.beginTime = cur->PCB.beginTime;
		temp->PCB.interuptTime = cur->PCB.interuptTime;
		temp->PCB.endTime = cur->PCB.endTime;
		temp->PCB.turnaroundTime = cur->PCB.turnaroundTime;
		temp->PCB.state = cur->PCB.state;
		temp = temp ->next;
		cur = cur ->next;
	}
}
void printScheduleInfo(VPCBnode* head)
{
	VPCBnode * temp = head->next;
	printf("          waitTime   beginTime   endTime  turnaroundTime\n");
	for(int i=0;i<ThreadNum-1;i++)
	{
		printf("thread %d: %d         %d         %d         %d\n",temp->PCB.tid,temp->PCB.waitTime,temp->PCB.beginTime,temp->PCB.endTime,temp->PCB.turnaroundTime);
		temp = temp -> next;
	}
}
int  FCFS()
{
	printf("------------FCFS SCHEDULING ALOGORITHM------------\n");
	sortByArriveTime();
	int totalWaitingTime = 0;
	int currentTime = head ->next->PCB.arriveTime;
	if (empty())
	{
	   printf("there is no task\n");
	   return -1;
	}
	VPCBnode * curr = head -> next;
	while(1)
	{
		curr->PCB.beginTime = currentTime;
		curr->PCB.endTime = currentTime + curr->PCB.runTime;
		curr->PCB.waitTime = currentTime - curr->PCB.arriveTime;
		curr->PCB.turnaroundTime = curr->PCB.waitTime + curr->PCB.runTime;
		curr->PCB.state = Finish;
		totalWaitingTime += curr->PCB.waitTime;
		printf("task %d has finish\n",curr->PCB.tid);
		// printf("curr->next:%x\n",curr->next);
		currentTime += curr->PCB.runTime;
		if (curr->next==NULL)
			break;
		curr = curr->next;
	}
	printScheduleInfo(head);
	printf("FCFS scheduling finished. Average Waiting Time = %f\n",(float)totalWaitingTime/ThreadNum);
	printf("------------FCFS SCHEDULING END------------\n");
	return 0;
}
int SJF()
{
	printf("------------SJF SCHEDULING ALOGORITHM------------\n");
	sortByArriveTime();
	VPCBnode * curr = head->next;
	VPCBnode * shortest = head->next;
	int currShortestPeriod = INT_MAX;
	int currentTime = head->next->PCB.arriveTime;
	int totalWaitingTime = 0;
	int allFinish = 1;
	while(1)
	{
		for (int i = 0; i < ThreadNum; ++i)
		{
		 if(curr->PCB.runTime<=currShortestPeriod&&(curr->PCB.state!=Finish)&&curr->PCB.arriveTime<=currentTime)
			{
			 shortest = curr;
			 currShortestPeriod = curr->PCB.runTime;
			 allFinish = 0;
			}
			curr = curr -> next;
		}
		// printf("current shortest is %d current time = %d\n",shortest->PCB.tid,currentTime);
		// printf("isfinish: %d\n",allFinish);
		if(!allFinish)
		{
			shortest->PCB.beginTime = currentTime;
			shortest->PCB.endTime = currentTime + shortest->PCB.runTime;
			shortest->PCB.waitTime = currentTime - shortest->PCB.arriveTime;
			shortest->PCB.turnaroundTime = currentTime + shortest->PCB.runTime;
			shortest->PCB.state = Finish;
			totalWaitingTime +=shortest->PCB.waitTime;
			printf("task %d has finished\n",shortest->PCB.tid);
			currentTime+=shortest->PCB.runTime;
		}
		

		curr = head->next;
		shortest = head->next;
		currShortestPeriod = INT_MAX;
		if (allFinish)
		 break;
		allFinish =1;
	}
	printScheduleInfo(head);
	printf("SJF scheduling finished. Average Waiting Time = %f\n",(float)totalWaitingTime/ThreadNum);
	printf("------------SJF SCHEDULING END------------\n");

}
int RR(int slide)
{
	printf("------------RR SCHEDULING ALOGORITHM------------\n");
	sortByArriveTime();
	int allFinish = 1;
	int totalWaitingTime = 0;
	int currentTime = head->next->PCB.arriveTime;
	VPCBnode * curr = head->next;
	while(1)
	{
		for (int i = 0; i < ThreadNum; ++i)
		{
			if (curr->PCB.state!=Finish&&curr->PCB.arriveTime<=currentTime)
			{
				printf("current thread %d  currentTime %d\n",curr->PCB.tid,currentTime);
				// execute current thread
				if (curr->PCB.state==Ready)
				{
					// the first time to run this thread
					curr->PCB.beginTime = currentTime;
					curr->PCB.waitTime = currentTime - curr->PCB.arriveTime;
				}
				if (curr->PCB.runTime-curr->PCB.interuptTime<=slide)
				{
					// would not use up this slide OR just fit in slide 
					curr->PCB.waitTime += currentTime - curr->PCB.interuptTime;
					currentTime += curr->PCB.runTime - curr->PCB.interuptTime;
					curr->PCB.endTime = currentTime;
					curr->PCB.state = Finish;
					curr->PCB.turnaroundTime = currentTime - curr->PCB.arriveTime;
					totalWaitingTime += curr->PCB.waitTime;

				}
				else
				{
					currentTime += slide;
					curr->PCB.interuptTime +=slide; 
					curr->PCB.state = Waiting;
				}
				allFinish = 0;
			}
			curr = curr->next;
		}
		curr = head->next;
		if (allFinish)
			break;
		allFinish = 1;
		// Break Condition: all finish
	}
	printScheduleInfo(head);
	printf("RR scheduling finished. Average Waiting Time = %f\n",(float)totalWaitingTime/ThreadNum);
	printf("------------RR SCHEDULING END------------\n");
}
int main()
{

	init();
	VPCBnode* store = malloc(sizeof(VPCBnode));
	PCBstore(head,store);
	FCFS();
	PCBrestore(store,head);
	SJF();
	PCBrestore(store,head);
	RR(3);
	PCBrestore(store,head);


}