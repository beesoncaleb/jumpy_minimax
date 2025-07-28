CC = gcc
CFLAGS = -Wall -Wextra -O2

# Default target
jumpy: jumpy.c
	$(CC) $(CFLAGS) -o jumpy jumpy.c

# Clean target to remove compiled binary
clean:
	rm -f jumpy

