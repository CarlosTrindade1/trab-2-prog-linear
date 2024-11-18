#include <iostream>
#include <fstream>

using namespace std;

struct Edge {
    int id;
    int cost;
    Edge *next;
};

struct Vertex {
    int id;
    Edge *edges;
};

class Graph {
    public:
        Graph(char *filename);
        void print();
        int get_num_vertices();
        int get_num_edges();
        Edge* get_vertex_edges(int vertex);

    private:
        Vertex *vertices;

        int num_vertices;
        int num_edges;

        void read_graph(char *filename);
};