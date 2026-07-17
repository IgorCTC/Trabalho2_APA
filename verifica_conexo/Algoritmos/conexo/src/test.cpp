#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#include "../../../Grafo/include/connection.h"
#include "../../../Grafo/include/graph.h"

void readGraphFromFile(const std::string& filename,std::vector<std::string>& vertices,std::vector<Connection*>& connections, bool is_directed=false) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo %s\n", filename.c_str());
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        
        if (line[start] == 'X') {
            size_t begin = line.find('{');
            size_t end   = line.find('}');
            if (begin == std::string::npos || end == std::string::npos) continue;
            std::string content = line.substr(begin + 1, end - begin - 1);
            std::stringstream ss(content);
            std::string token;
            
            while (std::getline(ss, token, ',')) {
                size_t first = token.find_first_not_of(" \t");
                size_t last  = token.find_last_not_of(" \t");
                if (first != std::string::npos && last != std::string::npos) {
                    vertices.push_back(token.substr(first, last - first + 1));
                }
            }
        }
        else if (line[start] == 'U') {
            size_t begin = line.find('{');
            size_t end   = line.find('}');
            if (begin == std::string::npos || end == std::string::npos) continue;
            std::string content = line.substr(begin + 1, end - begin - 1);
            size_t pos = 0;
            while (pos < content.length()) {
                size_t open = content.find('(', pos);
                if (open == std::string::npos) break;
                size_t close = content.find(')', open);
                
                if (close == std::string::npos) break;
                std::string pairStr = content.substr(open + 1, close - open - 1);
                size_t comma = pairStr.find(',');
                if (comma == std::string::npos) { pos = close + 1; continue; }
                std::string v1 = pairStr.substr(0, comma);
                std::string v2 = pairStr.substr(comma + 1);
                
                auto trim = [](std::string& s) {
                    size_t f = s.find_first_not_of(" \t");
                    size_t l = s.find_last_not_of(" \t");
                    if (f != std::string::npos && l != std::string::npos)
                        s = s.substr(f, l - f + 1);
                };
                trim(v1);
                trim(v2);

                connections.push_back(new Connection(v1, v2, 0, is_directed));
                pos = close + 1;
            }
        }
    }
    file.close();
}


int main(){
    std::vector<std::string> vertices_id;
    std::vector<Connection*> connections_id;
    
    readGraphFromFile("../instances/g1.txt", vertices_id, connections_id);
    
    printf("Matriz de adjacência:\n");
    Graph graph(vertices_id, connections_id, false);
    graph.printAdjacencyMatrix();
    printf("Fecho transitivo:\n");
    graph.printFechoGrafo();

}