CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -MMD
LDLIBS = -lgtest_main -lgtest -lgmock
TEXT_EXE = test
BUILD_DIR = build
OBJECTS = $(wildcard *.cpp)
OBJECTS := $(addprefix $(BUILD_DIR)/, $(OBJECTS:.cpp=.o))

all: .mkbuild $(BUILD_DIR)/$(TEXT_EXE)

$(BUILD_DIR)/$(TEXT_EXE): $(OBJECTS)
	@$(CC) $+ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) -c $(CFLAGS) $< -o $@

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean .mkbuild

check: .mkbuild $(BUILD_DIR)/$(TEXT_EXE)
	@./$(BUILD_DIR)/$(TEXT_EXE)

clean:
	@rm -rf $(BUILD_DIR)

.mkbuild:
	@mkdir -p $(BUILD_DIR)	