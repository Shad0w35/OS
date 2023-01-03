#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
//#include <bfd.h>
char comm[100],*ptr;
char *args[10];
int tot_arg;
int search(char option, char *pattern, char *fname)
{
	char buff[100],*ptr;
	int cnt;
	FILE *fp;
	int i;
	fp= fopen(fname,"r");
	if( fp == NULL )
	{
		printf("\nUnable to open file");
		exit(0);
	}
	switch(option)
	{
	case 'a' : // print all occ
			while ( !feof (fp))
			{
				buff[0] = '\0';
				fgets(buff,80,fp);
				ptr = strstr(buff,pattern);
				if( ptr != NULL )
					printf("%s",buff);
			}
			fclose(fp);
			break;
	case 'c' : // count occ
			cnt=0;
			while ( !feof (fp))
			{
				buff[0] = '\0';
				fgets(buff,80,fp);
				ptr = strstr(buff,pattern);
				while(ptr != NULL )
				{
					cnt++;
					ptr = ptr + strlen(pattern);
					ptr = strstr(ptr,pattern);
				} // while
			}
			fclose(fp);
			printf("\nThe serach string %s occurs %d times",pattern,cnt);
			break;

	case 'f' : //print first occ
			while ( !feof (fp))
			{
				buff[0] = '\0';
				fgets(buff,80,fp);
				ptr = strstr(buff,pattern);
				if( ptr != NULL )
				{
					printf("%s",buff);
					break;
				}
			}
			fclose(fp);
			break;
		} // switch
	return;
}//search 
main()
{
	do
	{
		printf("myshell $ ");
		getcomm();
		sep_arg();
		take_action();
	}while(1);
} // main

sep_arg()
{
	int i,j;
	i = j = tot_arg = 0;

	args[tot_arg] = (char*)malloc(sizeof(char) * 20);
	for(i=0; comm[i] !='\0' ; i++)
	{
		if( comm[i] == ' ')
		{
			args[tot_arg][j] = '\0';
			tot_arg++;
			args[tot_arg] = (char*)malloc(sizeof(char) * 20);	
			j=0;
		}
		else
		{
			args[tot_arg][j] =comm[i];
			j++;
		}
	} // while
	
	args[tot_arg][j] = '\0'; // complete last word
 	return;
}
getcomm()
{
	int len;
	ptr = fgets(comm,80,stdin);
	len = strlen(comm);
	comm[len-1] = '\0';
	return;
} // getcomm

take_action()
{
	char str[100];
	pid_t   pid;
	int status;

	if(strcmp(args[0],"search") == 0 )
		search(*args[1],args[2],args[3]);
	else
	if(strcmp(args[0],"quit") == 0 )
	exit(0);				
}// take action


