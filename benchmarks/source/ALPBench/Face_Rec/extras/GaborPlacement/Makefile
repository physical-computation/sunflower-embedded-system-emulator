TARGET   = GaborPlacement

vpath %.c ../../src
vpath %.C .
vpath %.h ../../src .


QUI      = MainWindow
QCLASSES = $(QUI) GraphView GaborWindow
CLASSES  = $(QCLASSES)
UTIL     = csuCommonImage csuCommonCommandLine csuCommonFile \
           csuCommonMatrix csuGaborUtil csuCommonUtil
SOURCE   = $(TARGET).C $(CLASSES:%=%.C) $(UTIL:%=%.c)
OBJECTS  = $(TARGET).o $(CLASSES:%=%.o) $(UTIL:%=%.o) $(QCLASSES:%=%_moc.o)
HEADERS  = $(UTIL:%=%.h) 
HFILES   = $(CLASSES:%=%.h) $(HEADERS)
OPT      = -g
QTDIR    = /usr/local/qt
UTILDIR  = ../../src
CFLAGS   = $(OPT) -I$(QTDIR)/include -I$(UTILDIR)
CPPFLAGS = $(CFLAGS) -Wno-deprecated
LDFLAGS  = -L$(QTDIR)/lib -L/usr/X11R6/lib -lqt-mt -lSM -lICE -lXmu -lXext -lX11 -lm
CC       = g++
CCC      = g++
CXX      = $(CCC)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OPT) $(OBJECTS) -o $@ $(LDFLAGS)

%_moc.o: %.h
	$(QTDIR)/bin/moc -o $(@:%_moc.o=%_moc.C) $(@:%_moc.o=%.h)
	$(CXX) -c $(CPPFLAGS) -o $@ $(@:%_moc.o=%_moc.C)
	rm -f $(@:%_moc.o=%_moc.C)

%.C:%.ui %.h
	$(QTDIR)/bin/uic -i $(@:%.C=%.h) -o $(@:%.C=%.C) $(@:%.C=%.ui)

%.h:%.ui
	$(QTDIR)/bin/uic -o $(@:%.h=%.h) $(@:%.h=%.ui)

clean:
	rm -f *.o *~ \#*\# $(TARGET) *_moc.C

$(OBJECTS): $(HFILES) Makefile
