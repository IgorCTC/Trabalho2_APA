#include <iostream>
#include "Grafo.h"

using namespace std;

int main() {
    // Cenário de Teste 1: Grafo contendo circuito (1 -> 2 -> 3 -> 1)
    cout << "=== Teste 1: Grafo com Circuito ===" << endl;
    Grafo grafo1("../instances/g1.txt");
    grafo1.buscarCircuito();

    cout << "\n";

    // Cenário de Teste 2: Grafo limpo (Sem nenhum circuito)
    cout << "=== Teste 2: Grafo sem Circuito ===" << endl;
    string v2 = "X = {x1,x2,x3,x4}";
    string a2 = "U = {(x1,x2),(x1,x3),(x2,x4),(x3,x4)}";
    Grafo grafo2(v2, a2);
    grafo2.buscarCircuito();

    cout << "\n";

    string v3 = "X = {x1,x2,x3,x4,x5,x6}";
    string a3 = "U = {(x1,x2),(x2,x3),(x3,x1),(x4,x5),(x5,x6),(x2,x4)}";
    Grafo grafo3(v3, a3);
    grafo3.buscarCircuito();

    return 0;
}