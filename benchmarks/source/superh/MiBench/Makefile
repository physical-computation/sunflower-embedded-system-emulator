DIRS =\
	automotive/basicmath\
	automotive/bitcount\
	automotive/qsort\
	automotive/susan\
	consumer/jpeg/jpeg-6a\
	consumer/lame/lame3.70\
	network/dijkstra\
	office/ghostscript/src\
	office/stringsearch\
	security/blowfish\
	security/rijndael\
	security/sha\
	telecomm/CRC32\
	telecomm/FFT\

#	consumer/mad\
#	consumer/tiff-data\
#	consumer/tiff-v3.5.4\
#	consumer/tiff2bw\
#	consumer/tiff2rgba\
#	consumer/tiffdither\
#	consumer/tiffmedian\
#	consumer/typeset/lout-3.24\
#	office/ispell\
#	office/rsynth\
#	office/sphinx\
#	security/pgp\
#	telecomm/adpcm\
#	network/patricia\
#	telecomm/gsm\

all:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE)\
		); \
	done

edit:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			vi Makefile */Makefile */*/Makefile */*/*/Makefile\
		); \
	done

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE) clean\
		); \
	done

nuke:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE) nuke\
		); \
	done
