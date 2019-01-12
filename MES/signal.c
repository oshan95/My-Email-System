/*
FILE: signal.c

UNIT: CNCO3002

PURPOSE: To catch and handle signals

REFERENCE: 
[1] Curtin University.(AU). Worksheet 04: I/O Models. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[2] Stevens, W. Richard, Bill Fenner, and Andrew M Rudoff. 1999. UNIX Network Programming. Chapter 5. TCP Client/Server Example(lib/signal.c). 1st ed. Boston: Addison-Wesley/Prentice-Hall.

COMMENTS: The following functions handles errors retruned by system calls
*/

/* include signal */
#include	"acc.h"
#include	<signal.h>

Sigfunc *
signal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
/* end signal */

Sigfunc *
Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
		err_sys("signal error");
	return(sigfunc);
}
