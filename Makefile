CC=g++
DEBUG=-g
EXE=Fetch
LIBS=-lssl -lcrypto
SRC=src/*.cpp
C_OBJ=-c
C_LINK=-o


%.o: $(SRC)/%.cpp
	$(CC) $(DEBUG) $(C_OBJ) $<

$(EXE): *.o
	$(CC) $(DEBUG) $(LIBS) $? $(C_LINK) $(EXE)

clean:
	@echo "Cleaning Directory"
	rm -f *.o $(EXE)

debug:
	gdb $(EXE)

print: $(SRC)/*.cpp
	@echo $?

run:
	./$(EXE)
