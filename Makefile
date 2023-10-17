TARGET = mymalloc
CC     = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined

all: mymalloc memgrind

$(TARGET): $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -c $< -o $@

memgrind: memgrind.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM