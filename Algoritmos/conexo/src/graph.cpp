#include "../include/graph.h"

List<vertice*> Graph::fecho_transitivo_OVE(vertice* v)
{
    List<vertice*> visited;
    std::unordered_set<vertice*> visited_set;
    aux_fecho(v, &visited, &visited_set);
    return visited;
};

void Graph::aux_fecho(vertice* v, List<vertice*> *visited, std::unordered_set<vertice*>* visited_set)
{
    if(visited_set->count(v)) return;
    
    visited->push_back(v);
    visited_set->insert(v);
    List<vertice*> adjacents = v->getAdjacents();
    for(int i=0; i<adjacents.get_size(); i++)
    {   
        vertice *adj = adjacents.get(i);
        if(!visited_set->count(adj))
            aux_fecho(adj, visited, visited_set);    
    }
};

bool Graph::ehConexo() {
    List<vertice*> visited;
    std::unordered_set<vertice*> visited_set;
    //como o grafo é não orientado, ou simétrico só precisa checar um vértice qualquer
    aux_fecho(vertices[vertices_id[0]], &visited, &visited_set);
    return visited.get_size() == num_vertices;
}