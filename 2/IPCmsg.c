#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<errno.h>
int infoqueue;
// char *s = "ok";
// char *a = "ok";
char s[10] = "ok";
char readbuffer[100];
typedef struct msg_u
{
	long type;
	char messageEntity[10]; 
	//char* messageEntity;
}msg_u;
void childProcessFunction(int i);
int getqid()
{
	key_t key = ftok(".",'t');
	return msgget(key,0666|IPC_CREAT);

}
void sendMessage(int messageQueue, msg_u* msgObject)
{
	int ret=msgsnd(messageQueue,msgObject,sizeof(msgObject->messageEntity),IPC_NOWAIT);

	if(ret!=0)
	{
		printf("Message send failed\n");
		printf("ret=%d\n",ret);
		printf("messageQueue=%d\n",messageQueue);
		printf("messageEntity:%s\n", msgObject->messageEntity);
		printf("%s",strerror( errno ));
	}
}
struct msg_u receiveMessage(int messageQueue)
{
	msg_u msgObject;
	int length = msgrcv(messageQueue,&msgObject,sizeof(msgObject.messageEntity),0,IPC_NOWAIT);
	// printf("length=%d\n",length+2);
	// printf("===========\n");
	// printf("%s\n",msgObject.messageEntity);
	// printf("==========\n");
	// printf("afasdfasdfa\n");
	// printf("fdasdfas\n");

	return msgObject;
}
int main(int argc, char const *argv[])
{
	 infoqueue = getqid();
	int pid;
	 pid = fork();
	if(pid==0)
	{	// the first child process
		childProcessFunction(0);
	}
	else if(pid>0)
	{
		// main process
		wait(NULL);
		pid = fork();
		if (pid==0)
		{
			// the second child process
			childProcessFunction(1);
		}
	}
	else
	{
		printf("process created error\n");
	}
	return 0;
}
void childProcessFunction(int i)
{
	// char s[10] = "ok";

	switch(i)
	{
		case 0:
		printf("i am the first child process,i will write\n");
		FILE *fp=NULL;
		fp=fopen("2.txt","w");
		fprintf(fp, "this is file's contents\n");
		fclose(fp);
		msg_u msgObject;
		msgObject.type = 1;
		//msgObject.messageEntity = s;
		// printf("%s---------------\n",s);
		
		strcpy(msgObject.messageEntity,s);
		// printf("%s",msgObject.messageEntity);//
		sendMessage(infoqueue,&msgObject);
		break;
		case 1:
		printf("i am the second child process,i will read\n");
		// printf("asdaas\n");
		msg_u msgObject2 = receiveMessage(infoqueue);
		// printf("%s",msgObject2.messageEntity);
		int ret2 = strcmp(msgObject2.messageEntity,"ok"); 
		if(!ret2)
		{
			FILE *fp2 =NULL;
			fp2=fopen("2.txt","r");
			fgets(readbuffer,sizeof(readbuffer),(FILE*)fp2);
			printf("file's contents:%s",readbuffer);
			fclose(fp);
		}
		break;
	}
	exit(0);
}