DIRS =\
	DAM+S-MAC\
	DAM+S-MAC+EAR\
	DAM+Sift\
	DAM+Sift+EAR\
	DAM+idealHWMAC+EAR\
	PARC-damv2\

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
