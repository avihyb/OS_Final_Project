# Compiler options
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS :=
GUI := -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin

# Targets
EXECUTABLE := $(BINDIR)/main

# Source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS := $(wildcard $(INCDIR)/*.hpp)

# Default target
all: $(EXECUTABLE)

# Link object files into executable
$(EXECUTABLE): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $^ -o $@ $(GUI)

# Compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@ $(GUI)

# Clean objects and executable
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

# Phony targets
.PHONY: all clean
