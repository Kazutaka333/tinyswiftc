# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -Wextra -g -v

# Target executable
TARGET = $(OBJDIR)/main

# Source files
SRCS = main.cpp lexer.cpp

# Object directory
OBJDIR = build

# Object files
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR)