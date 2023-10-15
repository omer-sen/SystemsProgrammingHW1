TARGET = mymalloc
CC     = gcc
CFLAGS = -g -c -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined
$(TARGET): $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM
