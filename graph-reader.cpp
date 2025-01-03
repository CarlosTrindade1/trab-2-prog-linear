#include <graph.h>

Graph::Graph(char *filename) {
    read_graph(filename);
}

/*
    @params: char* filename - Nome do arquivo que contém o grafo
    @description: Este método realiza a leitura do grafo, através do arquivo, e
    o armazena em uma lista de adjacências.
    @return: void
*/
void Graph::read_graph(char *filename) {
    ifstream file(filename);
    string line;
    int counter_vertices = 0;
    int counter_edges = 0;

    if (!file.is_open()) {
        cout << "Erro ao abrir" << endl;
        exit(1);
    }

    while (getline(file, line)) {
        if (line[0] == 'P') {
            sscanf(line.c_str(), "P %d %d", &num_vertices, &num_edges);

            vertices = new Vertex[num_vertices];
        } else if (line[0] == 'A') {
            int vertice, edge_id, cost;

            counter_edges++;

            sscanf(line.c_str(), "A %d %d %d", &vertice, &edge_id, &cost);

            if (vertices[vertice].edges) {
                Edge *edge = vertices[vertice].edges;

                while (edge->next) {
                    edge = edge->next;
                }
                edge->next = new Edge{edge_id, cost, NULL};
            } else {
                vertices[vertice].edges = new Edge{edge_id, cost, NULL};
            }
        } else if (line[0] == 'T') {
            if (counter_edges != num_edges) {
                cout << "E" << endl;
                exit(1);
            }
        }
    }

    file.close();
}