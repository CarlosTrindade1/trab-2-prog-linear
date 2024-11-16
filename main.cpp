#include <graph.h>

int main(int argc, char **argv) {
    char *filename = argv[1];

    Graph graph = Graph(filename);

    graph.print();
 
    return 0;
}