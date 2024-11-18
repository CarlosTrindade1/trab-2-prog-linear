#include <graph.h>
#include <glpk.h>
#include <string>

int main(int argc, char **argv) {
    char *filename = argv[1];
    int s = atoi(argv[2]);
    int t = atoi(argv[3]);

    glp_prob *lp;
    Graph graph = Graph(filename);

    int num_vertices = graph.get_num_vertices();
    int num_edges = graph.get_num_edges();

    lp = glp_create_prob();
    glp_set_prob_name(lp, "Caminho mínimo");
    glp_set_obj_dir(lp, GLP_MIN);

    /*
        Configuração das restrições
    */

    glp_add_rows(lp, num_vertices);

    for (int i = 0; i < num_vertices; i++) {
        if (i == s) {
            glp_set_row_name(lp, i+1, "Vértice s");
            glp_set_row_bnds(lp, i+1, GLP_FX, 1.0, 1.0);
        } else if (i == t) {
            glp_set_row_name(lp, i+1, "Vértice t");
            glp_set_row_bnds(lp, i+1, GLP_FX, -1.0, -1.0);
        } else {
            char name[20];

            name[0] = '\0';
            sprintf(name, "Vértice %d", i+1);

            glp_set_row_name(lp, i+1, name);
            glp_set_row_bnds(lp, i+1, GLP_FX, 0.0, 0.0);
        }
    }

    /*
        Configuração das variáveis
    */
    glp_add_cols(lp, num_edges);

    int counter = 0;

    for (int i = 0; i < num_vertices; i++) {
        Edge *edges = graph.get_vertex_edges(i);

        while (edges) {
            char name[5];
            name[0] = '\0';
            sprintf(name, "x%d%d", i, edges->id);

            glp_set_col_name(lp, counter+1, name);
            glp_set_col_bnds(lp, counter+1, GLP_DB, 0.0, 1.0);
            glp_set_obj_coef(lp, counter+1, edges->cost);

            cout << edges->cost << " " << name << endl;

            edges = edges->next;
            counter++;
        }
    }

    return 0;
}