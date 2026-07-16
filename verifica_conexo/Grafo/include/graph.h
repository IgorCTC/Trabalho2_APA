#ifndef GRAPH_H
#define GRAPH_H

#include "../../Lista/include/list.h"
#include "connection.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include "vertice.h"

#include <unordered_set>
#include <unordered_map>

class Graph {
public:
    Graph(List<std::string>& vertices_id, const List<Connection*>& connections, bool simetric=false) : num_vertices(vertices_id.get_size()), connections(connections), simetric(simetric) {
        fillAdjacents(vertices_id);
    }
    
    void fillAdjacents(List<std::string> vertices_id){
        for(int i = 0; i < num_vertices; i++)
        {
            this->vertices[vertices_id.get(i)] = new vertice(vertices_id.get(i));
            this->vertices_id[i] = vertices_id.get(i);
        }
            
        for(int i = 0; i < num_vertices; i++)
        {
            for(int j = 0; j < connections.get_size(); j++)
            {
                vertice* head = connections.get(j)->getHead();
                vertice* tail = connections.get(j)->getTail();

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
            List<vertice*> adjacents = vertices[vertices_id[i]]->getAdjacents();
            printf("%s  ", vertices_id[i].c_str());
            
            for(int j=0; j<num_vertices; j++)
            {
                bool isAdjacent = false;
                for(int k=0; k<adjacents.get_size(); k++)
                    if(adjacents.get(k)->getName() == vertices_id[j])
                        isAdjacent = true;
                
                if(isAdjacent)
                    printf("1  ");
                else
                    printf("0  ");
            }
            printf("\n");
        }
    }

    List<vertice*> fecho_transitivo_OVE(vertice* v);

    void aux_fecho(vertice* v, List<vertice*> *visited, std::unordered_set<vertice*>* visited_set);

    void printFechoGrafo(){
        for(int i=0; i<vertices_id.size(); i++)
        {
            vertice *v = vertices[vertices_id[i]];        
            List<vertice*> fecho = fecho_transitivo_OVE(v);
            printf("%s ->", vertices_id[i].c_str());       
            
            for(int j=0; j<fecho.get_size(); j++)
                printf("%s ", fecho.get(j)->getName().c_str());
            printf("\n");
        }
    }

    bool ehConexo();
    
    std::unordered_map<std::string, vertice*> getVertices() { return vertices; }
    std::unordered_map<int, std::string> getVertices_id() { return vertices_id; }    
    
    List<Connection*> getConnections() { return connections; }
    int getNumVertices() { return num_vertices; }
    
private:
    List<Connection*> connections;
    std::unordered_map<int,std::string> vertices_id;
    std::unordered_map<std::string, vertice*> vertices;
    bool simetric;
    int num_vertices;
};

#endif // GRAPH_H
