#include <cstdio>
#include <stdlib.h>
#include <string>

#include "../../verifica_conexo/Grafo/include/graph.h"
#include "../include/apagador.h"

int main() {
    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections;

    Graph graph;
    graph.readGraphFromFile("../../instances/g1.txt", vertices_id, connections, false);
    graph.initialize_graph(vertices_id, connections, false);

    bool resultado = apagador(graph, false); 

    return 0;
}