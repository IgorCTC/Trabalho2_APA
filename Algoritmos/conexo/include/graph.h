#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"
#include "connection.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


class vertice{
public:
    vertice(string name) : name(name) {}
    string getName() { return name; }
    List<string> getAdjacents() { return adjacents; }
    void setName(string name) { this->name = name; }

    void addAdjacent(string v) {
        for(int i = 0; i < adjacents.get_size(); i++)
            if(strcmp(adjacents.get(i).c_str(), v.c_str()) == 0) return;
        adjacents.push_back(v); 
    }
    
private:
    string name;
    List<string> adjacents;
};

class Graph {
public:
    Graph(List<string> vertices_id, List<Connection*> connections) : num_vertices(vertices_id.get_size()), connections(connections) {
        adjcentMatrix(vertices_id);
    }

    void adjcentMatrix(List<string> vertices_id){
        for(int i = 0; i < num_vertices; i++)
        {
            this->vertices.push_back(new vertice(vertices_id.get(i)));
            for(int j = 0; j < connections.get_size(); j++)
            {
                if(strcmp(connections.get(j)->getHead().c_str(), this->vertices.back()->getName().c_str()) == 0)
                {
                    this->vertices.back()->addAdjacent(connections.get(j)->getTail());                    
                }
                else if(strcmp(connections.get(j)->getTail().c_str(), this->vertices.back()->getName().c_str()) == 0)
                {
                    this->vertices.back()->addAdjacent(connections.get(j)->getHead());
                }
            }
        }
    }

    void printAdjacencyMatrix() {
        List<vertice*> vertices = getVertices();
        
        for(int i=0;i<num_vertices;i++)
        {
            List<string> adjacents = vertices.get(i)->getAdjacents();
            for(int j=0; j<num_vertices; j++)
            {
                bool isAdjacent = false;
                for(int k=0; k<adjacents.get_size(); k++)
                    if(strcmp(adjacents.get(k).c_str(), vertices.get(j)->getName().c_str()) == 0)
                        isAdjacent = true;
                
                if(isAdjacent)
                    printf("1 ");
                else
                    printf("0 ");
            }
            printf("\n");
        }
    }

        vector<vector<int>> getAdjacencyMatrix() {
        List<vertice*> vertices = getVertices();
        vector<vector<int>> A(num_vertices, vector<int>(num_vertices, 0));

        for(int i=0;i<num_vertices;i++)
        {
            List<string> adjacents = vertices.get(i)->getAdjacents();
            for(int j=0; j<num_vertices; j++)
            {
                bool isAdjacent = false;
                for(int k=0; k<adjacents.get_size(); k++)
                    if(strcmp(adjacents.get(k).c_str(), vertices.get(j)->getName().c_str()) == 0)
                        isAdjacent = true;

                A[i][j] = isAdjacent ? 1 : 0;
            }
        }
        return A;
    }
    
    List<vertice*> getVertices() { return vertices; }    
    List<Connection*> getConnections() { return connections; }
    int getNumVertices() { return num_vertices; }
    
private:
    List<Connection*> connections;
    List<vertice*> vertices;
    int num_vertices;
};

#endif // GRAPH_H
