#include <iostream>
#include "Grafo.h"

using namespace std;

int main() {
    // Cenário de Teste 1: Grafo contendo circuito (1 -> 2 -> 3 -> 1)
    cout << "=== Teste 1: Grafo com Circuito ===" << endl;
    Grafo grafo1("../instances/g1.txt");
    grafo1.buscarCircuito();

    return 0;
}