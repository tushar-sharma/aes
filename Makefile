CC=g++
CFLAGS=-I.
DEPS = aes.h
OBJ = aes.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aes: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

clean:
	rm *.o
