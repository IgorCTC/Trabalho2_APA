#ifndef APAGADOR_H
#define APAGADOR_H

#include <vector>
#include <string>
#include <iostream>
#include "graph.h"

using namespace std;

inline string apagador(const vector<vector<int>>& A, int n, bool verbose = false) {
    vector<int> V(n, 0);
    vector<bool> apagado(n, false);
    int totalApagados = 0;

    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
            V[j] += A[i][j];

    bool acabou = false;
    int passo = 0;

    while (!acabou) {
        acabou = true;
        ++passo;

        for (int i = 0; i < n; ++i) {
            if (!apagado[i] && V[i] == 0) {
                for (int j = 0; j < n; ++j)
                    if (!apagado[j] && V[j] != 0)
                        V[j] -= A[i][j];

                apagado[i] = true;
                ++totalApagados;
                acabou = false;
            }
        }

        if (verbose) {
            cout << "Passo " << passo << ": V = [";
            for (int k = 0; k < n; ++k) {
                cout << (apagado[k] ? -1 : V[k]);
                if (k < n - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
    }

    return (totalApagados == n) ? "fracasso" : "sucesso";
}

inline string apagador(Graph& g, bool verbose = false) {
    return apagador(g.getAdjacencyMatrix(), g.getNumVertices(), verbose);
}

#endif // APAGADOR_H