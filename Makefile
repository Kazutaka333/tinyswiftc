# Compiler
CXX = clang++

LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS = $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs core irreader support)
LLVM_SYSTEM_LIBS = $(shell $(LLVM_CONFIG) --system-libs)

# Compiler flags
CXXFLAGS = -Wall -Wextra -g -v -std=c++17 $(LLVM_CXXFLAGS)

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
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS) $(LLVM_SYSTEM_LIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR)