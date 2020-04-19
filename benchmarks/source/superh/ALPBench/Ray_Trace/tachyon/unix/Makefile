# Makefile for the rendering library.
# Copyright 1994-2004 John E. Stone
# All Rights Reserved
#
# $Id: Makefile,v 1.96 2004/05/28 01:46:52 johns Exp $
#

#.SILENT:

#
# Include the machine dependent compilation flags etc.
#
include Make-opengl
include Make-config
include Make-arch
include Make-distrib

COMPILEDIR=../compile
ARCHDIR=../compile/${ARCH}
OBJDIR=${ARCHDIR}/libtachyon

#
# Name of raytracing library file to create
#
RAYLIB=${ARCHDIR}/libtachyon.a
RAYLIBDIR=${ARCHDIR}

#
# Extra parsers and such
#
PARSEOBJS = ${OBJDIR}/mgfparse.o ${OBJDIR}/ac3dparse.o 
PARSEINC  = ${MGFINC}
PARSEDIRS = ${MGFDIR}
PARSELIBS = ${MGFLIB} 

#
# Miscellaneous object files etc.
#
MISC= ${OBJDIR}/mainanim.o \
	${OBJDIR}/animspheres.o \
	${OBJDIR}/animspheres \
	${OBJDIR}/animspheres2.o \
	${OBJDIR}/animskull.o \
	${OBJDIR}/animskull \
	${OBJDIR}/animspheres2 \
	${OBJDIR}/fire.o \
	${OBJDIR}/fire \
	${OBJDIR}/hypertex.o \
	${OBJDIR}/hypertex \
	${OBJDIR}/tgatoyuv \
	${OBJDIR}/tachyon \
	${OBJDIR}/animray \
	${OBJDIR}/main.o \
	${OBJDIR}/spaceball.o \
	${OBJDIR}/trackball.o \
	${OBJDIR}/getargs.o \
	${OBJDIR}/parse.o \
	${OBJDIR}/nffparse.o \
	${OBJDIR}/glwin.o \
	${OBJDIR}/init.o \
	${OBJDIR}/devrtc.o \
	${OBJDIR}/misc.o 


#
# Test programs included..
#
#BINARIES = ${COMPILEDIR} ${ARCHDIR} ${OBJDIR} ${PARSEDIRS} \
#	${RAYLIB} ${PARSELIB} ${ARCHDIR}/tachyon \
#	${ARCHDIR}/fire ${ARCHDIR}/hypertex ${ARCHDIR}/tgatoyuv \
#	${ARCHDIR}/animray ${ARCHDIR}/animspheres ${ARCHDIR}/animskull \
#	${ARCHDIR}/animspheres2

#
# No test programs included..
#
BINARIES = ${COMPILEDIR} ${ARCHDIR} ${OBJDIR} ${PARSEDIRS} \
	${RAYLIB} ${PARSELIB} ${ARCHDIR}/tachyon


#----------------------------------------------------------------------
# Demo Programs etc
#----------------------------------------------------------------------

DEMOINC= -I${SRCDIR} -I${DEMOSRC}

all : mesg directories 
	${MAKE} ${BINARIES}
	@echo ""
	@echo "Build Complete"

directories :
	@if [ ! -d ${COMPILEDIR} ] ; then \
		echo "Making compile directory ${COMPILEDIR}"; \
		mkdir ${COMPILEDIR}; \
	fi; 
	@if [ ! -d ${ARCHDIR} ] ; then \
		echo "Making architecture directory ${ARCHDIR}"; \
		mkdir ${ARCHDIR}; \
	fi; 
	@if [ ! -d ${OBJDIR} ] ; then \
		echo "Making library directory ${OBJDIR}"; \
		mkdir ${OBJDIR}; \
	fi; 
	

mesg : 
	@echo "Building Parallel Raytracing library"
	@echo "Copyright 1994-2004, John E. Stone"
	@echo "All Rights Reserveed"

${MGFDIR} : ${ARCHDIR}
	@if [ ! -d ${MGFDIR} ] ; then \
		echo "Making libmgf directory ${MGFDIR}"; \
		mkdir ${MGFDIR}; \
	fi; 

