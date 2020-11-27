CC=g++
CFLAGS=-std=c++20

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

default: simulator

#TARGET TO COMPILE ONLY SIMULATOR
main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o

simulator: main_top.o
	$(CC) -g -o bin/seird build/main_top.o

#TARGET TO COMPILE EVERYTHING
all: simulator tests

#CLEAN COMMANDS
clean:
	rm -f bin/* build/*

