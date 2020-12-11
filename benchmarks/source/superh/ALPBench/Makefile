DIRS =\
	Face_Rec\
	MPGdec\
	MPGenc\
	Ray_Trace\
	Sphinx3\


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
