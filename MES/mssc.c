/*
FILE: mssc.c

UNIT: CNCO3002

PURPOSE: Act as interface between client and server

REFERENCE: 
[1] Curtin University.(AU). Worksheet 03 (tcpcli01.c). In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au
*/

#include	"acc.h"

int main(int argc, char **argv)
{
	
	int			sockfd;
	struct sockaddr_in	servaddr;
	char str[100];

	if (argc == 3)
	{
		//Creating a raw socket that is in closed state		
		sockfd = Socket(AF_INET, SOCK_STREAM, 0);

		//Replacing every byte of server's address structure with zero
		bzero(&servaddr, sizeof(servaddr));

		//Filling details of servers address structure
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(atoi(argv[2]));

		//Converting server's address structure from text form to binary form
		Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
		//Establishing a reliable connection with the server
		//This function moves socket from closed state to established state if doesn't return any error
		Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	
		//To change the prompt for the first time
		printf("MES-C > ");

		//Contains methods of communication
		str_cli(stdin,sockfd);		// do it all 

		exit(0);	
	}
	else
	{
		printf("MES-C > ");
		err_quit("usage: mssc.out <IPaddress> <port number>");
	}

	
	exit(0);
	
}


