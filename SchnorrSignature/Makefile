# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -O2 -I. -I..
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lgmp -lgmpxx

# Source files
SRCS = generators.c signature.c test.c ../hash/stribog.c
CPPSRCS = prng_c_interface.cpp prng_instance.cpp ../src/prng.cpp ../hash/hash_conventer.cpp ../hash/hash_c_interface.cpp
OBJS = $(SRCS:.c=.o) $(CPPSRCS:.cpp=.o)
TARGET = test

# All Targets
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Generic rule to build C object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule to build C++ object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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