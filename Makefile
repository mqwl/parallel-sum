CC = gcc
CXX = g++
FLAGS := -Wall -g -fopenmp
SRC := main.cpp sums.c loc.cpp conf.cpp
EXE := ps
$(EXE): ${SRC}
	${CXX} ${SRC} ${FLAGS} -o ${EXE}
.PHONY: all
all: $(EXE)
.PHONY: clean
clean: rm -f ${EXE}