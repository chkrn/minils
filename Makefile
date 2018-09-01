CC=gcc
CFLAGS=-I.
DEPS = dynar.h file_info.h
OBJ = dynar.o file_info.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

minils: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o minils
