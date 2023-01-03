//EXTENDED SHELL ---COUNT

#include<stdio.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
char comm[100],*ptr;
char *args[10];
int tot_arg;
int count( char option, char *fname)// count c/w/l   <filename>
{
	int ccnt,wcnt,lcnt,wflag;
	int ch;
	FILE *fp;
	int i;
	fp= fopen(fname,"r");
	if( fp == NULL )
	{
		printf("\nUnable to open file");
		exit(0);
	}
	ccnt =wcnt = lcnt = 0;
	wflag=1;
	ch = fgetc(fp);
	while ( ch != EOF)
	{
		ccnt++;
		if( ch ==' ' || ch== '\t')
		{
			if( wflag == 0 )
			{
				wcnt++;
				wflag = 1;
			}	
		}
		else
		if( ch == '\n')
		{
			lcnt++;
			if( wflag == 0 )
			{
				wcnt++;	
				wflag = 1;
			}
		}
		else
		{
			wflag = 0;
		}
		ch = fgetc(fp);
	}//while
	if( wflag == 0 )
	{	
		wcnt++;
		lcnt++;
	}
	fclose(fp);
	switch(option)
	{
	case 'c' : // print all chars 
			printf("\nTot chars = %d\n",ccnt);
			break;
	case 'w' : // print all words 
			printf("\nTot words = %d\n",wcnt);
			break;
	case 'l' : // print all lines 
			printf("\nTot lines = %d\n",lcnt);
			break;
	} // switch
	return;
}// count

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

	if(strcmp(args[0],"count") == 0 )
		count(*args[1],args[2]);
	else
	if(strcmp(args[0],"quit") == 0 )
		exit(0);		
}// take action

