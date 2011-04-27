SRC = mrfstereo.cpp

CC = g++
CPPFLAGS = -O3 -W -Wall -I../MRF -I../imageLib -arch i386
OBJ = $(SRC:.cpp=.o)
BIN = $(SRC:.cpp=)
EXE = $(SRC:.cpp=.exe)
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

clean: 
	rm -f $(OBJ) core *.stackdump

allclean: clean
	rm -f $(BIN) $(EXE)

damnBinary: $(OBJ)
	g++ $(OBJ) $(LDLIBS) $(CPPFLAGS) -I/usr/X11/include -L/usr/X11/lib -o mrfstereo
