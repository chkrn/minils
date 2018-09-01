CC=gcc
CFLAGS=-I. -Wall
DEPS = file_info.h Makefile
OBJ = file_info.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

minils: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o minils
