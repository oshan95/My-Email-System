/*
FILE: acc.h

UNIT: CNCO3002

PURPOSE: Header file that provides all the necessary libraries that need for program to run without errors 

REFERENCE: 
[1] Curtin University.(AU). Worksheet 05(acc.h): Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <dirent.h>


#ifdef	HAVE_PTHREAD_H
#include	<pthread.h>
#endif


#define	MAXLINE	4096
#define	LISTENQ	1024
#define	OK	52001
#define	SA	struct sockaddr


#ifndef		__unp_pthread_h
#define		__unp_pthread_h

typedef	void	Sigfunc(int);

//Structure to pass multiple data to the thread
typedef struct{
	int connfd;
	int cli_id;
	int listen_fd;
}cli_details;


void	Pthread_create(pthread_t *, const pthread_attr_t *, void * (*)(void *), void *);
   /*void * (*)(void *), void *);*/
void	Pthread_join(pthread_t, void **);
void	Pthread_detach(pthread_t);
void	Pthread_kill(pthread_t, int);

void	Pthread_mutexattr_init(pthread_mutexattr_t *);
void	Pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
void	Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
void	Pthread_mutex_lock(pthread_mutex_t *);
void	Pthread_mutex_unlock(pthread_mutex_t *);

void	Pthread_cond_broadcast(pthread_cond_t *);
void	Pthread_cond_signal(pthread_cond_t *);
void	Pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void	Pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
							   const struct timespec *);

void	Pthread_key_create(pthread_key_t *, void (*)(void *));
void	Pthread_setspecific(pthread_key_t, const void *);
void	Pthread_once(pthread_once_t *, void (*)(void));

#endif	/* __unp_pthread_h */
