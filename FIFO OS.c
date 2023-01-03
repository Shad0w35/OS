#include<stdio.h>

int frame[3]={-1,-1,-1};
int pg_found(int pno)
{
	int i;
	for(i=0;i<3;i++)
		if(frame[i]==pno)
		return i;
	return -1;
}
int main()
{
	int p_req[]={9,14,10,11,15,9,11,9,15,10,9,15,10,12,15};
	int size=15,page_fault=0,i;
	
	printf("\n FIFO page replacement");
	printf("\n page no  -----------------page frames");
	printf("\n---------------------------------------");
	for(i=0;i<size;i++)
	{
		if(pg_found(p_req[i])==-1)
		{
			frame[page_fault%3]=p_req[i];
			page_fault++;
			printf("\n %4d--------------------%4d%4d%4d",p_req[i],frame[0],frame[1],frame[2]);
		}
		else
			printf("\n %4d.......No Page fault",p_req[i]);
	}
	printf("\n-------------------------------------------");
	printf("\n Total page fault=%d",page_fault);
	
}

