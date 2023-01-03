#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<dirent.h>

char comm[100],*ptr;
char *args[10];
int tot_arg;


int list( char option, char dirname[]);

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

if(strcmp(args[0],"list") == 0 )
		list(*args[1],args[2]);
	else
	if(strcmp(args[0],"quit") == 0 )
				exit(0);
}// take action

			
int list( char option, char dirname[])
{
	DIR *dp;
	struct dirent *dent;
	struct stat buff;
	char orgdir[100];
	int cnt,reply;
	getcwd(orgdir,90);
	chdir(dirname);
	switch(option)
	{
	case 'f' : // list all dir entries
			dp= opendir(".");
			if( dp == NULL )
			{
				printf("\nUnable to open dir");
				exit(0);
			}
			dent = readdir(dp);
			while( dent != NULL )
			{
				printf("\n%s",dent->d_name);
				dent= readdir(dp);
			}
			printf("\n");
			break;
	case 'n' : // list number all dir entries
			cnt =0;
			dp= opendir(".");
			if( dp == NULL )
			{
				printf("\nUnable to open dir");
				exit(0);
			}
			dent = readdir(dp);
			while( dent != NULL )
			{
				cnt++;
				dent= readdir(dp);
			}
			printf("\ntot dir entries = %d\n",cnt);
			closedir(dp);
			break;
	case 'i' : // list all dir entries with inode
			dp= opendir(".");
			if( dp == NULL )
			{
				printf("\nUnable to open dir");
				exit(0);
			}
			dent = readdir(dp);
			while( dent != NULL )
			{
				reply = stat(dent->d_name,&buff);
				if( reply < 0 )
				printf("\nError");
				else
				printf("\n%s %ld",dent->d_name,buff.st_ino);
				dent= readdir(dp);
			}
			closedir(dp);
			break;
	default :	printf("\nInvalid option");
			exit(0);
	} // switch
	chdir(orgdir);
	return(0);
}// main


