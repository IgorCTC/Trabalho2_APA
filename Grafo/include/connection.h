#ifndef CONNECTION_H
#define CONNECTION_H

#include "vertice.h"
#include <stdlib.h>

#include <initializer_list>
#include <string>
#include <unordered_map>

class Connection {
    public:
        Connection(std::string h_vertice, std::string t_vertice, int weight=0, bool is_directed=false){
            this->h_vertice = new vertice(h_vertice);
            this->t_vertice = new vertice(t_vertice);
            this->weight = weight;
            this->is_directed = is_directed;
        }

        int getWeight() { return weight; };
        bool isDirected() { return is_directed; };
        vertice* getHead() { return h_vertice; };
        vertice* getTail() { return t_vertice; };
        
        std::string getHead_id() { return h_vertice->getName(); };
        std::string getTail_id() { return t_vertice->getName(); };
    private:
        vertice* h_vertice;
        vertice* t_vertice;
        int weight=0;
        bool is_directed = false;
};

#endif