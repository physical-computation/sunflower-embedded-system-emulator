include 	../../../conf/setup.conf

PORT		= $(SUNFLOWERROOT)/sys/libOS/port
DEV		= $(SUNFLOWERROOT)/sys/dev

INCLUDEDIR      = $(SUNFLOWERROOT)/sys/include
LIB 	     	= $(SUNFLOWERROOT)/tools-lib/$(TARGET)
INCLUDES        = -I. -I$(INCLUDEDIR) -I$(PORT) -I$(GCCINCLUDEDIR) -I../../kern/port
CCFLAGS       	= -gstabs3 -O2 $(TARGET-ARCH-FLAGS) -nostdinc $(INCLUDES) -fno-builtin -Wall -Werror -c

DELETE          = rm -f 
COPY            = cp


OBJS     	=\
		atexit.o\
		chmod.o\
		lstat.o\
		getcwd.o\
		getrusage.o\
		gettimeofday.o\
		time.o\
		times.o\
		system.o\
		dup2.o\
		pipe.o\
		execvp.o\
		access.o\
		mkdir.o\
		memccpy.o\
		getpwuid.o\
		getgrgid.o\
		chdir.o\
		fcntl.o\
		popen.o\
		pclose.o\
		opendir.o\
		readdir.o\
		closedir.o\
		fsync.o\
		random.o\

##		getenv.o\
##		fgetc.o\
##		rand.o\
##		srand.o\
##		checkinput.o\
##		abort.o\
##		atof.o\
##		atoi.o\
##		calloc.o\
##		ceil.o\
##		clock.o\
##		close.o\
##		ctime.o\
##		ctype.o\
##		environ.o\
##		eprintf.o\
##		errno.o\
##		exit.o\
##		exp.o\
##		fclose.o\
##		fflush.o\
##		fgets.o\
##		fileno.o\
##		floor.o\
##		fopen.o\
##		fprintf.o\
##		fputc.o\
##		fputc.o\
##		fputs.o\
##		free.o\
##		fscanf.o\
##		fseek.o\
##		fstat.o\
##		ftell.o\
##		getcwd.o\
##		getenv.o\
##		getrusage.o\
##		gets.o\
##		impure_ptr.o\
##		isatty.o\
##		ldexp.o\
##		localtime.o\
##		log10.o\
##		longjmp.o\
##		lseek.o\
##		malloc.o\
##		memcmp.o\
##		memset.o\
##		open.o\
##		perror.o\
##		pow.o\
##		printf.o\
##		qsort.o\
##		read.o\
##		realloc.o\
##		remove.o\
##		rewind.o\
##		sbrk.o\
##		scanf.o\
##		setjmp.o\
##		signal.o\
##		sprintf.o\
##		sqrt.o\
##		srget.o\
##		sscanf.o\
##		stat.o\
##		strchr.o\
##		strcspn.o\
##		strerror.o\
##		strrchr.o\
##		strspn.o\
##		strstr.o\
##		swbuf.o\
##		system.o\
##		ungetc.o\
##		unlink.o\
##		vfprintf.o\

PORT_OBJS	=\
##		memcpy.o\
##		memmove.o\
##		print.o\
##		strncat.o\
##		strncmp.o\
##		strncpy.o\
##		strcat.o\
##		strtok.o\
##		strlen.o\
##		strcpy.o\
##		strcmp.o\


all:	## Make sure you have edited setup.conf, and set eEK64/eEK32/eEK16 etc.
	@echo '==> sys/libOS/mOS';\
	$(MAKE) libmOS;\

libmOS: semiclean $(OBJS) $(PORT_OBJS)
	$(AR) r libmOS-$(TARGET).a $(OBJS) $(PORT_OBJS)

osinit.o: osinit.c version.h 
	$(CC) $(CCFLAGS) osinit.c

atoi.o:
	$(CC) $(CCFLAGS) atoi.c

chmod.o:
	$(CC) $(CCFLAGS) chmod.c

close.o:
	$(CC) $(CCFLAGS) close.c

ctime.o:
	$(CC) $(CCFLAGS) ctime.c

