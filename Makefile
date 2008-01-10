include ./conf/setup.conf

Z		= $(PATH):$(SUNFLOWERROOT)/tools/bin
CUR		= sunflower-1.0-beta#`echo $(PWD) | awk -F'/' '{print $NF}'`

RELEASE		= sunflower-1.0-release
MINOR		= 2
DISTRIBUTION	= $(RELEASE)-source-beta.$(MINOR)

DIRS =\
	sim\


all: sflr

sflr:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE)\
		); \
	done

cross:
	cd $(TOOLS); $(MAKE) HOST=$(HOST) PATH=$(Z)\
	TARGET=$(TARGET) TARGET-ARCH=$(TARGET-ARCH) all;\

cross-all:
	cd $(TOOLS); $(MAKE) HOST=$(HOST) PATH=$(Z)\
	TARGET=superH TARGET-ARCH=sh-coff all;\
	cd $(TOOLS); $(MAKE) HOST=$(HOST) PATH=$(Z)\
	TARGET=msp430 TARGET-ARCH=msp430 all;\

MINDIST	=\
	$(SUNFLOWERROOT)/benchmarks/README\
	$(SUNFLOWERROOT)/benchmarks/dist\

min-dist:
	@set -e;
		mkdir -p $(DISTRIBUTION)/benchmarks;
			for k in $(MINDIST); do \
				cp -r $$k $(DISTRIBUTION)/benchmarks;\
			done
		tar hczvf $(DISTRIBUTION).tgz $(DISTRIBUTION)
		md5 $(DISTRIBUTION).tgz > $(DISTRIBUTION).md5

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			($(MAKE) clean) \
		); \
	done; \

nuke: clean
	cd $(TOOLS); $(MAKE) nuke; \
	for dir in $(SUPPORTED-TARGETS); do ($(DEL) $(SUNFLOWERROOT)/tools-lib/*/*.a); done
