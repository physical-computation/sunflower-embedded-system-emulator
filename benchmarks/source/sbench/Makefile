DIRS =\
	DAM+S-MAC\
	DAM+S-MAC+EAR\
	DAM+Sift\
	DAM+Sift+EAR\
	DAM+idealHWMAC+EAR\
	EAR-route\
	PARC-damv2\
	S-MAC\
	Sift-MAC\

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
