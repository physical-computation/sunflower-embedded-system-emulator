include conf/setup.conf

DIRS =\
	sim\

all: sflr

sflr:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			$(MAKE)\
		); \
	done

cross-arm:
	cd $(TOOLCHAIN); $(MAKE) cross-arm

cross-superH:
	cd $(TOOLCHAIN); $(MAKE) cross-superH

cross-riscv:
	cd $(TOOLCHAIN); $(MAKE) cross-riscv

cross-uncertain-riscv:
	cd $(TOOLCHAIN); $(MAKE) cross-uncertain-riscv

# cross-msp430:
# 	cd $(TOOLCHAIN); $(MAKE) cross-riscv

cross-all:
	cd $(TOOLCHAIN); $(MAKE) cross-all

clean:
	@set -e; for dir in $(DIRS); do \
		(cd $$dir; \
			($(MAKE) clean) \
		); \
	done; \

nuke: clean
	cd $(TOOLCHAIN); $(MAKE) clean
