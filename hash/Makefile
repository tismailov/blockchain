# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

# Source files
SRCS = stribog.c test.c
OBJS = $(SRCS:.c=.o)
TARGET = stribog_test

# All Targets
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Generic rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET) *.txt *.exe

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