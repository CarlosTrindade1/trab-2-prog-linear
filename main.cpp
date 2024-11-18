#include <graph.h>
#include <glpk.h>
#include <string>
#include <vector>

struct Variable {
    int id;
    string name;
    int origin;
    int destiny;
};

/*
    @params:
    @description:
    @return:
*/
int get_id_variable_by_name(string name, Variable *variables, int num_variables) {
    for (int i = 0; i < num_variables; i++) {
        if (variables[i].name == name) {
            return variables[i].id;
        }
    }

    return -1;
}

/*
    @params:
    @description:
    @return:
*/
Variable get_variable_by_origin(int origin, Variable *variables, int num_variables) {
    for (int i = 0; i < num_variables; i++) {
        if (variables[i].origin == origin) {
            return variables[i];
        }
    }

    return Variable();
}

int main(int argc, char **argv) {
    char *filename = argv[1];
    int s = atoi(argv[2]);
    int t = atoi(argv[3]);

    glp_prob *lp;
    Graph graph = Graph(filename);
    int *ia, *ja;
    double *ar;
    vector<Variable> variables_path;

    int num_vertices = graph.get_num_vertices();
    int num_edges = graph.get_num_edges();

    ia = (int *) malloc(sizeof(int) * num_edges * num_vertices + 1);
    ja = (int *) malloc(sizeof(int) * num_edges * num_vertices + 1);
    ar = (double *) malloc(sizeof(double) * num_edges * num_vertices + 1);
    Variable variables[num_edges];

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

            variables[counter].id = counter+1;
            variables[counter].name = name;
            variables[counter].origin = i;
            variables[counter].destiny = edges->id;

            glp_set_col_name(lp, counter+1, name);
            glp_set_col_bnds(lp, counter+1, GLP_DB, 0.0, 1.0);
            glp_set_obj_coef(lp, counter+1, edges->cost);

            edges = edges->next;
            counter++;
        }
    }

    int nz = 1;

    for (int i = 0; i < num_vertices; i++) {
        Edge *edges = graph.get_vertex_edges(i);

        while (edges) {
            ia[nz] = i+1;
            ja[nz] = get_id_variable_by_name("x"+to_string(i)+to_string(edges->id), variables, num_edges);
            ar[nz] = 1.0;

            edges = edges->next;
            nz++;
        }

        for (int j = 0; j < num_vertices; j++) {
            Edge *edges = graph.get_vertex_edges(j);

            while (edges) {
                if (edges->id == i) {
                    ia[nz] = i+1;
                    ja[nz] = get_id_variable_by_name("x"+to_string(j)+to_string(edges->id), variables, num_edges);
                    ar[nz] = -1.0;

                    nz++;
                }

                edges = edges->next;
            }
        }
    }

    glp_smcp param_lp;

    glp_load_matrix(lp, nz - 1, ia, ja, ar);

    glp_term_out(GLP_OFF);

    glp_init_smcp(&param_lp);

    param_lp.msg_lev = GLP_MSG_OFF; 

    glp_simplex(lp, &param_lp);

    double z = glp_get_obj_val(lp);
    int status = glp_get_status(lp);

    if (status == GLP_OPT) {
        printf("C %.3f\n", z);

        for (int i = 0; i < num_edges; i++) {
            double x = glp_get_col_prim(lp, i+1);

            if (x > 0.0) {
                variables_path.push_back(variables[i]);
                printf("V %d %d %.3f\n", variables[i].origin, variables[i].destiny, x);
            }
        }

        vector<Variable> path;

        for (int i = 0; i < variables_path.size(); i++) {
            if (variables_path[i].origin == s) {
                path.push_back(variables_path[i]);
                variables_path.erase(variables_path.begin() + i);
            }
        }

        while (variables_path.size()) {
            int dest = path.back().destiny;

            for (int i = 0; i < variables_path.size(); i++) {
                if (variables_path[i].origin == dest) {
                    path.push_back(variables_path[i]);
                    variables_path.erase(variables_path.begin() + i);
                }
            }
        }

        cout << "P ";

        for (int i = 0; i < path.size(); i++) {
            cout << path[i].origin << " ";
        }

        cout << path.back().destiny << endl;

        return 0;
    }

    cout << "I" << endl;

    return 0;
}