CC=g++
DEBUG=-g
EXE=comDow
LIBS=-lssl -lcrypto
SRC = 

$(EXE):
	$(CC) $(DEBUG) $(SRC)

clean:
	@echo "Cleaning Directory"
	rm -f *.o $(EXE)

debug:
	
	gdb $(EXE)

memcheck:
	valgrind

run:
	./$(EXE)
