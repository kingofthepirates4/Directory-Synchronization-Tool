# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Output executable
OUTPUT = dirsync

# Source files
SRCS = dirsync.c readdir.c globals.c options.c compare.c sync.c glob.c

# Object files
OBJS = $(SRCS:.c=.o)

# Targets
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

