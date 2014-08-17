DIRS =\
	bubblesort\
	mpeg2dec\
	mpegencoder\
	sphinx3/src/libfeat\
	sphinx3/src/libio\
	sphinx3/src/libutil\
	sphinx3/src/libfbs\
	swradio\
	sbench\

#	swradio-pthread\
#	libsfpthread\
#	ALPBench\
#	MiBench\


all:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE)\
		); \
	done

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			($(MAKE) clean) \
		); \
	done; \
