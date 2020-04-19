# Makefile for CSU face recognition codes
# Makefile reminders: Check these things first if there is a problem with make.
# What to do if you get a messge that says: no rule to make *.o
#   1.  It is very posible that there is a header file has
#       been removed.  Check the variable HEADERS.
#   2.  Check rules to make sure that command come after a TAB and not
#       eight spaces.
#   3.  Check to make sure there were not any confilcts in cvs.  These usually
#       look like lines with ">>>>>".

TREEROOT	= ../../../../..
include $(TREEROOT)/conf/setup.conf

PORT		= ../../port
LIBOS		= mOS
SIMDIR		= /Users/pip/inferno20031205/libmyrmigki
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I../../libsfpthread/ -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(INCLUDES) $(TARGET-ARCH-FLAGS) $(OPTFLAGS)
LFLAGS       	= -Ttext $(LOADADDR) -T$(PORT)/superH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $@.map -L../../libsfpthread/ -lc -lm -lgcc -l$(LIBOS)-$(TARGET) -lm -lsfpthread -g
LOADADDR	= 0x08004000

#pip ARCH              = $(shell arch)
ARCH              = sunflowersim


OS_LIB_VERSION    = these_binaries_were_built_for_$(ARCH)

#pip CC                = gcc
#pip LINKER            = gcc
LINKER            = $(LD)

#CFLAGS            = -I$(INC_DIR) -O3 -g -Wall -pedantic # add flags needed by your compiler
#CFLAGS            = -I$(INC_DIR) -O3 -mcpu=pentiumpro -malign-double -g -Wall # add flags needed by your compiler
#pip CFLAGS            = -I$(INC_DIR) -O3 -Wall -DSIMD=0
CFLAGS            += -I$(INC_DIR) -O3 -Wall -DSUPERH #-DTHRD -DNUM_THREADS=4

#CFLAGS            =  -DALEX -DNUM_THREADS=4 -lpthread -I$(INC_DIR) -O3 -pg -Wall
#LFLAGS            = -lm # add flags needed by your linker

#pip LFLAGS            = -lm -lpthread -g # do code profiling

JUNK              = *~ core
CLEANUP           = rm -f $(JUNK)

SUBDIRS           = . 

SRC_DIR           = src
INC_DIR           = src
OBJ_DIR           = lib/$(ARCH)
LIB_DIR           =
BIN_DIR           = bin/$(ARCH)
RES_DIR           = results

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)


FILES_COMMON        = csuCommonMatrix csuCommonFile csuCommonUtil csuCommonSubspace \
                      csuCommonCommandLine csuCommonImage csuEBGMUtil csuEBGMSimilarity

# Alas, csuCommonSubspace.c now depends on csuSubspace[Eigen|Fisher|CVEigen]
FILES_COMMON2       = $(FILES_COMMON) csuSubspaceEigen csuSubspaceFisher csuSubspaceCVEigen


