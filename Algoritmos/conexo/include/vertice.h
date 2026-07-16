#ifndef VERTICE_H
#define VERTICE_H

#include <string>
#include <cstring>
#include "list.h"

class vertice{
public:
    vertice(std::string name) : name(name) {}
    std::string getName() { return name; }
    List<vertice*> getAdjacents() { return adjacents; }
    void setName(std::string name) { this->name = name; }
    
    void addAdjacent(vertice* v) {
        for(int i = 0; i < adjacents.get_size(); i++)
            if(adjacents.get(i) == v) return;
        
        adjacents.push_back(v); 
    }
    
private:
    std::string name;
    List<vertice*> adjacents;
};

#endif