#
# MGF Library compilation
#
${MGFLIB} : ../libmgf/Makefile
	{cd ../libmgf; \
	${MAKE} -f Makefile RANLIB=${RANLIB} MGFLIBDIR=${MGFLIBDIR} \
	MGFLIB=${MGFLIB} AR=${AR} ARFLAGS=${ARFLAGS} \
	};

${ARCHDIR}/tachyon : ${RAYLIB} ${PARSELIB} ${OBJDIR}/main.o ${OBJDIR}/getargs.o ${OBJDIR}/parse.o ${OBJDIR}/nffparse.o ${OBJDIR}/glwin.o ${OBJDIR}/spaceball.o ${OBJDIR}/trackball.o ${PARSEOBJS} $(SFINITOBJS)
	${LD} -o ${ARCHDIR}/tachyon $(SFINITOBJS) ${OBJDIR}/main.o ${OBJDIR}/getargs.o ${OBJDIR}/parse.o ${OBJDIR}/nffparse.o ${OBJDIR}/glwin.o ${OBJDIR}/spaceball.o ${OBJDIR}/trackball.o ${PARSEOBJS} -L${RAYLIBDIR} ${PARSELIBS} ${LFLAGS} ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/tachyon ${ARCHDIR}/tachyon.sr
	$(OBJDUMP) -G ${ARCHDIR}/tachyon > ${ARCHDIR}/tachyon.stabs
	${STRIP} ${ARCHDIR}/tachyon

${ARCHDIR}/animray : ${RAYLIB} ${OBJDIR}/mainanim.o $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/animray $(SFINITOBJS) ${OBJDIR}/mainanim.o ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/animray ${ARCHDIR}/animray.sr
	$(OBJDUMP) -G ${ARCHDIR}/animray > ${ARCHDIR}/animray.stabs
	${STRIP} ${ARCHDIR}/animray 

${ARCHDIR}/animspheres : ${RAYLIB} ${OBJDIR}/animspheres.o $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/animspheres $(SFINITOBJS) ${OBJDIR}/animspheres.o ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/animspheres ${ARCHDIR}/animspheres.sr
	$(OBJDUMP) -G ${ARCHDIR}/animspheres > ${ARCHDIR}/animspheres.stabs
	${STRIP} ${ARCHDIR}/animspheres

${ARCHDIR}/animspheres2 : ${RAYLIB} ${OBJDIR}/animspheres2.o  $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/animspheres2 $(SFINITOBJS) ${OBJDIR}/animspheres2.o ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/animspheres2 ${ARCHDIR}/animspheres2.sr
	$(OBJDUMP) -G ${ARCHDIR}/animspheres2 > ${ARCHDIR}/animspheres2.stabs
	${STRIP} ${ARCHDIR}/animspheres2

${ARCHDIR}/hypertex : ${RAYLIB} ${OBJDIR}/hypertex.o ${DEMOSRC}/hypertex.c  $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/hypertex $(SFINITOBJS) ${OBJDIR}/hypertex.o ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/hypertex ${ARCHDIR}/hypertex.sr
	$(OBJDUMP) -G ${ARCHDIR}/hypertex > ${ARCHDIR}/hypertex.stabs
	${STRIP} ${ARCHDIR}/hypertex 

${ARCHDIR}/fire : ${RAYLIB} ${OBJDIR}/fire.o ${DEMOSRC}/fire.c  $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/fire $(SFINITOBJS) ${OBJDIR}/fire.o
	$(OBJCOPY) -O srec ${ARCHDIR}/fire ${ARCHDIR}/fire.sr
	$(OBJDUMP) -G ${ARCHDIR}/fire > ${ARCHDIR}/fire.stabs
	${STRIP} ${ARCHDIR}/fire

${ARCHDIR}/animskull : ${RAYLIB} ${OBJDIR}/animskull.o  $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/animskull $(SFINITOBJS) ${OBJDIR}/animskull.o ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/animskull ${ARCHDIR}/animskull.sr
	$(OBJDUMP) -G ${ARCHDIR}/animskull > ${ARCHDIR}/animskull.stabs
	${STRIP} ${ARCHDIR}/animskull

