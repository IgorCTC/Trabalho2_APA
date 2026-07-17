#ifndef GRAPH_H
#define GRAPH_H

#include "connection.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include "vertice.h"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>

class Graph {
public:
    Graph() : num_vertices(0), simetric(false) {}
    void initialize_graph(std::vector<std::string>& vertices_id, const std::vector<Connection*>& connections, bool simetric=false){
        this->num_vertices = vertices_id.size();
        this->connections = connections;
        this->simetric = simetric;
        fillAdjacents(vertices_id);
    }
    
    Graph(std::vector<std::string>& vertices_id, const std::vector<Connection*>& connections, bool simetric=false) : num_vertices(vertices_id.size()), connections(connections), simetric(simetric) {
        fillAdjacents(vertices_id);
    }
    
    void fillAdjacents(std::vector<std::string> vertices_id){
        for(int i = 0; i < num_vertices; i++)
        {
            this->vertices[vertices_id[i]] = new vertice(vertices_id[i]);
            this->vertices_id[i] = vertices_id[i];
        }
            
        for(int i = 0; i < num_vertices; i++)
        {
            for(int j = 0; j < connections.size(); j++)
            {
                vertice* head = connections[j]->getHead();
                vertice* tail = connections[j]->getTail();

                if(simetric)
                {
                    //tá orientado mas na prática não faz diferença nenhuma porque tá simétrico
                    this->vertices[head->getName()]->addAdjacent(this->vertices[tail->getName()]);
                    this->vertices[tail->getName()]->addAdjacent(this->vertices[head->getName()]);
                } 
                else {
                    vertice* v = this->vertices[this->vertices_id[i]];
                    
                    if(head->getName() == v->getName())    
                        v->addAdjacent(this->vertices[tail->getName()]);                    
                    else if(tail->getName() == v->getName())
                        v->addAdjacent(this->vertices[head->getName()]);                    
                }
            }
        }
    }
    
    void printAdjacencyMatrix() {
        printf("   ");
        for(int i=0;i<num_vertices;i++)
            printf("%s  ", vertices_id[i].c_str());
        printf("\n");
        
        for(int i=0;i<num_vertices;i++)
        {
            std::vector<vertice*> adjacents = vertices[vertices_id[i]]->getAdjacents();
            printf("%s  ", vertices_id[i].c_str());
            
            for(int j=0; j<num_vertices; j++)
            {
                bool isAdjacent = false;
                for(int k=0; k<adjacents.size(); k++)
                    if(adjacents[k]->getName() == vertices_id[j])
                        isAdjacent = true;
                
                if(isAdjacent)
                    printf("1  ");
                else
                    printf("0  ");
            }
            printf("\n");
        }
    }

    void printFechoGrafo(){
        for(int i=0; i<vertices_id.size(); i++)
        {
            vertice *v = vertices[vertices_id[i]];        
            std::vector<vertice*> fecho = fecho_transitivo_OVE(v);
            printf("%s ->", vertices_id[i].c_str());       
            
            for(int j=0; j<fecho.size(); j++)
                printf("%s ", fecho[j]->getName().c_str());
            printf("\n");
        }
    }

//==============================================================APAGADOR=========================================================================================
    std::vector<std::vector<int>> getAdjacencyMatrix() {
        std::vector<std::vector<int>> A(
            num_vertices,
            std::vector<int>(num_vertices, 0)
        );

        for (int i = 0; i < num_vertices; i++) {

            vertice* v = vertices[vertices_id[i]];

            std::vector<vertice*> adj = v->getAdjacents();

            for (int k = 0; k < adj.size(); k++) {

                std::string nome = adj[k]->getName();

                for (int j = 0; j < num_vertices; j++) {

                    if (vertices_id[j] == nome) {
                        A[i][j] = 1;
                        break;
                    }
                }
            }
        }
        return A;
    }

//===================================================================FECHO TRANSITIVO=====================================================================================

        std::vector<vertice*> fecho_transitivo_OVE(vertice* v)
        {
            std::vector<vertice*> visited;
            std::unordered_set<vertice*> visited_set;
            aux_fecho(v, &visited, &visited_set);
            return visited;
        };
        
        void aux_fecho(vertice* v, std::vector<vertice*> *visited, std::unordered_set<vertice*>* visited_set)
        {
            if(visited_set->count(v)) return;
            
            visited->push_back(v);
            visited_set->insert(v);
            std::vector<vertice*> adjacents = v->getAdjacents();
            for(int i=0; i<adjacents.size(); i++)
            {   
                vertice *adj = adjacents[i];
                if(!visited_set->count(adj))
                    aux_fecho(adj, visited, visited_set);    
            }
        };
        
        bool ehConexo() {
            std::vector<vertice*> visited;
            std::unordered_set<vertice*> visited_set;
            //como o grafo é não orientado, ou simétrico só precisa checar um vértice qualquer
            aux_fecho(vertices[vertices_id[0]], &visited, &visited_set);
            return visited.size() == num_vertices;
        }
//===================================================================LEITURA DE ARQUIVO=====================================================================================
    
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
//============================================================================================================================================================
        
    std::unordered_map<std::string, vertice*> getVertices() { return vertices; }
    std::unordered_map<int, std::string> getVertices_id() { return vertices_id; }    
    
    std::vector<Connection*> getConnections() { return connections; }
    int getNumVertices() { return num_vertices; }
    
private:
    std::vector<Connection*> connections;
    std::unordered_map<int,std::string> vertices_id;
    std::unordered_map<std::string, vertice*> vertices;
    bool simetric;
    int num_vertices;
};

#endif // GRAPH_H
