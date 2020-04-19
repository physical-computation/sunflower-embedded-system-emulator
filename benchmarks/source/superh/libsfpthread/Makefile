TREEROOT	= ../../../..
SIMROOT		= $(TREEROOT)/sim

include $(TREEROOT)/conf/setup.conf

PORT		= ../port
DEFINES		= -DSUPERH -DTHREADID_ADDR=`./mkthreadidaddr` -DDEBUG=0 -DMODEFLAG=SPTHR_LIB_STATIC -DSPTHR_STATICMAP_ALLON0##-DSPTHR_STATICMAP_LOCALTHREADSTATE ##-DSPTHR_STATICMAP_CREATORLOCAL ##-DSPTHR_STATICMAP_ALLON0
INCLDIR		= -I$(SIMROOT) -I$(PORT) -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sys/libOS/port
CFLAGS       	= -c $(INCLDIR) $(DEFINES)
DEBUGFLAGS	= -Wall -Werror
OPTFLAGS	= -O2 $(TARGET-ARCH-FLAGS)
RM		= rm -f


INCLUDES	=\
		arch-superH.h\
		chan.h\
		e-types.h\
		pthread.h\
		spthr_all.h\
		spthr_error.h\
		spthr_extern.h\
		spthr_fns.h\
		spthr_internals.h\
		spthr_staticmtable.h\
		version.h\


OBJS		=\
		pthread_atfork.o\
		pthread_attr_destroy.o\
		pthread_attr_getdetachstate.o\
		pthread_attr_getinheritsched.o\
		pthread_attr_getschedparam.o\
		pthread_attr_getschedpolicy.o\
		pthread_attr_getscope.o\
		pthread_attr_getstackaddr.o\
		pthread_attr_getstacksize.o\
		pthread_attr_init.o\
		pthread_attr_setdetachstate.o\
		pthread_attr_setinheritsched.o\
		pthread_attr_setschedparam.o\
		pthread_attr_setschedpolicy.o\
		pthread_attr_setscope.o\
		pthread_attr_setstackaddr.o\
		pthread_attr_setstacksize.o\
		pthread_cancel.o\
		pthread_cleanup_pop.o\
		pthread_cleanup_push.o\
		pthread_cond_broadcast.o\
		pthread_cond_destroy.o\
		pthread_cond_init.o\
		pthread_cond_signal.o\
		pthread_cond_timedwait.o\
		pthread_cond_wait.o\
		pthread_condattr_destroy.o\
		pthread_condattr_getpshared.o\
		pthread_condattr_init.o\
		pthread_condattr_setpshared.o\
		pthread_create.o\
		pthread_detach.o\
		pthread_equal.o\
		pthread_exit.o\
		pthread_getschedparam.o\
		pthread_getspecific.o\
		pthread_join.o\
		pthread_key_create.o\
		pthread_key_delete.o\
		pthread_kill.o\
		pthread_mutex_destroy.o\
		pthread_mutex_init.o\
		pthread_mutex_lock.o\
		pthread_mutex_trylock.o\
		pthread_mutex_unlock.o\
		pthread_mutexattr_getprioceiling.o\
		pthread_mutexattr_getprotocol.o\
		pthread_mutexattr_getpshared.o\
		pthread_mutexattr_setprioceiling.o\
		pthread_mutexattr_setprotocol.o\
		pthread_mutexattr_setpshared.o\
		pthread_once.o\
		pthread_self.o\
		pthread_setcancelstate.o\
		pthread_setcanceltype.o\
		pthread_setschedparam.o\
		pthread_setspecific.o\
		pthread_sigmask.o\
		pthread_testcancel.o\
		pthread_register.o\
		spthr_internals.o\
		spthr_simcmd.o\
		arch-$(TARGET).o\
		devmtsp.o\
		devrand.o\


all:
	@echo '==> ' $(PWD);\
	cc $(INCLDIR) mkthreadidaddr.c -o mkthreadidaddr;\
	$(MAKE) libsfpthread;\

libsfpthread: $(OBJS)
	$(AR) r libsfpthread.a $(OBJS)


arch-$(TARGET).o: arch-$(TARGET).S $(INCLUDES) Makefile
		$(CPP) $(INCLDIR) $(DEFINES) arch-$(TARGET).S > arch-$(TARGET).i; $(AS) arch-$(TARGET).i -o $@


spthr_internals.o: spthr_internals.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) spthr_internals.c


spthr_simcmd.o: spthr_simcmd.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) spthr_simcmd.c


pthread_atfork.o: pthread_atfork.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_atfork.c


pthread_attr_destroy.o: pthread_attr_destroy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_destroy.c


pthread_attr_getdetachstate.o: pthread_attr_getdetachstate.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getdetachstate.c


pthread_attr_getinheritsched.o: pthread_attr_getinheritsched.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getinheritsched.c


pthread_attr_getschedparam.o: pthread_attr_getschedparam.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getschedparam.c


pthread_attr_getschedpolicy.o: pthread_attr_getschedpolicy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getschedpolicy.c


