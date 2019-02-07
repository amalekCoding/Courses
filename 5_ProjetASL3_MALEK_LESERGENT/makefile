CFLAGS=
LDFLAGS=-lfl -ly
EXEC=as

$(EXEC): lex.yy.o $(EXEC).tab.o
	gcc -o $@ $^ $(LDFLAGS)

lex.yy.o: lex.yy.c $(EXEC).tab.h
	gcc -c $^ $(CFLAGS)

$(EXEC).tab.o: $(EXEC).tab.c
	gcc -c $^ $(CFLAGS)

lex.yy.c: $(EXEC).lex
	flex $^

$(EXEC).tab.c $(EXEC).tab.h: $(EXEC).y
	bison -d $^

clean:
	rm -rf *.o
	rm -rf *.c
	rm -rf *.h
	rm -rf *.gch
	rm -rf $(EXEC)
