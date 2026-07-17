#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>


#include "../../../Grafo/include/connection.h"
#include "../../../Grafo/include/graph.h"


int main(int argc, char** argv){
    const std::string instancePath = (argc > 1) ? argv[1] : "../../../../../instances/g1.txt";
    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections_id;
    Graph graph;
    graph.readGraphFromFile(instancePath, vertices_id, connections_id,false);    
    graph.initialize_graph(vertices_id, connections_id, true);
    const auto start = std::chrono::steady_clock::now();
    bool resultado = graph.ehConexo();
    const auto end = std::chrono::steady_clock::now();
    const auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::printf("runVerificaConexo,%s,%lld,%d\n", instancePath.c_str(), static_cast<long long>(elapsed_ns), resultado ? 1 : 0);
}