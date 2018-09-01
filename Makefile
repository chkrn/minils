CC=gcc
CFLAGS=-I.
DEPS = dynar.h
OBJ = dynar.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

minils: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o minils