FILES_TRAIN         = csuSubspaceEigen csuSubspaceFisher csuSubspaceTrain csuSubspaceCVEigen $(FILES_COMMON2)
FILES_PROJECT       = csuSubspaceProject $(FILES_COMMON2)
FILES_PERMUTE       = csuAnalyzePermute $(FILES_COMMON2)
FILES_RANKCURVE     = csuAnalyzeRankCurve $(FILES_COMMON2)
FILES_TWOSTAGE      = csuAnalyzeTwoStage $(FILES_COMMON2)
FILES_REPLICATES    = csuToolsReplicates $(FILES_COMMON2)
FILES_PGM2SFI       = csuToolsPGM2SFI $(FILES_COMMON2)
FILES_SFI2PGM       = csuToolsSFI2PGM $(FILES_COMMON2)
FILES_IMGSTATS      = csuToolsImageStats $(FILES_COMMON2)
FILES_DUMPSPACE     = csuToolsWriteSubspaceAsMatrices $(FILES_COMMON2)
FILES_RESIZE        = csuResizeImages resample $(FILES_COMMON2)
FILES_BAYESIAN      = csuBayesianProject $(FILES_COMMON2)
FILES_BAY_TRAIN     = csuBayesianTrain $(FILES_COMMON2)
FILES_NET_TRAIN     = csuNeuralTrain backprop $(FILES_COMMON2)
FILES_NET_PROJECT   = csuNeuralProject backprop $(FILES_COMMON2)
FILES_NORM          = csuPreprocessNormalize $(FILES_COMMON2)
FILES_EBGMGRID     = csuEBGMGrid $(FILES_COMMON2)
FILES_EBGMMASKS    = csuEBGMMasks $(FILES_COMMON2)
FILES_EBGMLOC      = csuEBGMLocalization $(FILES_COMMON2)
FILES_EBGMFIT      = csuEBGMGraphFit $(FILES_COMMON2)
FILES_EBGMFACE     = csuEBGMFaceGraph $(FILES_COMMON2)
FILES_EBGMMEASURE  = csuEBGMMeasure $(FILES_COMMON2)

HEADER_FILES        = $(FILES_COMMON:%=%.h) csuCommon.h

HEADERS             = $(HEADER_FILES:%=$(INC_DIR)/%)

OBJECTS_TRAIN       = init.o $(FILES_TRAIN:%=%.o)
OBJECTS_PROJECT     = init.o $(FILES_PROJECT:%=%.o)
OBJECTS_PERMUTE     = init.o $(FILES_PERMUTE:%=%.o)
OBJECTS_RANKCURVE   = init.o $(FILES_RANKCURVE:%=%.o)
OBJECTS_TWOSTAGE    = init.o $(FILES_TWOSTAGE:%=%.o)
OBJECTS_REPLICATES  = init.o $(FILES_REPLICATES:%=%.o)
OBJECTS_PGM2SFI     = init.o $(FILES_PGM2SFI:%=%.o)
OBJECTS_SFI2PGM     = init.o $(FILES_SFI2PGM:%=%.o)
OBJECTS_IMGSTATS    = init.o $(FILES_IMGSTATS:%=%.o)
OBJECTS_DUMPSPACE   = init.o $(FILES_DUMPSPACE:%=%.o)
OBJECTS_RESIZE      = init.o $(FILES_RESIZE:%=%.o)
OBJECTS_BAYESIAN    = init.o $(FILES_BAYESIAN:%=%.o)
OBJECTS_BAY_TRAIN   = init.o $(FILES_BAY_TRAIN:%=%.o)
OBJECTS_NET_TRAIN   = init.o $(FILES_NET_TRAIN:%=%.o)
OBJECTS_NET_PROJECT = init.o $(FILES_NET_PROJECT:%=%.o)
OBJECTS_NORM        = init.o $(FILES_NORM:%=%.o)
OBJECTS_EBGMGRID   = init.o $(FILES_EBGMGRID:%=%.o)
OBJECTS_EBGMMASKS  = init.o $(FILES_EBGMMASKS:%=%.o)
OBJECTS_EBGMLOC    = init.o $(FILES_EBGMLOC:%=%.o)
OBJECTS_EBGMFIT    = init.o $(FILES_EBGMFIT:%=%.o)
OBJECTS_EBGMFACE   = init.o $(FILES_EBGMFACE:%=%.o)
OBJECTS_EBGMMEASURE= init.o $(FILES_EBGMMEASURE:%=%.o)

OBJECTS             = $(OBJECTS_TRAIN) $(OBJECTS_PROJECT) $(OBJECTS_PERMUTE) \
                      $(OBJECTS_RANKCURVE) $(OBJECTS_TWOSTAGE) $(OBJECTS_REPLICATES) \
                      $(OBJECTS_PGM2SFI) $(OBJECTS_SFI2PGM) $(OBJECTS_IMGSTATS) $(OBJECTS_DUMPSPACE) \
                      $(OBJECTS_RESIZE) $(OBJECTS_BAYESIAN) $(OBJECTS_BAY_TRAIN) $(OBJECTS_NET_TRAIN) $(OBJECT_NET_PROJECT) \
                      $(OBJECTS_NORM) $(OBJECTS_EBGMGRID) $(OBJECTS_EBGMMASKS) $(OBJECTS_EBGMLOC)\
                      $(OBJECTS_EBGMFIT) $(OBJECTS_EBGMFACE) $(OBJECTS_EBGMMEASURE)


TARGET_TRAIN        = $(BIN_DIR)/csuSubspaceTrain
TARGET_PROJECT      = $(BIN_DIR)/csuSubspaceProject
TARGET_PERMUTE      = $(BIN_DIR)/csuAnalyzePermute
TARGET_RANKCURVE    = $(BIN_DIR)/csuAnalyzeRankCurve
TARGET_TWOSTAGE     = $(BIN_DIR)/csuAnalyzeTwoStage
TARGET_REPLICATES   = $(BIN_DIR)/csuToolsReplicates
TARGET_PGM2SFI      = $(BIN_DIR)/csuToolsPGM2SFI
TARGET_SFI2PGM      = $(BIN_DIR)/csuToolsSFI2PGM
TARGET_IMGSTATS     = $(BIN_DIR)/csuToolsImageStats
TARGET_DUMPSPACE    = $(BIN_DIR)/csuToolsWriteSubspaceAsMatrices
TARGET_RESIZE       = $(BIN_DIR)/csuResizeImages
TARGET_BAYESIAN     = $(BIN_DIR)/csuBayesianProject
TARGET_BAY_TRAIN    = $(BIN_DIR)/csuBayesianTrain
TARGET_NET_TRAIN    = $(BIN_DIR)/csuNeuralTrain
TARGET_NET_PROJECT  = $(BIN_DIR)/csuNeuralProject
TARGET_NORM         = $(BIN_DIR)/csuPreprocessNormalize
TARGET_EBGMGRID    = $(BIN_DIR)/csuEBGMGrid
TARGET_EBGMMASKS   = $(BIN_DIR)/csuEBGMMasks
TARGET_EBGMLOC     = $(BIN_DIR)/csuEBGMLocalization
TARGET_EBGMFIT     = $(BIN_DIR)/csuEBGMGraphFit
TARGET_EBGMFACE    = $(BIN_DIR)/csuEBGMFaceGraph
TARGET_EBGMMEASURE = $(BIN_DIR)/csuEBGMMeasure

TARGETS             = $(TARGET_TRAIN) $(TARGET_PROJECT) $(TARGET_PERMUTE) \
                      $(TARGET_RANKCURVE) $(TARGET_TWOSTAGE) $(TARGET_REPLICATES) \
                      $(TARGET_PGM2SFI) $(TARGET_IMGSTATS) $(TARGET_DUMPSPACE) $(TARGET_RESIZE) \
                      $(TARGET_BAYESIAN) $(TARGET_BAY_TRAIN) $(TARGET_NET_TRAIN) $(TARGET_NET_PROJECT) $(TARGET_NORM) \
                      $(TARGET_EBGMGRID) $(TARGET_EBGMMASKS) $(TARGET_EBGMLOC) $(TARGET_EBGMFIT) \
                      $(TARGET_EBGMFACE) $(TARGET_EBGMMEASURE) $(TARGET_SFI2PGM)



# ************************  MAKEFILE RULES **************************
# ************************  -------------- **************************

stable: stablemessage archdirs subspace bayesian analyze tools preprocess ebgm

stablemessage:
	@ echo "************* Building stable analysis tools ************"

ansi: override CFLAGS = -I$(INC_DIR) -g -ansi -pedantic -Wall

ansi: clean devel

ansi-error: override CFLAGS = -I$(INC_DIR) -g -ansi -pedantic -Wall -Werror

ansi-error: devel

