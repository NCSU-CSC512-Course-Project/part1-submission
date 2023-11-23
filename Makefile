# Makefile for KeyPointsCollector
CXX = g++
CXXFLAGS = -O0 -g3 -std=c++17
LINKER_FLAGS = -lclang
DBG_FLAGS = -DDEBUG=true

DBG = gdb

BIN_DIR = bin
SRC_DIR = kpc
OBJS_DIR = $(BIN_DIR)/objs
OUT_DIR = out

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRC))
EXE = $(BIN_DIR)/kpc

.PHONY: all main run

all: dirs main

clean_out:
	rm -f $(OUT_DIR)/*

run: clean_out all
	$(EXE)

drun: all
	$(EXE) test_file.c --debug

dbg: all
	$(DBG) -q --args $(EXE) test_file.c	--debug

main: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LINKER_FLAGS) -o $(EXE) 

dirs:
	mkdir -p $(BIN_DIR) $(OBJS_DIR) $(OUT_DIR)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

