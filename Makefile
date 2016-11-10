CC=g++
DEBUG=-g
EXE=comDow
LIBS=-lssl -lcrypto
C_OBJ=-c
C_LINK=-o

%.o: *.cpp 
	$(CC) $(DEBUG) $(C_OBJ) *.cpp

%.o:*.h

$(EXE): %.o
	$(CC) $(DEBUG) $(LIBS) *.o $(C_LINK) $(EXE)

clean:
	@echo "Cleaning Directory"
	rm -f *.o $(EXE)

debug:
	gdb $(EXE)

run:
	./$(EXE)
