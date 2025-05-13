# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -Wextra -g -v -std=c++17

# Target executable
TARGET = $(OBJDIR)/main

# Source files
SRCS = $(shell find . -name '*.cpp')

# Object directory
OBJDIR = build

# Object files
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR)