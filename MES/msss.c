/*
FILE: msss.c

UNIT: CNCO3002

PURPOSE: Listening for client connections and serving them.

REFERENCE: 
[1] Curtin University.(AU). Worksheet 05(tcpserv01.c): Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[2] Curtin University.(AU). Module 05: Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[3] Stevens, W. Richard, Bill Fenner, and Andrew M Rudoff. 2012. UNIX Network Programming(Chapter 30) : serv07. 1st ed. Boston [etc.]: Addison-Wesley.

[4] "Terminating A Thread In C". 2018. Stack Overflow. https://stackoverflow.com/questions/6836894/terminating-a-thread-in-c.

*/

#include	"acc.h"

//Creating thread structure to store thread id of each thread
typedef struct{
	//To store the thread ID
	pthread_t thread_tid;
}Thread;

//Creating array of thread structures that was previously defined
Thread *tptr;

//Shared variables between created threads
int     listenfd, noOfCons, count, client_id; 
socklen_t addrlen;
FILE *cli_list;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

//Function that initiates the threadpool
void initThreadPool(int);

//Function that is handled by the each thread
void *thread_accept(void *);

//Function to terminate threads
void sig_int(int);

//Function that is given to the new threads made when threadpool is busy  
static void * doit(void *);

int main(int argc, char **argv)
{

	int		i, x, connfd;
	socklen_t	naddrlen, nlen;
	pthread_t	tid;
	struct sockaddr	*ncliaddr;
	struct sockaddr_in servaddr;
	cli_details cli_info;
	
	if (argc == 3)
	{
		//noOfCons variable is Needed to store the number of threds that need to be in the thread pool
		noOfCons = atoi(argv[1]);

		count = 0;
		client_id = -1;	
		
		//Checks if the user entered a invalid argument for the number of threads
		if(noOfCons == 0)
		{
			printf("MSS-S > ");
			err_quit("Invalid argument for number of threads");
		}
		
		//Creating IPV4(AF_INET), TCP(SOCK_STREAM) socket
		listenfd = Socket(AF_INET,SOCK_STREAM,0);
		
		//Replacing every byte of server's socket address structure with zero
		bzero(&servaddr, sizeof(servaddr));
	
		//Family of the server's socket address structure = AF_INET(IPV4)			
		servaddr.sin_family = AF_INET;

		//Address of the server = INADDR_ANY(assuming that the server is multihoming device)			
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

		//Port address the server should listen at
		servaddr.sin_port = htons(atoi(argv[2]));

		//Binding the server details to the socket address structure
		Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
		
		//Creating Main directory that contains all mailboxes 
		mkdir("./mailboxes", 0777);
		
		//Creating file to store client list
		cli_list = Fopen("./mailboxes/clientlist.txt", "a+");

		//Switching socket from closed state to listening state.
		Listen(listenfd,LISTENQ);

		//Allocating dynamic memory for the thread strcuture
		tptr = calloc( noOfCons,sizeof(Thread) );

		//Used create the threadpool
		for(i=0; i<noOfCons; i++)
		{
			//Everytime this function is called a new thread will be created
			initThreadPool(i);
		}
	}
	else
	{
		//If number of argument are less or greater than the number of arguments requested by the program
		err_quit("usage: ./mssc.out <number of threads> <port number>");
	}
	
	//Signal to caught when the user press ctrl+c/ when eof charachtor recieved
	Signal(SIGINT, sig_int);

	naddrlen = sizeof(ncliaddr);
	ncliaddr = (struct sockaddr *) Malloc(naddrlen);	
	
	for ( ;; )
	{
		//Creating new threads to handle new clients when whole thread pool is busy
		if(count == noOfCons)
		{
			nlen = naddrlen;

			//Mutex lock makes sure only one thread will call accept at the time and update the connfd
			Pthread_mutex_lock(&mlock);

			if(( connfd = Accept(listenfd, ncliaddr, &nlen)) > 3)
			{
				client_id++;
			}
			
			cli_info.connfd = connfd;
			cli_info.cli_id = client_id;
			cli_info.listen_fd = listenfd;		
	
			Pthread_mutex_unlock(&mlock);
			
			Pthread_create(&tid, NULL, &doit, (void *) &cli_info);
		}		
	}		
	
}

void initThreadPool(int i)
{
	Pthread_create(&tptr[i].thread_tid, NULL, &thread_accept, (void*)i);
	return;
}
void *thread_accept(void *arg)
{
	
	//Will lead threads in thread pool to run independently of parent thread
	Pthread_detach(pthread_self());
	
	int     nconnfd;
        socklen_t clilen;

	//Array of socket structre to store client's adress 
        struct sockaddr *cliaddr;

	//Dynamic memory allocation for sockets structure
        cliaddr = malloc(addrlen);

	printf("MES-S > Starting up thread %d \n",(int)arg);	
	
	for( ;; )
	{
		clilen = addrlen;

		//Mutex lock is used bacause to allow only one thread from pool to call accept() at a time 
		Pthread_mutex_lock(&mlock);

		if( (nconnfd = Accept(listenfd, cliaddr, &clilen)) > 3)
		{
			client_id++;
		}

		//Count is to check if all threads in the thread pool are busy, so that parent thread can accept new connections
		count++;

		Pthread_mutex_unlock(&mlock);
		
		//User commands are handled by this function 
		serv_client(nconnfd,listenfd,client_id,cli_list);
		
		//Mutex lock is used to avoid threads acessing connfd & count variables simultaneously
		Pthread_mutex_lock(&mlock);

		Close(nconnfd);
		count--;

		Pthread_mutex_unlock(&mlock);

		

	}
}

void sig_int(int signo)
{
	int i=0;
	
	printf("\n");

	for(i; i<noOfCons; i++)
	{
		//Terminating threads and checking if they are successfully terminated
		if (pthread_cancel(tptr[i].thread_tid) == 0)
		{
			printf("MES-S > Thread %d is terminated \n", i);
		}
	}

	fclose(cli_list);

	//Removing main folder created to store all the mailbox folders
	system("rm -rf ./mailboxes");
	
	//Terminating the main thread
	exit(0);
}

static void * doit(void *arg)
{
	//For new thread to run independently of parent thread
	Pthread_detach(pthread_self());

	int connfd, clientid, listenfd;

	cli_details *new_info = (cli_details*) arg;

	connfd = (*new_info).connfd;	
	clientid = (*new_info).cli_id;
	listenfd = (*new_info).listen_fd;

	//User commands are handled by this function
	serv_client(connfd, listenfd, clientid, cli_list);

	Close(connfd);

	return(NULL);
}

