#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class Grafo {
private:
    int totalVertices;
    
    // Lista de adjacência: representa as conexões de saída de cada vértice
    vector<list<int>> listaAdjacencia; 

    // Dicionário auxiliar para mapear nomes de vértices (ex: "x1") para índices (ex: 0)
    // e outro inverso para poder imprimir com os nomes originais depois.
    unordered_map<string, int> nomeParaIndice;
    vector<string> indiceParaNome;

    // Estados de controle do algoritmo
    vector<bool> foiVisitado;        // true se o vértice já foi visitado; false se nunca foi visitado
    vector<bool> estaNaPilhaAtiva;   // Controla se o vértice está na rota atual de exploração (evita ciclos)
    stack<int> caminhoExploracao;    // Pilha que guarda a rota exata que o explorador está percorrendo
    bool circuitoEncontrado;         // Sinalizador para interromper a busca assim que detectar um circuito

    // Procedimento recursivo de busca em profundidade (DFS)
    void explorarVertice(int verticeAtual) {
        // Marca o vértice atual como visitado e o adiciona ao caminho ativo
        foiVisitado[verticeAtual] = true;
        caminhoExploracao.push(verticeAtual);
        estaNaPilhaAtiva[verticeAtual] = true;

        // Explora todos os vizinhos (destinos) a partir do vértice atual
        for (int vizinho : listaAdjacencia[verticeAtual]) {
            // Se um circuito já foi detectado em outra ramificação, interrompe a busca imediatamente
            if (circuitoEncontrado) {
                return;
            }

            // Caso 1: O vizinho ainda não foi visitado
            if (!foiVisitado[vizinho]) {
                explorarVertice(vizinho);
            } 
            // Caso 2: O vizinho já foi visitado E está na pilha de exploração ativa.
            // Isso significa que acabamos de fechar um ciclo (circuito)!
            else if (estaNaPilhaAtiva[vizinho]) { 
                circuitoEncontrado = true;
                imprimirCircuitoDetectado(vizinho);
                return;
            }
        }

        // Se exploramos todos os caminhos a partir deste vértice e não encontramos circuito:
        // Voltamos atrás (backtracking), removendo-o da rota ativa de exploração.
        if (!circuitoEncontrado) {
            caminhoExploracao.pop();
            estaNaPilhaAtiva[verticeAtual] = false;
        }
    }

    // Recupera a rota de dentro da pilha e exibe o ciclo na tela
    void imprimirCircuitoDetectado(int verticeInicioCiclo) {
        vector<int> rotaInvertida;
        stack<int> pilhaTemporaria = caminhoExploracao;
        
        // Extrai o caminho da pilha até encontrar o ponto onde o ciclo se iniciou
        while (!pilhaTemporaria.empty()) {
            rotaInvertida.push_back(pilhaTemporaria.top());
            if (pilhaTemporaria.top() == verticeInicioCiclo) {
                break;
            }
            pilhaTemporaria.pop();
        }

        // Como extraímos da pilha (do topo para a base), invertemos para mostrar na ordem correta
        reverse(rotaInvertida.begin(), rotaInvertida.end());

        cout << "Circuito detectado: ";
        for (int vertice : rotaInvertida) {
            cout << indiceParaNome[vertice] << " -> ";
        }
        cout << indiceParaNome[verticeInicioCiclo] << endl; // Fecha o circuito ligando de volta ao início
    }

    // Método auxiliar para limpar espaços em branco de uma string
    string removerEspacos(const string& str) {
        string resultado = "";
        for (char c : str) {
            if (!isspace(c)) resultado += c;
        }
        return resultado;
    }

