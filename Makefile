CC=gcc
CFLAGS=-I.
LFLAGS=-lSDL
DFLAGS=-g -DDEBUG
SRC=beeptimer.c din-ding.c
OBJ=$(SRC:.c=.o)
DEP=*.h
EXE=Beeptimer

all: $(EXE)
	@echo Compiled Beeptimer

$(EXE): $(OBJ) $(EMB)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o $(EXE)

$(OBJ): $(SRC) $(DEP)
	$(CC) $(CFLAGS) $(SRC) -c

debug: CFLAGS+=$(DFLAGS)
debug: all

clean:
	$(RM) *.o *~ $(EXE)

# END
