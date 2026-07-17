#ifndef VERTICE_H
#define VERTICE_H

#include <vector>
#include <string>
#include <cstring>

class vertice{
public:
    vertice(std::string name) : name(name) {}
    std::string getName() { return name; }
    std::vector<vertice*> getAdjacents() { return adjacents; }
    void setName(std::string name) { this->name = name; }
    
    void addAdjacent(vertice* v) {
        for(int i = 0; i < adjacents.size(); i++)
            if(adjacents[i] == v) return;
        
        adjacents.push_back(v); 
    }
    
private:
    std::string name;
    std::vector<vertice*> adjacents;
};

#endif