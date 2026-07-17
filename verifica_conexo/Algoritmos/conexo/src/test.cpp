#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>


#include "../../../Grafo/include/connection.h"
#include "../../../Grafo/include/graph.h"


int main(){
    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections_id;
    Graph graph1;
    graph1.readGraphFromFile("../instances/g1.txt", vertices_id, connections_id,false);    
    graph1.initialize_graph(vertices_id, connections_id, false);
    
    printf("Matriz de adjacência:\n");
    graph1.printAdjacencyMatrix();
    printf("Fecho transitivo:\n");
    graph1.printFechoGrafo();
}