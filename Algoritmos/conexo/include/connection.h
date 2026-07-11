#ifndef CONNECTION_H
#define CONNECTION_H
#include <stdlib.h>
#include <string>

class Connection {
    public:
        Connection(std::string h_vertice, std::string t_vertice, int weight=0, bool is_directed=false){
            this->h_vertice = h_vertice;
            this->t_vertice = t_vertice;
            this->weight = weight;
            this->is_directed = is_directed;
        }
        int getWeight() { return weight; };
        bool isDirected() { return is_directed; };
        std::string getHead() { return h_vertice; };
        std::string getTail() { return t_vertice; };
        
    private:
        std::string h_vertice;
        std::string t_vertice;
        int weight=0;
        bool is_directed = false;
};

#endif