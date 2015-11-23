
CXX      = g++
LINK     = g++
CXXFLAGS = -O2 -Wall -DDEBUG
#CXXFLAGS = -g -DDEBUG_ATPG -Wall #Turn on debugging
INCPATH  =
#LIBS     = -lreadline -lcurses
DEL_FILE = rm -f

####### Files

HEADERS = gate.h tree.h

SOURCES = main.cpp gate.cpp tree.cpp

OBJECTS = gate.o tree.o main.o

TARGET  = tech

####### Implicit rules

.SUFFIXES: .o .cpp .cc .cxx

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

####### Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(INCPATH)

$(OBJECTS): $(HEADERS) Makefile

#readcircuit.tab.cc: readcircuit.y gate.h circuit.h
#	bison -d -t readcircuit.y
#	mv readcircuit.tab.c readcircuit.tab.cc

#lex.yy.cc: readcircuit.l readcircuit.y
#	flex readcircuit.l 
#	mv lex.yy.c lex.yy.cc

clean:
	@$(DEL_FILE) $(OBJECTS) $(TARGET)
#	@$(DEL_FILE) readcircuit.tab.cc readcircuit.tab.h lex.yy.cc

