#include "../../../Grafo/include/graph.h"


std::vector<vertice*> Graph::fecho_transitivo_OVE(vertice* v)
{
    std::vector<vertice*> visited;
    std::unordered_set<vertice*> visited_set;
    aux_fecho(v, &visited, &visited_set);
    return visited;
};

void Graph::aux_fecho(vertice* v, std::vector<vertice*> *visited, std::unordered_set<vertice*>* visited_set)
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

bool Graph::ehConexo() {
    std::vector<vertice*> visited;
    std::unordered_set<vertice*> visited_set;
    //como o grafo é não orientado, ou simétrico só precisa checar um vértice qualquer
    aux_fecho(vertices[vertices_id[0]], &visited, &visited_set);
    return visited.size() == num_vertices;
}