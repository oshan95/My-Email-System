/*
FILE: client_thread.c

UNIT: CNCO3002

PURPOSE: Looks for data at both socket and standrad input. Once data is available display.

REFERENCE: 
[1] Curtin University.(AU). Worksheet 05 (strclithread.c): Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[2] "Print Statement Won't Print Before An Infinite Loop". 2018. Stack Overflow. https://stackoverflow.com/questions/13667625/print-statement-wont-print-before-an-infinite-loop.

[3] Stevens, W. Richard, Bill Fenner, and Andrew M Rudoff. 1999. UNIX Network Programming. chapter 26 (threads/strclithread.c). 1st ed. Boston: Addison-Wesley/Prentice-Hall.
*/

#include	"acc.h"

void	*copyto(void *);

static int	sockfd;		/* global for both threads to access */
static FILE	*fp;

void str_cli(FILE *fp_arg, int sockfd_arg)
{
	char		recvline[MAXLINE];
	pthread_t	tid;

	sockfd = sockfd_arg;	// copy arguments to externals
	fp = fp_arg;	

	// does no pass argument 
	Pthread_create(&tid, NULL, copyto, NULL);

	//Checking if data available at the socket
	while (Readline(sockfd, recvline, MAXLINE) > 0)
	{
		Fputs(recvline, stdout);
		printf("MES-C > ");

		/*since the program stucks at slow system calls printf() function won't print until program
		terminates or encounter newline charactor. 	
		So fflush() function is used to print things in stdout buffer forcefully.*/ 
		fflush(stdout);
	}
	

}

void * copyto(void *arg)
{
	char	sendline[MAXLINE];

	//Checking if user have entered any data to standard input
	while ((void *) Fgets(sendline, MAXLINE, fp) != NULL)
	{
		Writen(sockfd, sendline, strlen(sendline));
		printf("MES-C > ");
	}

	//Shutdown() & return() won't execute until user enters CTRL+C( eof charachtor)
	Shutdown(sockfd, SHUT_WR);	// EOF on stdin, send FIN

	return(NULL);
}
