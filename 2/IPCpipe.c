#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
char signalbuffer[10];
char readbuffer[100];
int fd[2],nbytes;
int w_fd;
int r_fd;
void childProcessFunction(int i);
int main()
{
	int pid;
 	pipe(fd);
 	w_fd = fd[1];
 	r_fd = fd[0];
	//create two children process
	for(int i=0;i<2;i++)
	{
		pid = fork();
		if(pid==0)
		{
			childProcessFunction(i);
		}
		else if(pid>0)
		{
			//parent process do nothing
		}
		else
		{
			printf("create process error\n");
		}
	}

}

void childProcessFunction(int i)
{

	switch(i)
	{
		case 0:
		printf("i am the first child process,i will write\n");
		close(r_fd);
		FILE *fp=NULL;
		fp=fopen("1.txt","w");
		fprintf(fp, "this is file's contents\n");
		fclose(fp);
		char *s = "ok";
		write(w_fd,s,2);
		// the first child process will execute the following code.
		break;
		case 1:
		printf("i am the second child process,i will read\n");
		close(w_fd);
		nbytes = read(r_fd,signalbuffer,sizeof(signalbuffer));
		if (strcmp(signalbuffer,"ok")==0)
		{
			// printf("i have received the signal\n");
			printf("ok\n");
			FILE *fp2=NULL;
			fp2=fopen("1.txt","r");
			fgets(readbuffer,sizeof(readbuffer),(FILE*)fp2);
			printf("file's contents:%s",readbuffer);
			fclose(fp);
		}
		// the second child process will execute the following code.
		break;
	}
	exit(0);

}