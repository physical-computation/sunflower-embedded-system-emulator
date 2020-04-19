#
# Copyright (c) 1995 Carnegie Mellon University.  ALL RIGHTS RESERVED.
# 
# HISTORY
# 
# 23-Aug-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University.
# 		Created.
#

# Before building:
# setenv MACHINE alpha {or sun4, hp700_ux, linux, etc. as appropriate}
# 
# Also, create the following directories if not already existing:
#     lib/$(MACHINE),
#     bin/$(MACHINE),
#     src/libutil/$(MACHINE),
#     src/libio/$(MACHINE),
#     src/libfeat/$(MACHINE),
#     src/libfbs/$(MACHINE),

s3decode:
	(cd src/libutil/$(MACHINE); make -f ../Makefile install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile install)

s3decode-prof:
	(cd src/libutil/$(MACHINE); make -f ../Makefile-prof install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile-prof install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile-prof install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-prof install)
s3astar: 
	(cd src/libutil/$(MACHINE); make -f ../Makefile install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-astar install)

s3decode-dbg:
	(cd src/libutil/$(MACHINE)-debug; make -f ../Makefile-debug install)
	(cd src/libio/$(MACHINE)-debug;   make -f ../Makefile-debug install)
	(cd src/libfeat/$(MACHINE)-debug; make -f ../Makefile-debug install)
	(cd src/libfbs/$(MACHINE)-debug;  make -f ../Makefile-debug install)

s3decode-anytopo:
	(cd src/libutil/$(MACHINE); make -f ../Makefile install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-anytopo install)

s3decode-anytopodbg:
	(cd src/libutil/$(MACHINE); make -f ../Makefile-debug install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile-debug install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile-debug install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-anytopodbg install)

s3align:
	(cd src/libutil/$(MACHINE); make -f ../Makefile install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-align install)

s3allphone:
	(cd src/libutil/$(MACHINE); make -f ../Makefile install)
	(cd src/libio/$(MACHINE);   make -f ../Makefile install)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile install)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-allphone install)

only-s3decode:
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile install)

only-s3align:
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-align install)

only-s3allphone:
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile-allphone install)

clean:
	(cd src/libutil/$(MACHINE); make -f ../Makefile clean)
	(cd src/libio/$(MACHINE);   make -f ../Makefile clean)
	(cd src/libfeat/$(MACHINE); make -f ../Makefile clean)
	(cd src/libfbs/$(MACHINE);  make -f ../Makefile clean)
	(cd lib/$(MACHINE); rm -f *.*)
	(cd bin/$(MACHINE); rm -f *.*)
