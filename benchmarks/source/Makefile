DIRS =\
	ALPBench\
	MiBench\
	bubblesort\
	mpeg2dec\
	mpegencoder\
	sbench\
	sphinx3/src/libfbs\
	sunflower-pthread\
	swradio\
	swradio-pthread\


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
