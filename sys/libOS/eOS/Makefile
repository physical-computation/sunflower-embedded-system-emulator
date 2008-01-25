include 	../../../conf/setup.conf

PORT		= $(SUNFLOWERROOT)/sys/libOS/port
DEV		= $(SUNFLOWERROOT)/sys/dev

INCLUDEDIR      = $(SUNFLOWERROOT)/sys/include
LIB 	     	= $(SUNFLOWERROOT)/tools-lib/$(TARGET)
INCLUDES        = -I. -I$(INCLUDEDIR) -I$(PORT) -I$(GCCINCLUDEDIR) -I../../kern/port
CCFLAGS       	= -g3 -O2 $(TARGET-ARCH-FLAGS) -nostdinc $(INCLUDES) -fno-builtin -Wall -Werror -c

DELETE          = rm -f 
COPY            = cp


OBJS     	=\
		#osinit.o\
		#proc.o\
		#mem.o\
		#sleep.o\
		#wakeup.o\

PORT_OBJS	=\
		lock.o\
		memcpy.o\
		memmove.o\
		print.o\
		rand.o\
		read.o\
		strncat.o\
		strncmp.o\
		strncpy.o\
		strcat.o\
		strtok.o\
		strlen.o\
		strcpy.o\
		strcmp.o\
		twofish.o\

DEV_OBJS	=\
		devether.o\
		devarp.o\
		devipv4.o\
		devipv6.o\
		devicmp.o\

all: ## Make sure you have edited setup.conf, and set eEK64/eEK32/eEK16 etc.
	@echo '==> sys/libOS/eOS';\
	$(MAKE) libeOS;\

libeOS: semiclean $(OBJS) $(PORT_OBJS) 	##XXX$(DEV_OBJS)
	$(AR) r libeOS-$(TARGET).a $(OBJS) $(PORT_OBJS) ##XXX$(DEV_OBJS)


osinit.o: osinit.c version.h 
	$(CC) $(CCFLAGS) osinit.c

version.h:
	echo 'char const VERSION[] = "eOS-$(TARGET-ARCH)-'`date '+%m-%d-%Y-%H:%M:%S'` \(MARIA\)\"\; > version.h

proc.o: proc.c
	$(CC) $(CCFLAGS) proc.c

mem.o: mem.c
	$(CC) $(CCFLAGS) mem.c

sleep.o: sleep.c
	$(CC) $(CCFLAGS) sleep.c

wakeup.o: wakeup.c
	$(CC) $(CCFLAGS) wakeup.c


include $(PORT)/Makefile.in
#include $(DEV)/Makefile.in

## This is to make sure when we build a lib, we recompile objs
semiclean:
	@set -e; echo 'Making Semi-Clean in' $(PWD)
	$(DELETE) *.o
	$(DELETE) version.h 

clean:
	@echo '==> sys/libOS/eOS '
	$(DELETE) *.o *.a version.h
