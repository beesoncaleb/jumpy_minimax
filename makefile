CC = gcc
CFLAGS = -Wall -Wextra -O0 -g

# Default target
jumpy: jumpy.c
	$(CC) $(CFLAGS) -o jumpy jumpy.c

# Clean target to remove compiled binary
clean:
	rm -f jumpy