pthread_attr_getscope.o: pthread_attr_getscope.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getscope.c


pthread_attr_getstackaddr.o: pthread_attr_getstackaddr.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getstackaddr.c


pthread_attr_getstacksize.o: pthread_attr_getstacksize.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_getstacksize.c


pthread_attr_init.o: pthread_attr_init.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_init.c


pthread_attr_setdetachstate.o: pthread_attr_setdetachstate.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setdetachstate.c


pthread_attr_setinheritsched.o: pthread_attr_setinheritsched.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setinheritsched.c


pthread_attr_setschedparam.o: pthread_attr_setschedparam.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setschedparam.c


pthread_attr_setschedpolicy.o: pthread_attr_setschedpolicy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setschedpolicy.c


pthread_attr_setscope.o: pthread_attr_setscope.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setscope.c


pthread_attr_setstackaddr.o: pthread_attr_setstackaddr.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setstackaddr.c


pthread_attr_setstacksize.o: pthread_attr_setstacksize.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_attr_setstacksize.c


pthread_cancel.o: pthread_cancel.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cancel.c


pthread_cleanup_pop.o: pthread_cleanup_pop.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cleanup_pop.c


pthread_cleanup_push.o: pthread_cleanup_push.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cleanup_push.c


pthread_cond_broadcast.o: pthread_cond_broadcast.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_broadcast.c


pthread_cond_destroy.o: pthread_cond_destroy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_destroy.c


pthread_cond_init.o: pthread_cond_init.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_init.c


pthread_cond_signal.o: pthread_cond_signal.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_signal.c


pthread_cond_timedwait.o: pthread_cond_timedwait.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_timedwait.c


pthread_cond_wait.o: pthread_cond_wait.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_cond_wait.c


pthread_condattr_destroy.o: pthread_condattr_destroy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_condattr_destroy.c


pthread_condattr_getpshared.o: pthread_condattr_getpshared.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_condattr_getpshared.c


pthread_condattr_init.o: pthread_condattr_init.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_condattr_init.c


pthread_condattr_setpshared.o: pthread_condattr_setpshared.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_condattr_setpshared.c


pthread_create.o: pthread_create.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_create.c


pthread_detach.o: pthread_detach.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_detach.c


pthread_equal.o: pthread_equal.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_equal.c


pthread_exit.o: pthread_exit.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_exit.c


pthread_getschedparam.o: pthread_getschedparam.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_getschedparam.c


pthread_getspecific.o: pthread_getspecific.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_getspecific.c


pthread_join.o: pthread_join.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_join.c


pthread_key_create.o: pthread_key_create.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_key_create.c


pthread_key_delete.o: pthread_key_delete.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_key_delete.c


pthread_kill.o: pthread_kill.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_kill.c


pthread_mutex_destroy.o: pthread_mutex_destroy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutex_destroy.c


pthread_mutex_init.o: pthread_mutex_init.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutex_init.c


pthread_mutex_lock.o: pthread_mutex_lock.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutex_lock.c


pthread_mutex_trylock.o: pthread_mutex_trylock.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutex_trylock.c


pthread_mutex_unlock.o: pthread_mutex_unlock.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutex_unlock.c


pthread_mutexattr_destroy.o: pthread_mutexattr_destroy.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_destroy.c


pthread_mutexattr_getprioceiling.o: pthread_mutexattr_getprioceiling.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_getprioceiling.c


pthread_mutexattr_getprotocol.o: pthread_mutexattr_getprotocol.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_getprotocol.c


pthread_mutexattr_getpshared.o: pthread_mutexattr_getpshared.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_getpshared.c


pthread_mutexattr_setprioceiling.o: pthread_mutexattr_setprioceiling.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_setprioceiling.c


pthread_mutexattr_setprotocol.o: pthread_mutexattr_setprotocol.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_setprotocol.c


pthread_mutexattr_setpshared.o: pthread_mutexattr_setpshared.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_mutexattr_setpshared.c


pthread_once.o: pthread_once.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_once.c


pthread_self.o: pthread_self.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_self.c


pthread_setcancelstate.o: pthread_setcancelstate.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_setcancelstate.c


pthread_setcanceltype.o: pthread_setcanceltype.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_setcanceltype.c


pthread_setschedparam.o: pthread_setschedparam.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_setschedparam.c


pthread_setspecific.o: pthread_setspecific.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_setspecific.c


pthread_sigmask.o: pthread_sigmask.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_sigmask.c


pthread_testcancel.o: pthread_testcancel.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_testcancel.c


pthread_register.o: pthread_register.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) pthread_register.c


devmtsp.o: $(PORT)/devmtsp.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) $(PORT)/devmtsp.c

devrand.o: $(PORT)/devrand.c $(INCLUDES) Makefile
		$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OPTFLAGS) $(PORT)/devrand.c

clean:
	@echo '==> ' $(PWD)
	$(RM) mkthreadidaddr *.o *.a *.i
