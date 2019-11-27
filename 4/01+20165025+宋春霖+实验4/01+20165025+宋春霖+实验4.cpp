#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INT_MAX ((1<<31)-1)
void load_workload(int workload_num);
void FIFO(int frame_number,int j);
void clock(int frame_number,int j);
void LRU(int frame_number,int j);
int buf[1500000];
int pageTable[50000000];
int pageTime[50000000];// store page's last referred time.
int currWorkLoad;
// int currPageInMem[2000];
// int currPageInMem[5000];
// int currPageInMemA[5000];
// int currPageInMemN[5000];
// only store page whether is in memory or not.
//  0->false  1->true

int main()
{
//    load_workload(12);
    //         FIFO(100,currWorkLoad);
//         FIFO(500,currWorkLoad);
//         FIFO(1000,currWorkLoad);
//         FIFO(2000,currWorkLoad);
//         FIFO(5000,currWorkLoad);
//         LRU(100,currWorkLoad);
//         LRU(500,currWorkLoad);
//         LRU(1000,currWorkLoad);
//         LRU(2000,currWorkLoad);
//         LRU(5000,currWorkLoad);
//         clock(100,currWorkLoad);
//         clock(500,currWorkLoad);
//         clock(1000,currWorkLoad);
//         clock(2000,currWorkLoad);
//         clock(5000,currWorkLoad);
	 for(int i=1;i<=6;i++)
     {
	 	memset(buf,0,1500000*4);
	 	load_workload(i);
	 	FIFO(100,currWorkLoad);
	 	FIFO(500,currWorkLoad);
	 	FIFO(1000,currWorkLoad);
	 	FIFO(2000,currWorkLoad);
	 	FIFO(5000,currWorkLoad);
	 	LRU(100,currWorkLoad);
         LRU(500,currWorkLoad);
         LRU(1000,currWorkLoad);
         LRU(2000,currWorkLoad);
         LRU(5000,currWorkLoad);
         clock(100,currWorkLoad);
         clock(500,currWorkLoad);
         clock(1000,currWorkLoad);
         clock(2000,currWorkLoad);
         clock(5000,currWorkLoad);
	 }
////       load_workload();
//         FIFO(100,currWorkLoad);
//         FIFO(500,currWorkLoad);
//         FIFO(1000,currWorkLoad);
//         FIFO(2000,currWorkLoad);
//         FIFO(5000,currWorkLoad);
//         LRU(100,currWorkLoad);
//         LRU(500,currWorkLoad);
//         LRU(1000,currWorkLoad);
//         LRU(2000,currWorkLoad);
//         LRU(5000,currWorkLoad);
//         clock(100,currWorkLoad);
//         clock(500,currWorkLoad);
//         clock(1000,currWorkLoad);
//         clock(2000,currWorkLoad);
//         clock(5000,currWorkLoad);


	return 0;
}

void load_workload(int workload_num)
{
	if(workload_num>0&&workload_num<13)
	{
		char buffer[20];
		memset(buffer,0,10);
		// printf("1\n");
		char work_num[3];
		if(workload_num>=10)
        {
		    work_num[0] = '0'+workload_num/10;
		    work_num[1] = '0'+workload_num%10;
		    work_num[2] = 0;
        }
		else
        {
		    work_num[0] = '0'+workload_num;
		    work_num[1] = 0;
		    work_num[2] = 0;
        }

		char filename[20] = {'.','/','w','o','r','k','l','o','a','d'};
		strcat(filename,work_num);
		strcat(filename,".txt");
		FILE *fp = fopen(filename,"r");
		if(fp==NULL)
		printf("fail to open %s\n",filename);
		else
		{
			// printf("2\n");
			int number=0;
			while(fgets(buffer,20,fp)!=NULL)
			{

				buf[number] = atoi(buffer);
				number++;

			}
			currWorkLoad  = workload_num;
		}
	}
	else
		printf("wrong workload_num\n");
}

