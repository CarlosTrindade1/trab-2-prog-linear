all:
	g++ -I./ main.cpp graph-reader.cpp graph.cpp -lglpk -lm -o main