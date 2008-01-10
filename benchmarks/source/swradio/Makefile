DIRS =\
	swradio-lpf\
	swradio-demod\
	swradio-eq\
	swradio-source\
	swradio-sink\


all:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE)\
		); \
	done

install:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE) install\
		); \
	done

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			($(MAKE) clean) \
		); \
	done; \
	rm -f *.sr

nuke: clean
	rm -rf simlog*
