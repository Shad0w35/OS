#include<stdio.h>
//#include<conio.h>
#include<stdlib.h>
#include<time.h>
struct procdata
{
	int atime,cpubt,inrq;
}proc[30];
struct gchart
{
	int stime,ctime,pid;
}gchart[50];
struct rqnode
{
	int pid;
	struct rqnode *next;
}*front=NULL,*rear=NULL,*curr,*prev;
int currtime=0,gindex=0,wtime[50],totime[50],i,n;
int finish_proc()
{
	for(i=1;i<=n;i++)
	if(proc[i].cpubt!=0)
	return 0;
	return 1;
}
void add_proc_req()
{
	for(rear=front;rear->next!=NULL;rear=rear->next); //this we have to do because some time rear & curr on same position in the Q,when curr is free then rear also free so to occupy rear's correct position this is does.
	for(i=1;i<=n;i++)
	if(proc[i].inrq==0 && proc[i].atime<=currtime && proc[i].cpubt!=0)
	{
		curr=(struct rqnode *)malloc(sizeof(struct rqnode));
		curr->pid=i;
		curr->next=NULL;
		if(front==NULL)
		front=curr;
		else
		rear->next=curr;
		rear=curr;
		proc[i].inrq=1;
	}
}
int get_sjf_proc()
{
	int pid;
	if(front==NULL)
	return -1;
	for(pid=front->pid,curr=front->next;curr!=NULL;curr=curr->next)
	if(proc[curr->pid].cpubt<proc[pid].cpubt)
	pid=curr->pid;
	for(curr=front;curr->pid!=pid;prev=curr,curr=curr->next);//*
	if(curr==front)
	front=front->next;           //from * to *,code is written for
	else			      //'freeing' the curr node in SJF
	prev->next=curr->next;
	free(curr); //*
	return pid;
}
int getctime(int pid)
{
	for(i=gindex;i>0;i--)
	if(gchart[i].pid==pid)
	return gchart[i].ctime;
	return proc[pid].atime;
}
void schedule_proc()
{
	while(!finish_proc())
	{
		int pid;
		add_proc_req();
		pid=get_sjf_proc();
		proc[pid].inrq=0;
		if(pid==-1)
		{
			currtime++;
			continue;
		}
		wtime[pid]+=(currtime-getctime(pid));//getctime() return sespending time of pid,default arrival time.
		gchart[++gindex].stime=currtime;
		gchart[gindex].pid=pid;
		currtime++;
		proc[pid].cpubt--;
		gchart[gindex].ctime=currtime;
		if(proc[pid].cpubt==0)
		{
			totime[pid]+=(currtime-proc[pid].atime);
			proc[pid].atime=gchart[gindex].ctime+2;//2 is io operation
	  //		totime[pid]+=2;
		}
	}
}
void print_times()
{
	int avgw=0,avgt=0;
	printf("\n WEIGHTING TIME|TURN AROUND TIME");
	for(i=1;i<=n;i++)
	{
		printf("\n %d\t\t|%d",wtime[i],totime[i]);
		avgw+=wtime[i];
		avgt+=totime[i];
	}
	printf("\n AVERAGE WEIGHTING TIME=%.2f",(float)avgw/n);
	printf("\n AVERAGE TURN AROUND TIME=%.2f",(float)avgt/n);
}
void print_gchart()
{
	printf("\n GANG CHART IS:");
	printf("\n_______________________________________________________\n");
	for(i=1;i<=gindex;i++)
	{
		printf("%d^|P-%d|",gchart[i].stime,gchart[i].pid);
	}
	printf("%d",gchart[gindex].ctime);
	printf("\n--------------------------------------------------------");
}
void main()
{
	//clrscr();
	printf("\n HOW MANY PROCESSES ? :");
	scanf("%d",&n);
	printf("\n Enter All Processes A time & CPU B time :\n");
	for(i=1;i<=n;i++)
	{
		scanf("%d%d",&proc[i].atime,&proc[i].cpubt);
		proc[i].inrq=0;
	}
	schedule_proc();
	 randomize();
	for(i=1;i<=n;i++)
	{
		proc[i].inrq=0;
		while(proc[i].cpubt==0)
		proc[i].cpubt=random(10);
	}
	//schedule_proc();
	print_gchart();
	print_times();
	//getch();
}