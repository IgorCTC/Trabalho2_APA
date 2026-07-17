#include <iostream>
#include <chrono>
#include "Grafo.h"

using namespace std;

int main(int argc, char** argv) {
    const string instancePath = (argc > 1) ? argv[1] : "../../instances/g1.txt";

    Grafo grafo1(instancePath);
    const auto start = chrono::steady_clock::now();
    bool resultado = grafo1.buscarCircuito();
    const auto end = chrono::steady_clock::now();
    const auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "runBuscaCircuito," << instancePath << "," << static_cast<long long>(elapsed_ns) << "," << (resultado ? 1 : 0) << '\n';

    return 0;
}