void FIFO(int frame_number,int j)
{
	// int *FIFO_buf = malloc((frame_number+1)*sizeof(int));
	int FIFO_buf[frame_number+1];
	int head = 0;
	int rear = 0;
	int acessCount = 0;
	int pageFaultCount = 0;
	int remainPhynum = frame_number;
	memset(pageTable,0,4*50000000);
	// printf("4\n");
	// printf("%d",pageTable[buf[0]]);
	int i=0;
	while(buf[i]!=0)
	 {	 // dprintf("VA:%d\n",buf[i]);

		acessCount++;
		// printf("%d",acessCount);
		// printf(" buf: %d\n",buf[i]);
		if (pageTable[buf[i]]==0)
		{
			// printf("happen pageFault\n");
			// pageFault
			if(remainPhynum>0)
			{
				// still has Physics memory
				FIFO_buf[rear] = buf[i];
				rear = (rear+1)%(frame_number+1);
				remainPhynum--;

			}
			else
			{
				// has no Physics memory
			 	pageTable[FIFO_buf[head]] = 0;
			 	head = (head+1)%(frame_number+1);
			 	FIFO_buf[rear] = buf[i];
			 	rear = (rear+1)%(frame_number+1);
			}

			pageFaultCount++;
		}
		pageTable[buf[i]] = 1;
		i++;
	}
	// printf("pageFault number:%d\n",pageFaultCount);
	printf("FIFO: workload:%d Frame number:%d pageFault rate:%f\n", j, frame_number,(float)pageFaultCount/acessCount);
	// free(FIFO_buf);
}

