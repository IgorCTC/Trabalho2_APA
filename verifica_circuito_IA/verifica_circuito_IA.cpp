#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;

bool lerGrafo(const string& nomeArquivo,
              vector<vector<int>>& grafo,
              unordered_map<string, int>& indice) {

    ifstream arq(nomeArquivo);
    if (!arq.is_open())
        return false;

    string linha;

    // ---------- Vértices ----------
    getline(arq, linha);

    size_t ini = linha.find('{');
    size_t fim = linha.find('}');

    string vertices = linha.substr(ini + 1, fim - ini - 1);

    stringstream ss(vertices);
    string vertice;
    int id = 0;

    while (getline(ss, vertice, ',')) {
        indice[vertice] = id++;
    }

    grafo.resize(indice.size());

    // ---------- Arcos ----------
    getline(arq, linha);

    ini = linha.find('{');
    fim = linha.find('}');

    string arcos = linha.substr(ini + 1, fim - ini - 1);

    while (true) {

        size_t abre = arcos.find('(');

        if (abre == string::npos)
            break;

        size_t virg = arcos.find(',', abre);
        size_t fecha = arcos.find(')', virg);

        string origem = arcos.substr(abre + 1, virg - abre - 1);
        string destino = arcos.substr(virg + 1, fecha - virg - 1);

        grafo[indice[origem]].push_back(indice[destino]);

        arcos.erase(0, fecha + 1);
    }

    return true;
}

bool dfs(int v,
         const vector<vector<int>>& grafo,
         vector<bool>& visitado,
         vector<bool>& pilha) {

    visitado[v] = true;
    pilha[v] = true;

    for (int u : grafo[v]) {

        if (!visitado[u]) {
            if (dfs(u, grafo, visitado, pilha))
                return true;
        }
        else if (pilha[u]) {
            return true;
        }
    }

    pilha[v] = false;

    return false;
}

bool possuiCircuito(const vector<vector<int>>& grafo) {

    int n = grafo.size();

    vector<bool> visitado(n, false);
    vector<bool> pilha(n, false);

    for (int i = 0; i < n; i++) {

        if (!visitado[i]) {

            if (dfs(i, grafo, visitado, pilha))
                return true;
        }
    }

    return false;
}


int main(int argc, char** argv) {
    const string instancePath = (argc > 1) ? argv[1] : "../../instances/g1.txt";

    vector<vector<int>> grafo;
    unordered_map<string, int> indice;

    if (!lerGrafo(instancePath, grafo, indice)) {
        cout << "Erro ao abrir o arquivo.\n";
        return 1;
    }

    const auto start = chrono::steady_clock::now();
    bool resultado = possuiCircuito(grafo);
    const auto end = chrono::steady_clock::now();
    const auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "verifica_circuito_IA," << instancePath << "," << static_cast<long long>(elapsed_ns) << "," << (resultado ? 1 : 0) << '\n';

    return 0;
}