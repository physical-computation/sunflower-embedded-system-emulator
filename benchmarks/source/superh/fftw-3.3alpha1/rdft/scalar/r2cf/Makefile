#
#	Makefile for fftw-3.3alpha1/rdft/scalar/r2cf on Sunflowersim
#
TREEROOT	= ../../../../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../ -I../../../ -I../../../kernel -I../../../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	codlist.o\
	hc2cf2_16.o\
	hc2cf2_20.o\
	hc2cf2_32.o\
	hc2cf2_4.o\
	hc2cf2_8.o\
	hc2cf_10.o\
	hc2cf_12.o\
	hc2cf_16.o\
	hc2cf_2.o\
	hc2cf_20.o\
	hc2cf_32.o\
	hc2cf_4.o\
	hc2cf_6.o\
	hc2cf_8.o\
	hc2cfdft2_16.o\
	hc2cfdft2_20.o\
	hc2cfdft2_32.o\
	hc2cfdft2_4.o\
	hc2cfdft2_8.o\
	hc2cfdft_10.o\
	hc2cfdft_12.o\
	hc2cfdft_16.o\
	hc2cfdft_2.o\
	hc2cfdft_20.o\
	hc2cfdft_32.o\
	hc2cfdft_4.o\
	hc2cfdft_6.o\
	hc2cfdft_8.o\
	hf2_16.o\
	hf2_20.o\
	hf2_25.o\
	hf2_32.o\
	hf2_4.o\
	hf2_5.o\
	hf2_8.o\
	hf_10.o\
	hf_12.o\
	hf_15.o\
	hf_16.o\
	hf_2.o\
	hf_20.o\
	hf_25.o\
	hf_3.o\
	hf_32.o\
	hf_4.o\
	hf_5.o\
	hf_6.o\
	hf_64.o\
	hf_7.o\
	hf_8.o\
	hf_9.o\
	r2cfII_10.o\
	r2cfII_12.o\
	r2cfII_15.o\
	r2cfII_16.o\
	r2cfII_2.o\
	r2cfII_20.o\
	r2cfII_25.o\
	r2cfII_3.o\
	r2cfII_32.o\
	r2cfII_4.o\
	r2cfII_5.o\
	r2cfII_6.o\
	r2cfII_64.o\
	r2cfII_7.o\
	r2cfII_8.o\
	r2cfII_9.o\
	r2cf_10.o\
	r2cf_11.o\
	r2cf_12.o\
	r2cf_128.o\
	r2cf_13.o\
	r2cf_14.o\
	r2cf_15.o\
	r2cf_16.o\
	r2cf_2.o\
	r2cf_20.o\
	r2cf_25.o\
	r2cf_3.o\
	r2cf_32.o\
	r2cf_4.o\
	r2cf_5.o\
	r2cf_6.o\
	r2cf_64.o\
	r2cf_7.o\
	r2cf_8.o\
	r2cf_9.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
