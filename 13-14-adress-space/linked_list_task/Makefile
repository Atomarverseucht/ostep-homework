# === Project Settings ===
CC      := gcc
CFLAGS  := -Wall -Wextra -g
TARGET  := linkedlist

# === Source Files ===
SRC     := main.c linked_list.c
OBJ     := $(SRC:.c=.o)
DEPS    := list.h

# === Default Target ===
all: $(TARGET)

# === Linking Step ===
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# === Compilation Step ===
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# === Utility Targets ===
run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

# === Prevent Make from Treating These as Files ===
.PHONY: all cle
