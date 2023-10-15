TARGET = mymalloc
CC     = gcc
CFLAGS = -g -Wall -Wvla -Werror -fsanitize=address,undefined
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@
memgrind: memgrind.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM
