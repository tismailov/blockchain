CXX = /mingw64/bin/g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/mingw64/include -Isrc -I.
LDFLAGS = -L/mingw64/lib
LDLIBS = -lgmp -lgmpxx

TARGET = main.exe
SRCS = main.cpp \
       src/merkle_tree.cpp \
       src/prng.cpp \
       hash/hash_conventer.cpp \
       hash/stribog.c \
       src/create_transactions.cpp\
       src/block_builder.cpp\
       #hash/stribog_utils.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean