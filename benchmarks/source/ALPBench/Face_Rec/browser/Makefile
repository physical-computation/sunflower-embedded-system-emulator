TARGET   = FeretBrowser
QCLASSES = facedata filelistview radiowidget mainview imageview formview
CLASSES  = $(QCLASSES)
SOURCE   = $(TARGET).C $(CLASSES:%=%.C)
OBJECTS  = $(SOURCE:.C=.o) $(QCLASSES:%=%_moc.o) feretio.o
HFILES   = $(CLASSES:%=%.h)
OPT      = -g
QTDIR    = /usr/local/qt
CFLAGS   = $(OPT) -I$(QTDIR)/include
CPPFLAGS = $(CFLAGS)
LDFLAGS  = -L$(QTDIR)/lib  -lqt-mt -I/usr/X11R6/include  -lSM -lICE -L/usr/X11R6/lib  -lXmu  -lXext -lX11 -lm
CCC      = g++
CXX      = $(CCC)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OPT) $(OBJECTS) -o $@ $(LDFLAGS)

%_moc.o:%.h
	$(QTDIR)/bin/moc -o $(@:%_moc.o=%_moc.C) $(@:%_moc.o=%.h)
	$(CXX) -c $(CPPFLAGS) -o $@ $(@:%_moc.o=%_moc.C)
	rm -f $(@:%_moc.o=%_moc.C)

clean:
	rm -f *.o *~ \#*\# $(TARGET) $(CLASSES:%=%_moc.C)

$(OBJECTS): $(HFILES)