rebuild: clean devel

devel: develmessage archdirs subspace analyze tools preprocess bayesian neural ebgm

develmessage:
	@ echo "************* Building development analysis tools ************"

archdirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

subspace: $(TARGET_TRAIN) $(TARGET_PROJECT) 

analyze: $(TARGET_RANKCURVE) $(TARGET_PERMUTE) $(TARGET_TWOSTAGE)

tools: $(TARGET_PGM2SFI) $(TARGET_REPLICATES) \
       $(TARGET_SFI2PGM) $(TARGET_IMGSTATS) $(TARGET_DUMPSPACE)

preprocess: $(TARGET_NORM)

bayesian: $(TARGET_BAYESIAN) $(TARGET_BAY_TRAIN) 

neural: $(TARGET_NET_TRAIN) $(TARGET_NET_PROJECT)

ebgm: $(TARGET_EBGMGRID) $(TARGET_EBGMMASKS) $(TARGET_EBGMLOC)\
       $(TARGET_EBGMFIT) $(TARGET_EBGMFACE) $(TARGET_EBGMMEASURE)

extra: $(BIN_DIR)/csuCovariateStudy $(BIN_DIR)/csuToolAddSFI

targets: $(TARGETS)

$(TARGET_TRAIN): $(OBJECTS_TRAIN:%=$(OBJ_DIR)/%)
	$(LINKER) $(LFLAGS) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_PROJECT): $(OBJECTS_PROJECT:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_PERMUTE): $(OBJECTS_PERMUTE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_RANKCURVE): $(OBJECTS_RANKCURVE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_TWOSTAGE): $(OBJECTS_TWOSTAGE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_REPLICATES): $(OBJECTS_REPLICATES:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_PGM2SFI): $(OBJECTS_PGM2SFI:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_SFI2PGM): $(OBJECTS_SFI2PGM:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_IMGSTATS): $(OBJECTS_IMGSTATS:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_DUMPSPACE): $(OBJECTS_DUMPSPACE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_RESIZE): $(OBJECTS_RESIZE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_BAYESIAN): $(OBJECTS_BAYESIAN:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_BAY_TRAIN): $(OBJECTS_BAY_TRAIN:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_NET_TRAIN): $(OBJECTS_NET_TRAIN:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_NET_PROJECT): $(OBJECTS_NET_PROJECT:%=$(OBJ_DIR)/%)
	$(LINKER) $(LFLAGS) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_NORM): $(OBJECTS_NORM:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMGRID): $(OBJECTS_EBGMGRID:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMMASKS): $(OBJECTS_EBGMMASKS:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMLOC): $(OBJECTS_EBGMLOC:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMFIT): $(OBJECTS_EBGMFIT:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMFACE): $(OBJECTS_EBGMFACE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(TARGET_EBGMMEASURE): $(OBJECTS_EBGMMEASURE:%=$(OBJ_DIR)/%)
	$(LINKER) $^ -o $@ $(LFLAGS)
	$(OBJCOPY) -O srec $@ $@.sr
	$(OBJDUMP) -G $@ > $@.stabs

$(BIN_DIR)/csuCovariateStudy: extras/csuCovariateStudy.cpp $(FILES_COMMON2:%=src/%.c)
	g++ -g -O3 -Isrc  $(FILES_COMMON2:%=src/%.c) extras/csuCovariateStudy.cpp -o $(BIN_DIR)/csuCovariateStudy

$(BIN_DIR)/csuToolAddSFI: extras/csuToolAddSFI.cpp
	g++ -g -O3 -Isrc extras/csuToolAddSFI.cpp -o $(BIN_DIR)/csuToolAddSFI

$(OBJECTS): $(HEADERS_FILES)
 

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/init.o: $(PORT)/init.S
	$(CPP) $(PORT)/init.S > init.i; $(AS) init.i -o $@; rm init.i

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGETS) core *~ *.o $(BIN_DIR)/*


