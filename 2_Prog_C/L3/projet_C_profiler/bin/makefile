CC= gcc
CFLAGS=-Werror -Wall -I../include
LDFLAGS=-lrt -lMLV -lm
OBJ=myprofiler


all: $(OBJ)

$(OBJ): main.o function_tree.o gui.o stack.o function_table.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: ../src/main.c ../include/function_tree.h ../include/function_table.h ../include/gui.h ../include/macro_profiler.h
function_tree.o: ../src/function_tree.c ../include/function_tree.h
gui.o: ../src/gui.c ../include/gui.h ../include/stack.h ../include/function_tree.h ../include/function_table.h
stack.o: ../src/stack.c ../include/stack.h ../include/function_tree.h ../include/function_tree.h
function_table.o: ../src/function_table.c ../include/function_table.h ../include/function_tree.h

%.o: ../src/%.c
	$(CC) -c $< $(CFLAGS)

clean: 
	rm -rf *.o
	rm -rf ../include/*.gch

mrproper: clean
	rm -f myprofiler