ctype.o:
	$(CC) $(CCFLAGS) ctype.c

errno.o:
	$(CC) $(CCFLAGS) errno.c

exit.o:
	$(CC) $(CCFLAGS) exit.c

atexit.o:
	$(CC) $(CCFLAGS) atexit.c

fflush.o:
	$(CC) $(CCFLAGS) fflush.c

fgets.o:
	$(CC) $(CCFLAGS) fgets.c

fileno.o:
	$(CC) $(CCFLAGS) fileno.c

fprintf.o:
	$(CC) $(CCFLAGS) fprintf.c

printf.o:
	$(CC) $(CCFLAGS) printf.c

free.o:
	$(CC) $(CCFLAGS) free.c

fstat.o:
	$(CC) $(CCFLAGS) fstat.c

getenv.o:
	$(CC) $(CCFLAGS) getenv.c

impure_ptr.o:
	$(CC) $(CCFLAGS) impure_ptr.c

isatty.o:
	$(CC) $(CCFLAGS) isatty.c

lseek.o:
	$(CC) $(CCFLAGS) lseek.c

lstat.o:
	$(CC) $(CCFLAGS) lstat.c

malloc.o:
	$(CC) $(CCFLAGS) malloc.c

calloc.o:
	$(CC) $(CCFLAGS) calloc.c

memcmp.o:
	$(CC) $(CCFLAGS) memcmp.c

memset.o:
	$(CC) $(CCFLAGS) memset.c

open.o:
	$(CC) $(CCFLAGS) open.c

perror.o:
	$(CC) $(CCFLAGS) perror.c

rand.o:
	$(CC) $(CCFLAGS) rand.c

random.o:
	$(CC) $(CCFLAGS) random.c

read.o:
	$(CC) $(CCFLAGS) read.c

stat.o:
	$(CC) $(CCFLAGS) stat.c

strcspn.o:
	$(CC) $(CCFLAGS) strcspn.c

strerror.o:
	$(CC) $(CCFLAGS) strerror.c

strrchr.o:
	$(CC) $(CCFLAGS) strrchr.c

strspn.o:
	$(CC) $(CCFLAGS) strspn.c

swbuf.o:
	$(CC) $(CCFLAGS) swbuf.c

unlink.o:
	$(CC) $(CCFLAGS) unlink.c

dup2.o:
	$(CC) $(CCFLAGS) dup2.c

pipe.o:
	$(CC) $(CCFLAGS) pipe.c

execvp.o:
	$(CC) $(CCFLAGS) execvp.c

access.o:
	$(CC) $(CCFLAGS) access.c

mkdir.o:
	$(CC) $(CCFLAGS) mkdir.c

memccpy.o:
	$(CC) $(CCFLAGS) memccpy.c

getpwuid.o:
	$(CC) $(CCFLAGS) getpwuid.c

getgrgid.o:
	$(CC) $(CCFLAGS) getgrgid.c

chdir.o:
	$(CC) $(CCFLAGS) chdir.c

fcntl.o:
	$(CC) $(CCFLAGS) fcntl.c

popen.o:
	$(CC) $(CCFLAGS) popen.c

pclose.o:
	$(CC) $(CCFLAGS) pclose.c

opendir.o:
	$(CC) $(CCFLAGS) opendir.c

readdir.o:
	$(CC) $(CCFLAGS) readdir.c

closedir.o:
	$(CC) $(CCFLAGS) closedir.c

fsync.o:
	$(CC) $(CCFLAGS) fsync.c

version.h:
	echo 'char const VERSION[] = "mOS-$(TARGET-ARCH)-'`date '+%m-%d-%Y-%H:%M:%S'` \(OS interface for Applications Running Over Myrmigki Simulator\)\"\; > version.h

include $(PORT)/Makefile.in

## This is to make sure when we build a lib, we recompile objs, 
## as they be for different architectures (Ah, Plan9 !)
semiclean:
	@set -e; echo 'Making Semi-Clean in' $(PWD)
	$(DELETE) *.o
	$(DELETE) version.h 

clean:
	@echo '==> sys/libOS/mOS '
	$(DELETE) *.o *.a version.h