public:

    /**
     * Realiza a leitura e construção do grafo a partir de strings formatadas.
     * Exemplo de formato:
     * verticesStr = "X = {x1,x2,x3,x4,x5,x6}"
     * arcosStr    = "U = {(x1,x2),(x2,x3),(x3,x1),(x4,x5),(x5,x6),(x2,x4)}"
     */
    Grafo(string verticesStr, string arcosStr) {
        verticesStr = removerEspacos(verticesStr);
        arcosStr = removerEspacos(arcosStr);

        // 1. Extrair e mapear os vértices
        // Expressão regular para encontrar os nomes dos vértices dentro das chaves { }
        // Encontra qualquer sequência alfanumérica (ex: x1, x2)
        regex regexVertices(R"([a-zA-Z0-9]+)");
        
        // Encontra onde começam as chaves para ler apenas o conteúdo de dentro delas
        size_t inicioChaveVert = verticesStr.find('{');
        size_t fimChaveVert = verticesStr.find('}');
        string conteudoVertices = verticesStr.substr(inicioChaveVert + 1, fimChaveVert - inicioChaveVert - 1);

        auto verticesBegin = sregex_iterator(conteudoVertices.begin(), conteudoVertices.end(), regexVertices);
        auto verticesEnd = sregex_iterator();

        nomeParaIndice.clear();
        indiceParaNome.clear();
        int indiceAtual = 0;

        for (auto i = verticesBegin; i != verticesEnd; ++i) {
            string nomeVertice = i->str();
            if (nomeParaIndice.find(nomeVertice) == nomeParaIndice.end()) {
                nomeParaIndice[nomeVertice] = indiceAtual++;
                indiceParaNome.push_back(nomeVertice);
            }
        }

        // Configura o tamanho das estruturas internas do Grafo baseado no número de vértices lidos
        this->totalVertices = indiceAtual;
        this->listaAdjacencia.assign(totalVertices, list<int>());
        this->foiVisitado.assign(totalVertices, false);
        this->estaNaPilhaAtiva.assign(totalVertices, false);

        // 2. Extrair os arcos (arestas direcionadas)
        // Expressão regular para capturar pares no formato (origem,destino)
        regex regexArcos(R"(\(([^,]+),([^)]+)\))");
        
        size_t inicioChaveArcos = arcosStr.find('{');
        size_t fimChaveArcos = arcosStr.find('}');
        string conteudoArcos = arcosStr.substr(inicioChaveArcos + 1, fimChaveArcos - inicioChaveArcos - 1);

        auto arcosBegin = sregex_iterator(conteudoArcos.begin(), conteudoArcos.end(), regexArcos);
        auto arcosEnd = sregex_iterator();

        for (auto i = arcosBegin; i != arcosEnd; ++i) {
            smatch match = *i;
            string origem = match[1].str();
            string destino = match[2].str();

            // Se ambos os vértices do arco existem no mapeamento, adiciona a aresta
            if (nomeParaIndice.find(origem) != nomeParaIndice.end() && 
                nomeParaIndice.find(destino) != nomeParaIndice.end()) {
                adicionarAresta(nomeParaIndice[origem], nomeParaIndice[destino]);
            }
        }
    }

    /**
     * Lê o arquivo fornecido pelo caminho 'caminhoArquivo', processa os vértices na 
     * primeira linha e os arcos na segunda linha para construir a estrutura do grafo.
     */
    bool lerGrafoDeArquivo(const string& caminhoArquivo) {
        ifstream arquivo(caminhoArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao tentar abrir o arquivo: " << caminhoArquivo << endl;
            return false;
        }

        string linhaVertices, linhaArcos;
        
        // Lê a primeira linha (Vértices: X = {...})
        if (!getline(arquivo, linhaVertices)) {
            cerr << "Erro: Arquivo vazio ou sem a definição de vértices na primeira linha." << endl;
            return false;
        }

        // Lê a segunda linha (Arcos: U = {...})
        if (!getline(arquivo, linhaArcos)) {
            cerr << "Erro: O arquivo não contém a definição de arcos na segunda linha." << endl;
            return false;
        }

        arquivo.close();

        // Remove espaços e caracteres de controle invisíveis
        linhaVertices = removerEspacos(linhaVertices);
        linhaArcos = removerEspacos(linhaArcos);

        // 1. Extração dos Vértices
        regex regexVertices(R"([a-zA-Z0-9]+)");
        size_t inicioChaveVert = linhaVertices.find('{');
        size_t fimChaveVert = linhaVertices.find('}');
        
        if (inicioChaveVert == string::npos || fimChaveVert == string::npos) {
            cerr << "Erro: Formato inválido na linha de vértices (chaves { } não encontradas)." << endl;
            return false;
        }
        
        string conteudoVertices = linhaVertices.substr(inicioChaveVert + 1, fimChaveVert - inicioChaveVert - 1);

        auto verticesBegin = sregex_iterator(conteudoVertices.begin(), conteudoVertices.end(), regexVertices);
        auto verticesEnd = sregex_iterator();

        nomeParaIndice.clear();
        indiceParaNome.clear();
        int indiceAtual = 0;

        for (auto i = verticesBegin; i != verticesEnd; ++i) {
            string nomeVertice = i->str();
            if (nomeParaIndice.find(nomeVertice) == nomeParaIndice.end()) {
                nomeParaIndice[nomeVertice] = indiceAtual++;
                indiceParaNome.push_back(nomeVertice);
            }
        }

        // Ajusta dinamicamente os vetores internos da classe Grafo
        this->totalVertices = indiceAtual;
        this->listaAdjacencia.assign(totalVertices, list<int>());
        this->foiVisitado.assign(totalVertices, false);
        this->estaNaPilhaAtiva.assign(totalVertices, false);

        // 2. Extração dos Arcos (Arestas)
        regex regexArcos(R"(\(([^,]+),([^)]+)\))");
        size_t inicioChaveArcos = linhaArcos.find('{');
        size_t fimChaveArcos = linhaArcos.find('}');
        
        if (inicioChaveArcos == string::npos || fimChaveArcos == string::npos) {
            cerr << "Erro: Formato inválido na linha de arcos (chaves { } não encontradas)." << endl;
            return false;
        }
        
        string conteudoArcos = linhaArcos.substr(inicioChaveArcos + 1, fimChaveArcos - inicioChaveArcos - 1);

        auto arcosBegin = sregex_iterator(conteudoArcos.begin(), conteudoArcos.end(), regexArcos);
        auto arcosEnd = sregex_iterator();

        for (auto i = arcosBegin; i != arcosEnd; ++i) {
            smatch match = *i;
            string origem = match[1].str();
            string destino = match[2].str();

            // Insere o arco apenas se ambos os vértices existem no conjunto de vértices
            if (nomeParaIndice.find(origem) != nomeParaIndice.end() && 
                nomeParaIndice.find(destino) != nomeParaIndice.end()) {
                adicionarAresta(nomeParaIndice[origem], nomeParaIndice[destino]);
            }
        }

        return true;
    }

    Grafo(string filePath) {
        if (!lerGrafoDeArquivo(filePath)) {
            cerr << "Falha ao ler o grafo do arquivo: " << filePath << endl;
        }
    }

    // Adiciona uma conexão direcionada de origem -> destino
    void adicionarAresta(int origem, int destino) {
        listaAdjacencia[origem].push_back(destino);
    }

    // Algoritmo principal de busca
    void buscarCircuito() {
        circuitoEncontrado = false;
        
        // Reinicializa as estruturas para garantir que estejam limpas
        fill(foiVisitado.begin(), foiVisitado.end(), false);
        fill(estaNaPilhaAtiva.begin(), estaNaPilhaAtiva.end(), false);
        while (!caminhoExploracao.empty()) {
            caminhoExploracao.pop();
        }

        // Garante que todos os pontos do grafo sejam testados como "raízes" de busca,
        // caso existam partes isoladas (grafos desconexos)
        for (int vertice = 0; vertice < totalVertices; vertice++) {
            if (!foiVisitado[vertice] && !circuitoEncontrado) {
                explorarVertice(vertice);
            }
        }

        if (circuitoEncontrado) {
            cout << "Resultado: SUCESSO (Circuito encontrado)" << endl;
        } else {
            cout << "Resultado: FRACASSO (Nenhum circuito encontrado no grafo)" << endl;
        }
    }
};