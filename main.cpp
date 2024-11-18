#include <graph.h>
#include <glpk.h>

int main(int argc, char **argv) {
    char *filename = argv[1];
    int s = atoi(argv[2]);
    int t = atoi(argv[3]);

    glp_prob *lp;
    Graph graph = Graph(filename);

    int num_vertices = graph.get_num_vertices();

    graph.print(); 

    lp = glp_create_prob();
    glp_set_prob_name(lp, "Caminho mínimo");
    glp_set_obj_dir(lp, GLP_MIN);

    glp_add_rows(lp, num_vertices);

    for (int i = 0; i < num_vertices; i++) {
        if (i == s) {
            glp_set_row_name(lp, i+1, "[COLOCAR O NAME]");
        } else if (i == t) {
            // TODO: preencher a restrição do t
        } else {
            // TODO: preencher as restrições para os demais vértices
        }
    }



    return 0;
}