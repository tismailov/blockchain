# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I. -I..
LDFLAGS = -lgmp

# Source files
SRCS = generators.c signature.c test.c ../hash/stribog.c
OBJS = $(SRCS:.c=.o)
TARGET = test

# All Targets
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Generic rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Specific rule for hash/stribog.c
../hash/stribog.o: ../hash/stribog.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET) *.txt *.exe && clear

# Run the target
run: all
	./$(TARGET)

# Help message
help:
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make clean    - Clean up object files and executable"
	@echo "  make run      - Run the executable"
	@echo "  make help     - Show this help message"