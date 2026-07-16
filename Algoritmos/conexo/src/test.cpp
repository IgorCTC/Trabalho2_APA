#include <cstdio>
#include <stdlib.h>
#include <string>

#include "../include/list.h"
#include "../include/graph.h"

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
    printf("Fecho transitivo:\n");



    for(int i=0; i<vertices_id.get_size(); i++)
    {
        vertice *v = graph.getVertices().get(i);
        List<vertice*> fecho = graph.AuxFecho(v);
        printf("%s ->", vertices_id.get(i).c_str());        
        for(int j=0; j<fecho.get_size(); j++)
            printf("%s ", fecho.get(j)->getName().c_str());
        printf("\n");
    }
    
    printf("\n");
    
}