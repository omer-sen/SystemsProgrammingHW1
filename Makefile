TARGET = mymalloc
CC     = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined

all: $(TARGET)

$(TARGET): mymalloc.o memgrind.o
	$(CC) $(CFLAGS) mymalloc.o memgrind.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

clean:
	rm -rf $(TARGET) memgrind *.o *.a  *.dylib *.dSYM


	
