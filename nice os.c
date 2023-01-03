#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
main()
{
    int pid, retnice;
    printf("press DEL to stop process \n");
    pid = fork();
    for (;;)
    {
        if (pid == 0)
        {
            retnice = nice(−5);
            print("child gets higher CPU priority %d \n", retnice);
            sleep(10);
        }
        else
        {
            retnice = nice(4);
            print("Parent gets lower CPU priority %d \n", retnice);
            sleep(10);
        }
    }
}