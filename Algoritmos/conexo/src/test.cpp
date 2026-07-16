#include <cstdio>
#include <stdlib.h>
#include <string>

#include "../../../Grafo/include/connection.h"

#include "../../../Grafo/include/graph.h"
#include <utility>


int main() {
    List<Connection*> connections_id;
    
    List<std::string> vertices_id({"a", "b", "c", "d", "e", "f", "g"});
    List<std::pair<std::string, std::string>> connections({
            {"a", "b"},
            {"a", "e"},
            {"b", "c"},
            {"c", "d"},
            {"d", "e"},
            {"c", "f"},
            {"f", "g"}
        });
    
    for(int i = 0; i < connections.get_size(); i++) {
        auto pair = connections.get(i);
        connections_id.push_back(new Connection(pair.first, pair.second));
    }

    printf("Matriz de adjacência:\n");
    Graph graph(vertices_id, connections_id,true);
    graph.printAdjacencyMatrix();
    printf("Fecho transitivo:\n");
    graph.printFechoGrafo();

    printf("O grafo é conexo? %s\n", graph.ehConexo() ? "Sim" : "Não");
    
    printf("\n");
    
}