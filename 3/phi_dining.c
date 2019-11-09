#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define N 5
#define EATING 0
#define THINKING 1
#define WAITING 2
#define LEFT (i-1+N)%N 
#define RIGHT (i+1)%N 
int phi_state[N] = {THINKING};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t phi[N];
void* phi_sem_process(void *arg);
void phi_takeup_fork(int i);
void test(int i);
void phi_putdown_fork(int i);
int printCount=0;
int main()
{

	pthread_t tid[N];
	int argument[N];
	for (int i = 0; i < N; ++i)
	{
		 argument[i] = i;
	}
	// pthread_mutex_init(&mutex,0,1);
	for (int i = 0; i < N; ++i)
	{
		pthread_mutex_init(&phi[i],NULL);
	}
	for(int i=0;i<N;i++)
	{
		pthread_create(&tid[i],NULL,phi_sem_process,&argument[i]);
	}
	for(int i=0;i<N;i++)
		pthread_join(tid[i],NULL);
	printf("all phi has quited\n");
	return 0;
}
void* phi_sem_process(void *arg)
{
	int i = *(int*)arg;
	printCount++;
	printf("phi_%d has been created   printCount=%d\n", i,printCount);
	for(int j=0;j<5;j++)
	{
		printCount++;
		printf("Iter %d : phi_%d is THINKING   printCount=%d\n",j,i,printCount);
		sleep(3);
		phi_takeup_fork(i);  // take up the fork or waiting 
		// if the following can execute, then it should get the fork
		 printCount++;
		printf("Iter %d : phi_%d is EATING  printCount=%d\n",j,i,printCount);
		sleep(10);
		phi_putdown_fork(i);
	}
	printf("phi_%d has quited\n",i);
	return NULL;
}

void phi_takeup_fork(int i)
{
	pthread_mutex_lock(&mutex);
	// sem_wait(&mutex);
	phi_state[i] = WAITING;
	test(i);  // try to get two fork
	// sem_post(&mutex);
	pthread_mutex_unlock(&mutex);
	// sem_wait(&phi[i]);
	pthread_mutex_lock(&phi[i]);

}

void test(int i)
{
	if(phi_state[i]==WAITING&&phi_state[LEFT]!=EATING&&phi_state[RIGHT]!=EATING)
	{
		phi_state[i] = EATING;
		// sem_post(&phi[i]);
		pthread_mutex_unlock(&phi[i]);
	}
}
void phi_putdown_fork(int i)
{
	// sem_wait(&mutex);
	pthread_mutex_lock(&mutex);
	phi_state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	// sem_post(&mutex);
	pthread_mutex_unlock(&mutex);
}