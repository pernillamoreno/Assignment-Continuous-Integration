CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++14 -MMD

TEST_EXE = test #variable for the exutable file
BUILD_DIR = build

#OBJECTS = $(filter-out CircularQueue.cpp, $(wildcard *.cpp))
OBJECTS = $(wildcard *.cpp)
OBJECTS := $(addprefix $(BUILD_DIR)/, $(OBJECTS:.cpp=.o))

all:.mkbuild $(BUILD_DIR)/$(TEST_EXE)

$(BUILD_DIR)/$(TEST_EXE): $(OBJECTS)
	@$(CC) $+ -lgtest -lgtest_main -o $@

#any object file=%.o depend on any=% sorcefile=.cpp
$(BUILD_DIR)/%.o: %.cpp
	@$(CC) -c $(CFLAGS) $< -o $@

-include $(wildcard $(BUILD_DIR)/*.d) 

.PHONY: all check clean .mkbuild 


check: .mkbuild $(BUILD_DIR)/$(TEST_EXE) 
	@./$(BUILD_DIR)/$(TEST_EXE)

clean:
	@rm -rf $(BUILD_DIR)

.mkbuild:
	@mkdir -p $(BUILD_DIR)