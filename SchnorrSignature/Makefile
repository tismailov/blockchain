# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -O2 -I. -I..
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lgmp -lgmpxx

# Source files
SRCS = generators.c signature.c test.c ../hash/stribog.c
CPPSRCS = prng_c_interface.cpp prng_instance.cpp ../src/prng.cpp ../hash/hash_conventer.cpp ../hash/hash_c_interface.cpp signer.cpp
OBJS = $(SRCS:.c=.o) $(CPPSRCS:.cpp=.o)
TARGET = test

# Test signer files
TEST_SIGNER_SRCS = test_signer.cpp
TEST_SIGNER_DEPS = test_signer.o signer.o generators.o signature.o ../hash/stribog.o prng_c_interface.o prng_instance.o ../src/prng.o ../hash/hash_conventer.o ../hash/hash_c_interface.o
TEST_SIGNER_TARGET = test_signer

# All Targets
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Build test_signer executable
$(TEST_SIGNER_TARGET): $(TEST_SIGNER_DEPS)
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
	rm -f $(OBJS) $(TEST_SIGNER_DEPS) $(TARGET) $(TEST_SIGNER_TARGET) *.txt *.exe && clear

# Run the target
run: all
	./$(TARGET)

# Run test_signer
run_signer: $(TEST_SIGNER_TARGET)
	./$(TEST_SIGNER_TARGET)

# Help message
help:
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make clean    - Clean up object files and executable"
	@echo "  make run      - Run the executable"
	@echo "  make test_signer - Build the test_signer executable"
	@echo "  make run_signer  - Run the test_signer executable"
	@echo "  make help     - Show this help message"