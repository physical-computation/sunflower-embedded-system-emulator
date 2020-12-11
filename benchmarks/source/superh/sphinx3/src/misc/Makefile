#
# Makefile
# 
# HISTORY
# 
# 23-Dec-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
# 		Created.
#


include ../../Makefile.defines

VPATH = .:..

AUXOBJ = ../../main/$(MACHINE)/mdef.o ../../main/$(MACHINE)/dict.o

dpalign: dag.o dp.o dpalign.o line2wid.o
	$(CC) $(CFLAGS) -o dpalign dpalign.o dp.o dag.o line2wid.o $(AUXOBJ) -lutil

allpalign: allpalign.o dag.o dp.o
	$(CC) $(CFLAGS) -o allpalign allpalign.o dp.o dag.o $(AUXOBJ) -lutil

pronerr: pronerr.o line2wid.o
	$(CC) $(CFLAGS) -o pronerr pronerr.o line2wid.o $(AUXOBJ) -lutil

pronerralign: pronerralign.o dp.o dag.o
	$(CC) $(CFLAGS) -o pronerralign pronerralign.o dp.o dag.o $(AUXOBJ) -lutil

v8seg2ascii: v8seg2ascii.o
	$(CC) $(CFLAGS) -o v8seg2ascii v8seg2ascii.o $(AUXOBJ) -lio -lutil

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#
