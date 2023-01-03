#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
void main()
{
int pid;

pid=fork();
printf("\n pid=%d",(int) pid);
if(pid==0)
{
printf("\nBefore parent process terminated");
printf("\nChild Process with id  %d,Parent Process with id  %d ",(int) getpid(),(int) getppid());
sleep(1);
printf("\nAfter parent process terminated");
printf("\nOrphan Child Process with id %d,Parent Process with id %d",(int) getpid(),(int) getppid());
printf("\n child is exiting");
exit(0);
}
else
{
printf("\nI am parent process with id %d \n",(int) getpid());
printf("\n Parent is exiting");
exit(0);
}

}