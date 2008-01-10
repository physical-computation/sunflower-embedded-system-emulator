INPUTFILES = rtvi_iface.c rtvi_ctrl.c
OBJS = rtvi_iface.o rtvi_ctrl.o
RTVI_HOME=/home/staff/cdtaylor/synergy/REL0.0
LIBRTVI = librtvictrl.appc

CC = ccmc -t ppc -x
AR = armc
ARFLAGS = -r

PPCFLAGS = -w0 -Ot -to ppc:c:-O3,-fomit-frame-pointer,-ffast-math -DVOLATILE="volatile" -DANSII_MODE -I$(RTVI_HOME)/etvi_include/

all:
	@echo "**                                       "
	@echo "** Building PowerPC RTVI Control Code... "
	@echo "**                                       "
	$(MAKE)	librtvictrl.appc
	cp $(RTVI_HOME)/etvi_archives/etvi_lib_mcos.appc libetvi_lib_mcos.appc

$(LIBRTVI) : $(INPUTFILES) $(OBJS)
	$(AR) $(ARFLAGS) $(LIBRTVI) $(OBJS)

rtvi_iface.o : rtvi_iface.c rtvi_iface.h
	$(CC) $(PPCFLAGS) rtvi_iface.c -c

rtvi_ctrl.o : rtvi_ctrl.c rtvi_ctrl.h
	$(CC) $(PPCFLAGS) rtvi_ctrl.c -c

clean :
	rm -f $(OBJS) $(LIBRTVI) libetvi_lib_mcos.appc


