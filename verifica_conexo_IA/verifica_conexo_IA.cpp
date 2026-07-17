#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std;

void dfs(int v, const vector<vector<int>>& grafo, vector<bool>& visitado) {
    visitado[v] = true;

    for (int u : grafo[v]) {
        if (!visitado[u]) {
            dfs(u, grafo, visitado);
        }
    }
}

bool lerGrafo(const string& nomeArquivo,
              vector<vector<int>>& grafo,
              unordered_map<string, int>& indice) {

    ifstream arq(nomeArquivo);
    if (!arq.is_open())
        return false;

    string linha;

    // ---------- Lê os vértices ----------
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

    // ---------- Lê as arestas ----------
    getline(arq, linha);

    ini = linha.find('{');
    fim = linha.find('}');

    string arestas = linha.substr(ini + 1, fim - ini - 1);

    size_t pos = 0;

    while ((pos = arestas.find('(')) != string::npos) {
        size_t virg = arestas.find(',', pos);
        size_t fecha = arestas.find(')', virg);

        string v1 = arestas.substr(pos + 1, virg - pos - 1);
        string v2 = arestas.substr(virg + 1, fecha - virg - 1);

        int u = indice[v1];
        int v = indice[v2];

        // Grafo não direcionado
        grafo[u].push_back(v);
        grafo[v].push_back(u);

        arestas.erase(0, fecha + 1);
    }

    return true;
}

bool verificarConexo(const vector<vector<int>>& grafo) {
    int n = grafo.size();
    vector<bool> visitado(n, false);

    dfs(0, grafo, visitado);

    for (bool v : visitado) {
        if (!v)
            return false;
    }

    return true;
}

int main(int argc, char** argv) {
    const string instancePath = (argc > 1) ? argv[1] : "../../instances/g1.txt";
    vector<vector<int>> grafo;
    unordered_map<string, int> indice;

    if (!lerGrafo(instancePath, grafo, indice)) {
        cout << "Erro ao abrir arquivo.\n";
        return 1;
    }

    const auto start = chrono::steady_clock::now();
    bool resultado = verificarConexo(grafo);
    const auto end = chrono::steady_clock::now();
    const auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "verifica_conexo_IA," << instancePath << "," << static_cast<long long>(elapsed_ns) << "," << (resultado ? 1 : 0) << '\n';

    return 0;
}