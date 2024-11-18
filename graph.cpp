#include <graph.h>

int Graph::get_num_vertices() {
    return num_vertices;
}

int Graph::get_num_edges() {
    return num_edges;
}

Edge* Graph::get_vertex_edges(int vertex) {
    return vertices[vertex].edges;
}