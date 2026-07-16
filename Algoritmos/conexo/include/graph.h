#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"
#include "connection.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include "vertice.h"

class Graph {
public:
    Graph(List<std::string>& vertices_id, const List<Connection*>& connections) : num_vertices(vertices_id.get_size()), connections(connections) {
        fillAdjacents(vertices_id);
    }
    
    void fillAdjacents(List<std::string> vertices_id){
        for(int i = 0; i < num_vertices; i++)
        {
            this->vertices.push_back(new vertice(vertices_id.get(i)));
            for(int j = 0; j < connections.get_size(); j++)
            {
                if(strcmp(connections.get(j)->getHead()->getName().c_str(), this->vertices.back()->getName().c_str()) == 0)
                    this->vertices.back()->addAdjacent(connections.get(j)->getTail());                                        
                else if(strcmp(connections.get(j)->getTail()->getName().c_str(), this->vertices.back()->getName().c_str()) == 0)
                    this->vertices.back()->addAdjacent(connections.get(j)->getHead());                    
            }
        }
    }
    
    void printAdjacencyMatrix() {
        List<vertice*> vertices = getVertices();

        printf("   ");
        for(int i=0;i<num_vertices;i++)
            printf("%s  ", vertices.get(i)->getName().c_str());
        printf("\n");
        
        for(int i=0;i<num_vertices;i++)
        {
            List<vertice*> adjacents = vertices.get(i)->getAdjacents();
            printf("%s  ", vertices.get(i)->getName().c_str());
            
            for(int j=0; j<num_vertices; j++)
            {
                bool isAdjacent = false;
                for(int k=0; k<adjacents.get_size(); k++)
                    if(strcmp(adjacents.get(k)->getName().c_str(), vertices.get(j)->getName().c_str()) == 0)
                        isAdjacent = true;
                
                if(isAdjacent)
                    printf("1  ");
                else
                    printf("0  ");
            }
            printf("\n");
        }
    }

    bool inList(vertice *val, List<vertice*> *list){
        for(int i=0; i<list->get_size(); i++)
        {
            printf("comp: %s, %s = %b\n",val->getName().c_str(),list->get(i)->getName().c_str(),(strcmp(val->getName().c_str(), list->get(i)->getName().c_str())==0));
            if(strcmp(val->getName().c_str(), list->get(i)->getName().c_str())== 0)
                return true;
        }

        return false;
    };

    List<vertice*> AuxFecho(vertice *v){
        List<vertice*> fecho;
        fechoTransitivo(v, &fecho);
        return fecho;
    };
    
    void fechoTransitivo(vertice *v, List<vertice*> *visited) { 
        if(inList(v, visited))
        {
            printf("Já na Lista: %s", v->getName().c_str());
            return;            
        }    

        //printf("Loop no %s\n", v->getName().c_str());
        
        visited->push_back(v);
        
        List<vertice*> adjacents = v->getAdjacents();
        for(int i=0; i<adjacents.get_size();i++)
        {   
            vertice* at = adjacents.get(i);
            if(!inList(at, visited)){
                //printf("Buscando %s, tam adj: %d\n", v->getName().c_str(), adjacents.get_size());
                //printf("Adjacente interno: %s\n", at->getName().c_str());
                fechoTransitivo(at, visited);             
            }
        }
    };
    
    List<vertice*> getVertices() { return vertices; }    
    List<Connection*> getConnections() { return connections; }
    int getNumVertices() { return num_vertices; }
private:
    List<Connection*> connections;
    List<vertice*> vertices;
    int num_vertices;
};

#endif // GRAPH_H