${ARCHDIR}/tgatoyuv : ${RAYLIB} ${DEMOSRC}/tgatoyuv.c  $(SFINITOBJS)
	${LD} -L${RAYLIBDIR} ${LFLAGS} -o ${ARCHDIR}/tgatoyuv $(SFINITOBJS) ${DEMOSRC}/tgatoyuv.c ${EXTRALIBS}
	$(OBJCOPY) -O srec ${ARCHDIR}/tgatoyuv ${ARCHDIR}/tgatoyuv.sr
	$(OBJDUMP) -G ${ARCHDIR}/tgatoyuv > ${ARCHDIR}/tgatoyuv.stabs
	${STRIP} ${ARCHDIR}/tgatoyuv

#
# Rules for building demo .o files
#

${OBJDIR}/animskull.o : ${DEMOSRC}/animskull.c 
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/animskull.c -o ${OBJDIR}/animskull.o

${OBJDIR}/hypertex.o : ${DEMOSRC}/hypertex.c
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/hypertex.c -o ${OBJDIR}/hypertex.o

${OBJDIR}/fire.o : ${DEMOSRC}/fire.c
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/fire.c -o ${OBJDIR}/fire.o

${OBJDIR}/mainanim.o : ${DEMOSRC}/mainanim.c
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/mainanim.c -o ${OBJDIR}/mainanim.o

${OBJDIR}/main.o : ${DEMOSRC}/main.c ${DEMOSRC}/getargs.h ${DEMOSRC}/parse.h ${DEMOSRC}/nffparse.h ${DEMOSRC}/ac3dparse.h ${DEMOSRC}/glwin.h
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/main.c -o ${OBJDIR}/main.o

${OBJDIR}/getargs.o : ${DEMOSRC}/getargs.c ${DEMOSRC}/getargs.h
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/getargs.c -o ${OBJDIR}/getargs.o

${OBJDIR}/glwin.o : ${DEMOSRC}/glwin.c ${DEMOSRC}/glwin.h
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/glwin.c -o ${OBJDIR}/glwin.o

${OBJDIR}/spaceball.o : ${DEMOSRC}/spaceball.c
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/spaceball.c -o ${OBJDIR}/spaceball.o

${OBJDIR}/trackball.o : ${DEMOSRC}/trackball.c
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/trackball.c -o ${OBJDIR}/trackball.o

${OBJDIR}/animspheres.o : ${DEMOSRC}/animspheres.c 
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/animspheres.c -o ${OBJDIR}/animspheres.o

${OBJDIR}/animspheres2.o : ${DEMOSRC}/animspheres2.c 
	${CC} ${CFLAGS} ${DEMOINC} -c ${DEMOSRC}/animspheres2.c -o ${OBJDIR}/animspheres2.o

${OBJDIR}/parse.o : ${DEMOSRC}/parse.c ${DEMOSRC}/parse.h
	${CC} ${CFLAGS} ${PARSEINC} ${DEMOINC} -c ${DEMOSRC}/parse.c -o ${OBJDIR}/parse.o

${OBJDIR}/mgfparse.o : ${DEMOSRC}/mgfparse.c ${DEMOSRC}/mgfparse.h
	${CC} ${CFLAGS} ${PARSEINC} ${DEMOINC} ${MGFINC} -c ${DEMOSRC}/mgfparse.c -o ${OBJDIR}/mgfparse.o

${OBJDIR}/nffparse.o : ${DEMOSRC}/nffparse.c ${DEMOSRC}/nffparse.h
	${CC} ${CFLAGS} ${PARSEINC} ${DEMOINC} -c ${DEMOSRC}/nffparse.c -o ${OBJDIR}/nffparse.o

${OBJDIR}/ac3dparse.o : ${DEMOSRC}/ac3dparse.c ${DEMOSRC}/ac3dparse.h
	${CC} ${CFLAGS} ${PARSEINC} ${DEMOINC} -c ${DEMOSRC}/ac3dparse.c -o ${OBJDIR}/ac3dparse.o

#pip:
${OBJDIR}/init.o: $(PORT)/init.S
	$(CPP) $(PORT)/init.S > init.i; $(AS) init.i -o $@; rm init.i

${OBJDIR}/devrtc.o: $(PORT)/devrtc.c
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -o $@ -c $(PORT)/devrtc.c

