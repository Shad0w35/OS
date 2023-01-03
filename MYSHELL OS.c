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

int search(char option, char *pattern, char *fname);
int count( char option, char *fname);
int typeline( char *option, char *fname);
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

	if(strcmp(args[0],"search") == 0 )
		search(*args[1],args[2],args[3]);
	else
	if(strcmp(args[0],"count") == 0 )
		count(*args[1],args[2]);
	else
	if(strcmp(args[0],"typeline") == 0 )
		typeline(args[1],args[2]);
	else
	if(strcmp(args[0],"list") == 0 )
		list(*args[1],args[2]);
	else
	{	
		if( ptr == NULL )
			exit(0);
		if ( strlen(comm) <= 1 )
			return;
		if( ( pid = fork() ) < 0 )
		{
			printf("\nUnable to create process");
		}
		else
		if(  pid == 0 ) // child
		{   sscanf(comm,"%s",comm);
			execvp(comm,args);
			strcpy(str,"./");
			strcat(str,args[0]);
			execvp(str,args);
			printf("\n%s : command not found",comm);
			exit(0);
		}
		else //parent
		 waitpid(pid,&status,0);
	} // else
		
}// take action


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

int typeline( char *option, char *fname) 
{
	FILE *fp;
	int tot_lines,ch,n,cnt;
	//---- count the lines
	fp= fopen(fname,"r");
	if( fp == NULL )
	{
		printf("\nUnable to open file");
		exit(0);
	}
	tot_lines = 0;//count total lines
	ch = fgetc(fp);
	while ( ch != EOF )
	{
		if( ch == 10 ) //new line char
		tot_lines++;
		ch = fgetc(fp);
	}
	fclose(fp);
	//---- take action
	if( strcmp(option,"a")== 0 ) // print all lines
	{
		fp= fopen(fname,"r");
		ch = fgetc(fp);
		while ( ch != EOF )
		{
			printf("%c",ch);
			ch = fgetc(fp);
		}
		fclose(fp);
	}
	else
	if( option[0] =='+') //print first n lines)
	{
		n = atoi(option+1);
		if( n > tot_lines)
		{	
			printf("\nInvalid  option");
			exit(0);
		}
		fp= fopen(fname,"r");
		cnt = 0;
		while( cnt < n )
		{
			ch = fgetc(fp);
			printf("%c",ch);
			if( ch == 10)
				cnt++;
		}
		fclose(fp);
	}
	else
	if( option[0] =='-') //print last  n lines)
	{
		n = atoi(option+1);
		if( n > tot_lines)
		{	
			printf("\nInvalid  option");
			exit(0);
		}
		fp= fopen(fname,"r");
		cnt = 0;
		while( cnt < tot_lines -n )
		{
			ch = fgetc(fp);
			if( ch == 10)
				cnt++;
		}
		while( (ch=fgetc(fp)) != EOF )
			printf("%c",ch);
		fclose(fp);
	}
	else
	{
		printf("\nInvalid option");
		exit(0);
	}
	
	return(0);
} //typeline 

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

