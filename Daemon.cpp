#include "Daemon.h"
#include "error.h"
//#include "wrapsignal.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MAXFD   64

int
Daemon()
{
    int     i;
    pid_t   pid;

    if ( (pid = fork()) < 0)
        err_sys("fork error");
    else if (pid)
        _exit(0);           /* parent terminates */

    /* child 1 continues... */

    if (setsid() < 0)           /* become session leader */
        return (-1);

//  Signal(SIGHUP, SIG_IGN);
    if ( (pid = fork()) < 0)
        err_sys("fork error");
    else if (pid)
        _exit(0);           /* child 1 terminates */

    /* child 2 continues... */

    /* close off file descriptors */
    for (i = 0; i < MAXFD; i++)
        close(i);

    /* redirect stdin, stdout, and stderr to /dev/null */
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    return (0);             /* success */
}
