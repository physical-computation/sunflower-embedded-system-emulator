#
#	Makefile for fftw-3.3alpha1/ on Sunflowersim
#
TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

LIB		= libfftw

OBJS		=\
		api/*.o\
		dft/*.o\
		dft/scalar/*.o\
		dft/scalar/codelets/*.o\
		kernel/*.o\
		rdft/*.o\
		rdft/scalar/*.o\
		rdft/scalar/r2cb/*.o\
		rdft/scalar/r2cf/*.o\
		rdft/scalar/r2r/*.o\
		reodft/*.o\

DIRS		=\
		api/\
		dft/\
		dft/scalar/\
		dft/scalar/codelets/\
		kernel/\
		rdft/\
		rdft/scalar/\
		rdft/scalar/r2cb/\
		rdft/scalar/r2cf/\
		rdft/scalar/r2r/\
		reodft/\
	
all:
	@echo '==> benchmarks/source/fftw-3.3alpha1/'
	$(MAKE) $(LIB)
	cd libbench2 && make
	cd tests && make

$(LIB):
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			echo '==> benchmarks/source/fftw-3.3alpha1/'$$dir;\
			$(MAKE)\
		); \
	done
	$(AR) r $(LIB)-$(TARGET).a $(OBJS)

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			echo '==> benchmarks/source/fftw-3.3alpha1/'$$dir;\
			$(MAKE) clean\
		); \
	done
	$(RM) $(LIB)-$(TARGET).a
	cd libbench2 && make clean
	cd tests && make clean