${OBJDIR}/misc.o: $(PORT)/misc.c
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -o $@ -c $(PORT)/misc.c

#----------------------------------------------------------------------
# End of demo programs
#----------------------------------------------------------------------

OBJDEPS= ${SRCDIR}/tachyon.h \
	${SRCDIR}/machine.h \
	${SRCDIR}/rtcommon.h \
	${SRCDIR}/types.h \
	${SRCDIR}/hash.h \
	${SRCDIR}/macros.h \
	${SRCDIR}/render.h \
	${SRCDIR}/quadric.h \
	${SRCDIR}/texture.h \
	${SRCDIR}/light.h \
	${SRCDIR}/util.h

RAYOBJS= ${OBJDIR}/api.o \
	${OBJDIR}/apigeom.o \
	${OBJDIR}/box.o \
	${OBJDIR}/global.o \
	${OBJDIR}/hash.o \
	${OBJDIR}/parallel.o \
	${OBJDIR}/threads.o \
	${OBJDIR}/camera.o \
	${OBJDIR}/render.o \
	${OBJDIR}/trace.o \
	${OBJDIR}/grid.o \
	${OBJDIR}/intersect.o \
	${OBJDIR}/sphere.o \
	${OBJDIR}/plane.o \
	${OBJDIR}/ring.o \
	${OBJDIR}/triangle.o \
	${OBJDIR}/cylinder.o \
	${OBJDIR}/quadric.o \
	${OBJDIR}/extvol.o \
	${OBJDIR}/vol.o \
	${OBJDIR}/ui.o \
	${OBJDIR}/util.o \
	${OBJDIR}/vector.o \
	${OBJDIR}/shade.o \
	${OBJDIR}/texture.o \
	${OBJDIR}/coordsys.o \
	${OBJDIR}/imap.o \
	${OBJDIR}/light.o \
	${OBJDIR}/imageio.o \
	${OBJDIR}/jpeg.o \
	${OBJDIR}/pngfile.o \
	${OBJDIR}/ppm.o \
	${OBJDIR}/sgirgb.o \
	${OBJDIR}/tgafile.o \
	${OBJDIR}/winbmp.o

${RAYLIB} : ${RAYOBJS}
	rm -f ${RAYLIB}
	${AR} ${ARFLAGS} ${RAYLIB} ${RAYOBJS}
	${RANLIB} ${RAYLIB}

${OBJDIR}/vol.o : ${SRCDIR}/vol.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/vol.c -o ${OBJDIR}/vol.o

${OBJDIR}/extvol.o : ${SRCDIR}/extvol.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/extvol.c -o ${OBJDIR}/extvol.o

${OBJDIR}/winbmp.o : ${SRCDIR}/winbmp.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/winbmp.c -o ${OBJDIR}/winbmp.o

${OBJDIR}/vector.o : ${SRCDIR}/vector.c ${OBJDEPS} 
	${CC} ${CFLAGS} -c ${SRCDIR}/vector.c -o ${OBJDIR}/vector.o

${OBJDIR}/triangle.o : ${SRCDIR}/triangle.c ${OBJDEPS} ${SRCDIR}/triangle.h
	${CC} ${CFLAGS} -c ${SRCDIR}/triangle.c -o ${OBJDIR}/triangle.o

${OBJDIR}/trace.o : ${SRCDIR}/trace.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/trace.c -o ${OBJDIR}/trace.o

${OBJDIR}/threads.o : ${SRCDIR}/threads.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/threads.c -o ${OBJDIR}/threads.o

${OBJDIR}/tgafile.o : ${SRCDIR}/tgafile.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/tgafile.c -o ${OBJDIR}/tgafile.o

${OBJDIR}/util.o : ${SRCDIR}/util.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/util.c -o ${OBJDIR}/util.o

${OBJDIR}/ui.o : ${SRCDIR}/ui.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/ui.c -o ${OBJDIR}/ui.o

${OBJDIR}/texture.o : ${SRCDIR}/texture.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/texture.c -o ${OBJDIR}/texture.o

${OBJDIR}/sphere.o : ${SRCDIR}/sphere.c ${OBJDEPS} ${SRCDIR}/sphere.h
	${CC} ${CFLAGS} -c ${SRCDIR}/sphere.c -o ${OBJDIR}/sphere.o

