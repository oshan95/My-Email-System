
/*
FILE: writen.c

UNIT: CNCO3002

PURPOSE: To write a particular set of charactors

REFERENCE: 
[1] Curtin University.(AU). Worksheet 05: Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[2] Stevens, W. Richard, Bill Fenner, and Andrew M Rudoff. 1999. UNIX Network Programming. Chapter 3. Sockets Introduction (lib/writen.c). 1st ed. Boston: Addison-Wesley/Prentice-Hall.

COMMENTS: The following functions write data to a particular file descriptor.
*/

/* include writen */
#include	"acc.h"

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}
