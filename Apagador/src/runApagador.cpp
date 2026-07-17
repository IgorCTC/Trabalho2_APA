#include <cstdio>
#include <stdlib.h>
#include <string>
#include <chrono>

#include "../../verifica_conexo/Grafo/include/graph.h"
#include "../include/apagador.h"

int main(int argc, char** argv) {
    const std::string instancePath = (argc > 1) ? argv[1] : "../../instances/g1.txt";

    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections;

    Graph graph;
    graph.readGraphFromFile(instancePath, vertices_id, connections, false);
    graph.initialize_graph(vertices_id, connections, false);

    const auto start = std::chrono::steady_clock::now();
    bool resultado = apagador(graph, false); 
    const auto end = std::chrono::steady_clock::now();
    const auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::printf("runApagador,%s,%lld,%d\n", instancePath.c_str(), static_cast<long long>(elapsed_ns), resultado ? 1 : 0);

    return 0;
}