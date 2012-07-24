OBJ=shuduku
CC=g++
CXXFLAGS=-DDEBUG -Wall -O2 -g

$(OBJ):SuDoku.C SuDoku.H
	$(CC) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) 