void LRU(int frame_number,int j)
{
		// printf("HELLO\n");

	  // printf("HELLO2\n");
	 int *currPageInMem = (int*)malloc(sizeof(int)*frame_number);
	 // int * currPageInMem = new int[frame_number];
	 memset(currPageInMem,0,sizeof(currPageInMem));
	 // int remainPhynum = frame_number;
	 memset(pageTime,0,sizeof(pageTime));
	 memset(pageTable,0,sizeof(pageTable));
	 // printf("HELLO3\n");
	 int top = 0;
	 int acessCount=0;
	 int pageFaultCount=0;
	 int i=0;
	 while(buf[i]!=0)
	 {
	 	acessCount++;
	 	// printf("acessCount:%d buf:%d\n", i,buf[i]);
	    if (pageTable[buf[i]]==0)
	    {
	    	// page fault
//	    	 printf("pagefault:%d %d\n",i,buf[i]);
	    	pageFaultCount++;
	    	if (top<frame_number)
	    	{
	    		// remainPhynum--;
	    		currPageInMem[top] = buf[i];
	    		pageTable[buf[i]] = 1;
	    		pageTime[buf[i]] = i;
	    		top++;

	    	}
	    	else
	    	{
	    		// has no remained physical page
	    		// swap a page out
	    		// m++;
	    		// printf("swap in and out,%d\n",m);
	    		int minVPageTime = INT_MAX;
	    		int minVPageNum;
	    		for (int k = 0; k < frame_number; ++k)
	    		{
	    			if(pageTime[currPageInMem[k]]<minVPageTime)
	    			{
	    				minVPageTime = pageTime[currPageInMem[k]];
	    				minVPageNum = k;
	    			}
	    		}
	    		pageTable[currPageInMem[minVPageNum]] = 0;
	    		currPageInMem[minVPageNum] = buf[i];
	    		pageTable[buf[i]] = 1;
	    		pageTime[buf[i]] = i;
	    	}
	    	// swap current page in and update the access Time
	    	// pageTable[buf[i]] = 1;
	    	// pageTime[buf[i]] = i;

	    }
	    else
	    {
	    	// hit this page and update the access Time
	    	pageTime[buf[i]] = i;
	    }

	    i++;
	 }
	 printf("LRU: workload:%d Frame number:%d pageFault rate:%f\n", j, frame_number,(float)pageFaultCount/acessCount);
	  free(currPageInMem);
}
void clock(int frame_number,int j)
{
	int *currPageInMemA = (int*)malloc(sizeof(int)*frame_number);  // store page's access bit that is in memory
	int *currPageInMemN = (int*)malloc(sizeof(int)*frame_number);  // store vitual page's num that is in memory
//    int currPageInMemA[frame_number];
//    int currPageInMemN[frame_number];
	memset(currPageInMemA,0,sizeof(int)*frame_number);
	memset(currPageInMemN,-1,sizeof(int)*frame_number);
	memset(pageTable,0,sizeof(int)*50000000);
	int acessCount =0;
	int pageFaultCount = 0;
	// int remainPhynum = frame_number;
	int top = 0;
	int i=0;
	int currPointer = 0; // clock's current pointer
	while(buf[i]!=0)
	{
		// printf("acessCount:%d\n",acessCount);
		acessCount++;
		if (pageTable[buf[i]]==0)
		{
			pageFaultCount++;
			//pageFault
			if (top<frame_number)
			{
				// still has physical memory

				currPageInMemA[top] = 0;
				currPageInMemN[top] = buf[i];
//				if(i<300)
//				{
//					printf("Condition1 :%d change memory%d and accessbit to %d(at physical:%d)\n",i,currPageInMemN[top],
//						currPageInMemA[top],top);
//				}
				top++;

			}
			else
			{
				// printf("Condition2 :%d\n",i);
				// swap out a page that is selected
				while(currPageInMemA[currPointer]==1)
				{
					currPageInMemA[currPointer] = 0;
					currPointer = (currPointer+1)%frame_number;
				}
				pageTable[currPageInMemN[currPointer]] = 0;
				currPageInMemA[currPointer] = 1;
				currPageInMemN[currPointer] = buf[i];
				currPointer = (currPointer+1)%frame_number;
//				if(i<300)
//				{
//					printf("Condition2 :%d change memory%d and accessbit to %d(at physical:%d)\n",i,currPageInMemN[currPointer-1],
//						currPageInMemA[currPointer-1],currPointer-1);
//				}
			}
			pageTable[buf[i]] = 1;
		}
		else
		{
			// printf("Condition3 :%d\n",i);
			for (int k = 0; k < frame_number; ++k)
			{
				if (currPageInMemN[k]==buf[i])
				{
					currPageInMemA[k] = 1;
//					if (i<300)
//					printf("Condition3 :%d change memory%d accessbit to %d(at physical:%d)\n",i,currPageInMemN[k],currPageInMemA[k],k);
				}
			}

		}
		// if(top==frame_number)
		// 	printf("when physical memory use up, i=%d\n", i);
//		if(i ==100)
//        {
//		    printf("i=100\n");
//        }
//		if(i%50==0&&i<300)
//		{
//			printf("================ %d\n", i);
//			for (int d = 0; d < frame_number; ++d)
//			{
//				printf("<%d %d %d>  ",d,currPageInMemN[d],currPageInMemA[d]);
//			}
//			printf("================\n");
//		}
//		if(i==251)
//		{
//
//			printf("************* %d\n", i);
//			for (int d = 0; d < frame_number; ++d)
//			{
//				printf("<%d %d %d>  ",i, currPageInMemN[d],currPageInMemA[d]);
//			}
//			printf("**************\n");
//		}
		i++;
	}
//	if(currPageInMemN[1]==0)
//    {
//	    printf("\n\n\nattention!!!! currPageInMemN[1]=0 i=%d",i);
//    }
	// printf("pageFaultCount:%d  ",pageFaultCount);
	// printf("accessCount:%d\n",acessCount );
	printf("clock: workload:%d Frame number:%d pageFault rate:%f\n", j,frame_number,(float)pageFaultCount/acessCount);
	 free(currPageInMemA);
	 free(currPageInMemN);
}