${OBJDIR}/sgirgb.o : ${SRCDIR}/sgirgb.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/sgirgb.c -o ${OBJDIR}/sgirgb.o

${OBJDIR}/shade.o : ${SRCDIR}/shade.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/shade.c -o ${OBJDIR}/shade.o

${OBJDIR}/ring.o : ${SRCDIR}/ring.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/ring.c -o ${OBJDIR}/ring.o

${OBJDIR}/render.o : ${SRCDIR}/render.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/render.c -o ${OBJDIR}/render.o

${OBJDIR}/quadric.o : ${SRCDIR}/quadric.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/quadric.c -o ${OBJDIR}/quadric.o

${OBJDIR}/jpeg.o : ${SRCDIR}/jpeg.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/jpeg.c -o ${OBJDIR}/jpeg.o

${OBJDIR}/pngfile.o : ${SRCDIR}/pngfile.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/pngfile.c -o ${OBJDIR}/pngfile.o

${OBJDIR}/ppm.o : ${SRCDIR}/ppm.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/ppm.c -o ${OBJDIR}/ppm.o

${OBJDIR}/plane.o : ${SRCDIR}/plane.c ${OBJDEPS} ${SRCDIR}/plane.h
	${CC} ${CFLAGS} -c ${SRCDIR}/plane.c -o ${OBJDIR}/plane.o

${OBJDIR}/parallel.o : ${SRCDIR}/parallel.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/parallel.c -o ${OBJDIR}/parallel.o

${OBJDIR}/objbound.o : ${SRCDIR}/objbound.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/objbound.c -o ${OBJDIR}/objbound.o

${OBJDIR}/light.o : ${SRCDIR}/light.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/light.c -o ${OBJDIR}/light.o

${OBJDIR}/intersect.o : ${SRCDIR}/intersect.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/intersect.c -o ${OBJDIR}/intersect.o

${OBJDIR}/imageio.o : ${SRCDIR}/imageio.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/imageio.c -o ${OBJDIR}/imageio.o

${OBJDIR}/imap.o : ${SRCDIR}/imap.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/imap.c -o ${OBJDIR}/imap.o

${OBJDIR}/grid.o : ${SRCDIR}/grid.c ${SRCDIR}/grid.h ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/grid.c -o ${OBJDIR}/grid.o

${OBJDIR}/global.o : ${SRCDIR}/global.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/global.c -o ${OBJDIR}/global.o

${OBJDIR}/hash.o : ${SRCDIR}/hash.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/hash.c -o ${OBJDIR}/hash.o

${OBJDIR}/cylinder.o : ${SRCDIR}/cylinder.c ${OBJDEPS} ${SRCDIR}/cylinder.h
	${CC} ${CFLAGS} -c ${SRCDIR}/cylinder.c -o ${OBJDIR}/cylinder.o

${OBJDIR}/coordsys.o : ${SRCDIR}/coordsys.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/coordsys.c -o ${OBJDIR}/coordsys.o

${OBJDIR}/camera.o : ${SRCDIR}/camera.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/camera.c -o ${OBJDIR}/camera.o

${OBJDIR}/box.o : ${SRCDIR}/box.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/box.c -o ${OBJDIR}/box.o

${OBJDIR}/bndbox.o : ${SRCDIR}/bndbox.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/bndbox.c -o ${OBJDIR}/bndbox.o

${OBJDIR}/apigeom.o : ${SRCDIR}/apigeom.c ${OBJDEPS}
	${CC} ${CFLAGS} -c ${SRCDIR}/apigeom.c -o ${OBJDIR}/apigeom.o

${OBJDIR}/api.o : ${SRCDIR}/api.c ${OBJDEPS} ${SRCDIR}/sphere.h ${SRCDIR}/plane.h ${SRCDIR}/triangle.h ${SRCDIR}/cylinder.h
	${CC} ${CFLAGS} -c ${SRCDIR}/api.c -o ${OBJDIR}/api.o

clean :
	@echo "Cleaning object files, binaries etc."
	@echo ""
	rm -rf ${COMPILEDIR}/*
