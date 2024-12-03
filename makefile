PROG = parser

all:
	g++ main.cpp -o $(PROG) --std=c++23

run: all
	./parser < input.txt
