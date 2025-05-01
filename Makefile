# Compiler and flags
CXX = g++
INCLUDES = -IClient/Headers
CXXFLAGS = -Wall -std=c++17 $(INCLUDES)

# Source files (with subdirectories specified)
SRCS = Client/TUI/CreateUI.cpp \
	   Server/prac.cpp
      
# Server/prac.cpp     		(PRODUCTION)
# Client/Test/test.cpp 		(UI/UX TESTING)
# Server/Test/test.cpp 		(Server TESTING)

# Object files (placed inside build/ directory, keeping folder structure)
OBJS = $(SRCS:%.cpp=build/%.o)

# Output binary name
TARGET = SupplySync

# Default build rule (link object files into the final binary)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile .cpp to .o, placing object files in the build directory
build/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r build





