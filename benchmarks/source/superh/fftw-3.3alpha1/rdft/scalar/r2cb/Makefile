#
#	Makefile for fftw-3.3alpha1/rdft/scalar/r2cb on Sunflowersim
#
TREEROOT	= ../../../../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../ -I../../../ -I../../../kernel -I../../../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	codlist.o\
	hb2_16.o\
	hb2_20.o\
	hb2_25.o\
	hb2_32.o\
	hb2_4.o\
	hb2_5.o\
	hb2_8.o\
	hb_10.o\
	hb_12.o\
	hb_15.o\
	hb_16.o\
	hb_2.o\
	hb_20.o\
	hb_25.o\
	hb_3.o\
	hb_32.o\
	hb_4.o\
	hb_5.o\
	hb_6.o\
	hb_64.o\
	hb_7.o\
	hb_8.o\
	hb_9.o\
	hc2cb2_16.o\
	hc2cb2_20.o\
	hc2cb2_32.o\
	hc2cb2_4.o\
	hc2cb2_8.o\
	hc2cb_10.o\
	hc2cb_12.o\
	hc2cb_16.o\
	hc2cb_2.o\
	hc2cb_20.o\
	hc2cb_32.o\
	hc2cb_4.o\
	hc2cb_6.o\
	hc2cb_8.o\
	hc2cbdft2_16.o\
	hc2cbdft2_20.o\
	hc2cbdft2_32.o\
	hc2cbdft2_4.o\
	hc2cbdft2_8.o\
	hc2cbdft_10.o\
	hc2cbdft_12.o\
	hc2cbdft_16.o\
	hc2cbdft_2.o\
	hc2cbdft_20.o\
	hc2cbdft_32.o\
	hc2cbdft_4.o\
	hc2cbdft_6.o\
	hc2cbdft_8.o\
	r2cbIII_10.o\
	r2cbIII_12.o\
	r2cbIII_15.o\
	r2cbIII_16.o\
	r2cbIII_2.o\
	r2cbIII_20.o\
	r2cbIII_25.o\
	r2cbIII_3.o\
	r2cbIII_32.o\
	r2cbIII_4.o\
	r2cbIII_5.o\
	r2cbIII_6.o\
	r2cbIII_64.o\
	r2cbIII_7.o\
	r2cbIII_8.o\
	r2cbIII_9.o\
	r2cb_10.o\
	r2cb_11.o\
	r2cb_12.o\
	r2cb_128.o\
	r2cb_13.o\
	r2cb_14.o\
	r2cb_15.o\
	r2cb_16.o\
	r2cb_20.o\
	r2cb_25.o\
	r2cb_3.o\
	r2cb_32.o\
	r2cb_4.o\
	r2cb_5.o\
	r2cb_6.o\
	r2cb_64.o\
	r2cb_7.o\
	r2cb_8.o\
	r2cb_9.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
