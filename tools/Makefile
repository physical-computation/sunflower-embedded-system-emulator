include ../conf/setup.conf

GCC		= $(TOOLS)/source/gcc-4.2.4
BINUTILS	= $(TOOLS)/source/binutils-2.16.1
NEWLIB		= $(TOOLS)/source/newlib-1.9.0



all: binutils gcc newlib


binutils: binutils-pre binutils-post

binutils-pre:
	if test -d $(TOOLS)/bin; then true; else mkdir $(TOOLS)/bin; fi;\
	if test -d $(TOOLS)/$(TARGET); then true; else mkdir $(TOOLS)/$(TARGET); fi;\
	cd $(BINUTILS);\
	$(DEL) objdir; mkdir -p objdir; cd objdir;\
	export MAKEINFO=missing && ../configure --disable-docs --target=$(TARGET-ARCH) --host=$(HOST)\
		--prefix=$(PREFIX) --disable-libssp --disable-nls -v;\
	$(MAKE) -r CC=$(TOOLCC) CFLAGS="-D_FORTIFY_SOURCE=1" LD=$(TOOLCC) all install;\

binutils-post:
	mv $(PREFIX)/bin/$(TARGET-ARCH)* $(TOOLS)/bin/;\
	$(DEL) $(BINUTILS)/objdir;\




gcc: gcc-pre gcc-post

g++-pre:
	if test -d $(TOOLS)/bin; then true; else mkdir $(TOOLS)/bin; fi;\
	if test -d $(TOOLS)/$(TARGET); then true; else mkdir $(TOOLS)/$(TARGET); fi;\
	cd $(GCC);\
	$(DEL) objdir; mkdir -p objdir; cd objdir;\
	export CFLAGS='-O2' CXXFLAGS='-O2' MAKEINFO=missing && ../configure --disable-docs --target=$(TARGET-ARCH) --host=$(HOST) --prefix=$(PREFIX)\
		--disable-libssp --with-gnu-as --with-gnu-ld --with-newlib\
		--enable-languages="c,c++"\
		--with-headers=$(NEWLIB)/newlib/libc/include -v;\
	$(MAKE) CC=$(TOOLCC) CFLAGS="-fgnu89-inline -ansi";\ 	# CXX=$(TOOLCXX) CFLAGS="-ansi";\ 
	$(MAKE) CC=$(TOOLCC) install;\				# CXX=$(TOOLCXX) install;\

gcc-pre:
	if test -d $(TOOLS)/bin; then true; else mkdir $(TOOLS)/bin; fi;\
	if test -d $(TOOLS)/$(TARGET); then true; else mkdir $(TOOLS)/$(TARGET); fi;\
	cd $(GCC);\
	$(DEL) objdir; mkdir -p objdir; cd objdir;\
	export CFLAGS='-O2 -D_XOPEN_SOURCE=600' CXXFLAGS='-O2' MAKEINFO=missing && ../configure --disable-docs --target=$(TARGET-ARCH) --host=$(HOST) --prefix=$(PREFIX)\
		--disable-libssp --with-gnu-as --with-gnu-ld --with-newlib\
		--enable-languages=c\
		--with-headers=$(NEWLIB)/newlib/libc/include -v;\
	$(MAKE) CC=$(TOOLCC) CFLAGS="-fgnu89-inline -ansi -D_XOPEN_SOURCE=600" all-gcc;\	# CXX=$(TOOLCXX) CFLAGS="-ansi";\ 
	$(MAKE) CC=$(TOOLCC) install;\					# CXX=$(TOOLCXX) AR=ar install;\


gcc-post:
	cd $(GCC);\
	cp $(PREFIX)/lib/gcc-lib/$(TARGET-ARCH)/4.2.4/*.a\
		$(SUNFLOWERROOT)/tools/tools-lib/$(TARGET)/;\
	cp $(PREFIX)/lib/gcc/$(TARGET-ARCH)/4.2.4/*.a\
		$(SUNFLOWERROOT)/tools/tools-lib/$(TARGET)/;\
	cp $(PREFIX)/lib/*.a $(SUNFLOWERROOT)/tools/tools-lib/$(TARGET)/;\
	cp $(PREFIX)/bin/$(TARGET-ARCH)* $(TOOLS)/bin/;\
	$(DEL) $(GCC)/objdir;\




newlib: newlib-pre newlib-post

newlib-pre:
	if test -d $(TOOLS)/bin; then true; else mkdir $(TOOLS)/bin; fi;\
	if test -d $(TOOLS)/$(TARGET); then true; else mkdir $(TOOLS)/$(TARGET); fi;\
	cd $(NEWLIB);\
	$(DEL) objdir; mkdir -p objdir; cd objdir;\
	../configure --target=$(TARGET-ARCH) --host=$(HOST) --prefix=$(PREFIX)\
		-v --with-stabs --nfp --disable-multilib;\
	$(MAKE) CC=$(TOOLCC) all;\
	$(MAKE) CC=$(TOOLCC) install;\

newlib-post:
	cp $(PREFIX)/$(TARGET-ARCH)/lib/*.a $(SUNFLOWERROOT)/tools/tools-lib/$(TARGET)/;\
	#$(DEL) $(NEWLIB)/objdir;\





clean:
	@echo '==> $(BINUTILS)'; $(DEL) $(BINUTILS)/objdir
	@echo '==> $(GCC)'; $(DEL) $(GCC)/objdir
	@echo '==> $(NEWLIB)'; $(DEL) $(NEWLIB)/objdir

nuke:
	@echo '==> $(BINUTILS)'; $(DEL) $(BINUTILS)/objdir
	@echo '==> $(GCC)'; $(DEL) $(GCC)/objdir
	@echo '==> $(TOOLS)'; $(DEL) $(TOOLS)/bin
	@echo '==> $(NEWLIB)'; $(DEL) $(NEWLIB)/objdir
	@set -e; for dir in $(SUPPORTED-TARGETS); do\
		($(DEL) $$dir; echo 'rm -rf' $$dir);\
	done;\
