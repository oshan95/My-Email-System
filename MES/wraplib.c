
/*
FILE: wraplib.c

UNIT: CNCO3002

PURPOSE: Network address conversion

REFERENCE: 
Curtin University.(AU). Worksheet 05: Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

COMMENTS: Following functions convert IPv4 and IPv6 addresses from binary to text form, and vice versa.
*/

/*
 * Wrapper functions for our own library functions.
 * Most are included in the source file for the function itself.
 */

#include	"acc.h"

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const char	*ptr;

	if (strptr == NULL)		/* check for old code */
		err_quit("NULL 3rd argument to inet_ntop");
	if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
		err_sys("inet_ntop error");		/* sets errno */
	return(ptr);
}

void
Inet_pton(int family, const char *strptr, void *addrptr)
{
	int		n;

	if ( (n = inet_pton(family, strptr, addrptr)) < 0)
		err_sys("inet_pton error for %s", strptr);	/* errno set */
	else if (n == 0)
		err_quit("inet_pton error for %s", strptr);	/* errno not set */

	/* nothing to return */
}
