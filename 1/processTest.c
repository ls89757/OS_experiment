#include <unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<pthread.h>

void *find_primes(void)
{
	// int count = *(int*)(arg);
	printf("please input the count to assure the primes range \n");
	int count;
	scanf("%d",&count);
	int* primesArray = (int*)malloc(count*sizeof(int));
	int currentIndex =0;
	int flag = 1;
	primesArray[currentIndex] = 2; 
	for(int i=3;i<=count;i++)
	{
		for(int j=2;j<i;j++)
		{
			if(i%j==0)
			{	flag = 0;
				break;
			}
		}
		if (flag==1)
		{
			currentIndex++;
			primesArray[currentIndex] = i;
		}
		flag = 1;
	} 
	printf("primes:\n");

	for(int i=0;i<=currentIndex;i++)
	{
		printf("%d ",primesArray[i]);
	}
	printf("\n");
	return NULL;
} 
void *Fibonacci(void)
{
	printf("please input the count to assure the Fibonacci series count \n");
	int count;
	scanf("%d",&count);
   	int f1 = 1;
   	int f2 = 1;
   	int temp;
   	printf("Fibonacci:\n");
   	printf("1 1 ");
    for(int i=2;i<count;i++)
    {	
    	 printf("%d ",f1+f2);
    	 temp = f2;
    	 f2 = f1 + f2;
    	 f1 = temp;
    }
    printf("\n");
    return NULL;
}

int main()
{	int pid = getpid();
	int pid_3=0;
	int pid_4 = 0;
	int pid_5 = 0;
	int pid_6 = 0;
	pid_3 = fork();
	pthread_t pid_5_t1;
	pthread_t pid_5_t2;
	if(pid_3 == 0)
	{    // No.3 process
		printf("i am No.3 process pid=%d\n",getpid());
		// printf("Executing 'ls' command:\n");
		// execl("/bin/ls","ls");
		printf("Executing 'ps' command:\n");
		execl("/bin/ps","ps");
		exit(0);
		// printf("Executing 'free command:'\n");
		// execl("/bin/free","free");
	}
	else if(pid_3<0)
		printf("process fork failed \n");
	else
	{    // No.1 process
		waitpid(pid_3,NULL,WCONTINUED);
		// wait(NULL);
		printf("i am a parent process  pid=%d,i will create the fourth process\n",getpid());
		pid_4 = fork();
		if (pid_4 == 0)
		{
			// No.4 process
			printf("i am pid_4 process  pid=%d, i will create the fifth and the sixth process\n",getpid());
			pid_5 = fork();
			if (pid_5==0)
			{
				//No.5 process
				int num = 30;
				
				// scanf("%d",&num);
				int ret1 = pthread_create(&pid_5_t1,NULL,find_primes,NULL);
				pthread_join(pid_5_t1, NULL);
				int ret2 = pthread_create(&pid_5_t2,NULL,Fibonacci,NULL);
				pthread_join(pid_5_t2, NULL);
				printf("two process has ended\n");
			    exit(0);
			}
			else
			{
				//No.4 process
				pid_6 = fork();
				if(pid_6==0)
				{
					//No.6 process

					printf("i am No.6 process ,pid=%d\n",getpid());
					system("./Helloworld");
					exit(0);
				}
				else
				{
					//No.4 process
					waitpid(pid_5,NULL,WCONTINUED);
					waitpid(pid_6,NULL,WCONTINUED);
					printf("No.4's all children processes has quited\n");
					exit(0);
				}
			}

		}
		else
		{  // No.1 process
			printf("i am pid_4 process's parent pid=%d\n",getpid());
			waitpid(pid_4,NULL,WCONTINUED);
			printf("all process has quited\n");
		}
	} 
	return 0;
}
