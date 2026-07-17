#include <cstdio>
#include <stdlib.h>
#include <string>

#include "../../verifica_conexo/Grafo/include/graph.h"
#include "../include/apagador.h"

int main() {
    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections;

    Graph graph;
    graph.readGraphFromFile("../instances/g1.txt", vertices_id, connections, false);
    graph.initialize_graph(vertices_id, connections, false);

    printf("Matriz de adjacência:\n");
    graph.printAdjacencyMatrix();

    
    printf("\n\nTeste do algoritmo apagador:\n");
    std::string resultado = apagador(graph, false); 
    printf("Resultado: %s\n", resultado.c_str());

    return 0;
}