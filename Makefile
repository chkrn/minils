CC=gcc
CFLAGS=-I.
DEPS = file_info.h
OBJ = file_info.o main.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

minils: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o minils
