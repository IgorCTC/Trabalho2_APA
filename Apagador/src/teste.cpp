#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <string>

#include "../../verifica_conexo/Lista/include/list.h"
#include "../../verifica_conexo/Grafo/include/graph.h"
#include "../include/apagador.h"

int main() {
    List<std::string> vertices_id;
    List<Connection*> connections;

    vertices_id.push_back("a");
    vertices_id.push_back("b");
    vertices_id.push_back("c");
    vertices_id.push_back("d");

    connections.push_back(new Connection("a", "b"));
    connections.push_back(new Connection("b", "c"));
    connections.push_back(new Connection("c", "d"));
    connections.push_back(new Connection("d", "a"));

    printf("Matriz de adjacência:\n");
    Graph graph(vertices_id, connections);
    graph.printAdjacencyMatrix();

    printf("\n\nTeste do algoritmo apagador:\n");
    std::string resultado = apagador(graph, false); 
    printf("Resultado: %s\n", resultado.c_str());

    return 0;
}