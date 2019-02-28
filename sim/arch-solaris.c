/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sf.h"
#include "mextern.h"

static char Ebadsfout[] = "Invalid Sunflower output destination";
pthread_t sched_handle;

int mfsize(int fd) {
  struct stat sb;

  if (fstat(fd, &sb) < 0) {
    return -1;
  }

  return sb.st_size;
}

void mstatelock(void) {}

void mstateunlock(void) {}

void marchinit(void) {}

char *mfgets(char *buf, int len, int fd) {
  int n, i = 0;
  char ch;

  if (len <= 0) {
    return NULL;
  }

  do {
    n = mread(fd, &ch, 1);
    if (n == 0) {
      if (i == 0)
        return NULL;
      else
        break;
    }

    buf[i++] = ch;
  } while ((i < len) && (ch != '\n'));
  buf[i] = '\0';

  return buf;
}

void mexit(Engine *E, char *str, int status) {
  printf("\n\n\tExiting: %s\n", str);
  printf("\tWriting all node information to sunflower.out\n\n");
  m_dumpall(E, E->logfilename, M_OWRITE, "Exit", "");

  exit(status);
}

void mprintfd(int fd, char *buf) {
  write(fd, buf, strlen(buf));

  return;
}

int mcreate(char *path, int mode) {
  int perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
  int flags = 0;
  int rw = mode & (~M_OTRUNCATE);
  int fd;

  if (rw == M_OREAD)
    flags = O_RDONLY;

  if (rw == M_OWRITE)
    flags = O_WRONLY;

  if (rw == (M_OREAD | M_OWRITE))
    flags = O_RDWR;

  if (mode & M_OTRUNCATE)
    flags |= O_TRUNC;
  else
    flags |= O_APPEND;

  fd = open(path, flags | O_CREAT, perm);
  return fd;
}

int mopen(char *path, int mode) {
  int perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
  int flags = 0;
  int rw = mode & (~M_OTRUNCATE);

  if (rw == M_OREAD)
    flags = O_RDONLY;

  if (rw == M_OWRITE)
    flags = O_WRONLY;

  if (rw == (M_OREAD | M_OWRITE))
    flags = O_RDWR;

  if (mode & M_OTRUNCATE)
    flags |= O_TRUNC;
  else
    flags |= O_APPEND;

  return open(path, flags, perm);
}

int mread(int fd, char *buf, int len) { return read(fd, buf, len); }

int mclose(int fd) { return close(fd); }

int mwrite(int fd, char *buf, int len) { return write(fd, buf, len); }

int mspawnscheduler(Engine *E) {
  int r = pthread_create(&E->sched_handle, NULL, (void *)scheduler, E);

  if (r) {
    return -1;
  }

  return 0;
}

int mkillscheduler(Engine *E) {
  if (E->nodetach || !E->on) {
    return 0;
  }

  /*	Scheduler will exit cleanly when E->on is cleared	*/
  E->on = 0;

  return 0;
}

int mchdir(char *path) { return chdir(path); }

char *mgetpwd(void) { return getwd(NULL); }

ulong musercputimeusecs(void) {
  struct rusage r;

  getrusage(RUSAGE_SELF, &r);

  return (ulong)(r.ru_utime.tv_sec * 1E6 + r.ru_utime.tv_usec);
}

ulong mcputimeusecs(void) {
  struct rusage r;

  getrusage(RUSAGE_SELF, &r);

  return (ulong)(r.ru_utime.tv_sec * 1E6 + r.ru_utime.tv_usec +
                 r.ru_stime.tv_sec * 1E6 + r.ru_stime.tv_usec);
}

ulong mwallclockusecs(void) {
  struct timeval t;
  gettimeofday(&t, NULL);

  return t.tv_usec;
}

void mlog(Engine *E, State *S, char *fmt, ...) {
  char *buf;
  va_list arg;

  if (!SF_SIMLOG) {
    return;
  }

  /*	Should never happen, but just in case code changes elsewhere:	*/
  if (S->logfd < 0) {
    return;
  }

  buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-solaris.c");
  if (buf == NULL) {
    mexit(E, "Could not allocate memory for (char *)buf in arch-linux.c", -1);
  }

  va_start(arg, fmt);
  vsnprintf(buf, MAX_MIO_BUFSZ, fmt, arg);
  va_end(arg);

  write(S->logfd, buf, strlen(buf));
  mfree(E, buf, "(char *)buf in arch-solaris.c");

  return;
}

int msnprint(char *dst, int size, char *fmt, ...) {
  va_list arg;
  int n;

  va_start(arg, fmt);
  n = vsnprintf(dst, size, fmt, arg);
  va_end(arg);

  return n;
}

void mprint(Engine *E, State *S, int out, char *fmt, ...) {
  int fmtlen;
  char *buf;
  va_list arg;

  if (!E->verbose && S != NULL) {
    return;
  }

  buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-solaris.c");
  if (buf == NULL) {
    mexit(E, "Could not allocate memory for (char *)buf in arch-solaris.c", -1);
  }

  va_start(arg, fmt);
  fmtlen = vsnprintf(buf, MAX_MIO_BUFSZ, fmt, arg);
  va_end(arg);

  if (fmtlen < 0) {
    fprintf(stderr, "vsnprintf() in mprint() failed.\n");
    mfree(E, buf, "(char *)buf in arch-solaris.c");

    return;
  }

  /*	If no state structure is given, send to global output	*/
  if (S == NULL) {
    if (out != siminfo) {
      fprintf(stderr, "%s\n", Ebadsfout);
      mfree(E, buf, "(char *)buf in arch-solaris.c");

      return;
    }

    fprintf(stdout, "%s", buf);
    mfree(E, buf, "(char *)buf in arch-solaris.c");

    return;
  }

  if (out == nodestdout || out == nodestderr || out == nodeinfo) {
    if (S->NODE_ID == E->cp->NODE_ID) {
      fprintf(stdout, "%s", buf);
    }
  } else {
    fprintf(stderr, "%s\n", Ebadsfout);
  }

  mfree(E, buf, "(char *)buf in arch-solaris.c");

  return;
}

void mnsleep(ulong nsecs) {
  /*								*/
  /*	Inferno doesn't provide us with enough granularity.	*/
  /*	This should still be fairly portable since nanosleep	*/
  /*	is POSIX.1b.						*/
  /*								*/
  struct timespec rqtp;

  rqtp.tv_sec = nsecs / 1000000000;
  rqtp.tv_nsec = nsecs % 1000000000;

  nanosleep(&rqtp, nil);
}

void merror(char *fmt, ...) {
  int fmtlen;
  char *buf;
  va_list arg;

  buf = mcalloc(E, 1, MAX_MIO_BUFSZ, "(char *)buf in arch-solaris.c");
  if (buf == NULL) {
    mexit(E, "Could not allocate memory for (char *)buf in arch-solaris.c", -1);
  }

  va_start(arg, fmt);
  fmtlen = vsnprintf(buf, MAX_MIO_BUFSZ, fmt, arg);
  va_end(arg);

  if (fmtlen < 0) {
    fprintf(stderr, "mprint failed.\n");
    mfree(E, buf, "(char *)buf in arch-solaris.c");

    return;
  }

  mprint(E, NULL, siminfo, "Error: %s\n", buf);
  mfree(E, buf, "(char *)buf in arch-solaris.c");

  return;
}
