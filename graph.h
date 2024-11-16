#include <iostream>
#include <fstream>

using namespace std;

class Graph {
    public:
        Graph(char *filename);
        void print();

    private:
        struct Edge {
            int id;
            int cost;
            Edge *next;
        };
    
        struct Vertex {
            int id;
            Edge *edges;
        };

        Vertex *vertices; 

        int num_vertices;
        int num_edges;

        void read_graph(char *filename);
};