CC      = gcc
CFLAGS  = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined

TARGET = mymalloc

all: $(TARGET)

$(TARGET): mymalloc.o memgrind.o
	$(CC) $(CFLAGS) mymalloc.o memgrind.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

clean:
	rm -f mymalloc.o memgrind.o memgrind

