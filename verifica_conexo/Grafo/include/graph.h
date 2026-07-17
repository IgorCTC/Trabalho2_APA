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

class Graph {
public:
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

    std::vector<vertice*> fecho_transitivo_OVE(vertice* v);

    void aux_fecho(vertice* v, std::vector<vertice*> *visited, std::unordered_set<vertice*>* visited_set);

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

    bool ehConexo();
    void readGraphFromFile(const std::string& filename,std::vector<std::string>& vertices,std::vector<Connection*>& connections);
    
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
