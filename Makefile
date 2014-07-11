SRC_DIR := ./src
INCLUDE_DIR := ./include
BUILD_DIR := ./build

CPP := g++
CC := gcc

CFLAG := -O2
LFLAG := -O2

SRC := $(wildcard $(SRC_DIR)/*.cpp)


.PHONY:all $(BUILD_DIR)
all:$(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o:$(SRCn_DIR)/%.cpp
	$(CPP) -c $< $(CFLAG) -o $@ -I$(INCLUDE_DIR)

clean:
	rm -rf $(BUILD_DIR)
