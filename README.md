# Trabalho2_APA
Repositório feito para o trabalho 2 da disciplina de Análise e Projeto de Algoritmos

# Enunciados
## Primeira implementação: Prompt: Dado um grafo formado por um conjunto de vértices e um conjunto de ligações desenvolver um algoritmo que verifique se o mesmo é conexo.

### Primeira parte: Implementar o algoritmo e fazer análise da complexidade teórica eprática com a simulação de outros exemplos. Plotar os gráficos de tempo.
- **Para o caso não orientado:**
  - Exemplo:
    - Dados de Entrada:
      - Conjunto de vértices:
        X = {x1.x2,x3,x4,x5,x6}
      - Conjunto de arestas:
        U = {[x1,x2],[x2,x3],[x3,x1],[x4,x5],[x5,x6],[x6,x4]}
    - A implementação deve ser feita da seguinte forma:
      1.  – Construir a lista de adjacência a partir dos dados de entrada.
      2.  – Computar o fecho transitivo de um vértice escolhido, podendo ser qualquer um.
        - Para tanto, utilizar o algoritmo de busca em profundidade. Não precisa visitar as
          arestas.
      3.  – Caso o conjunto do fecho seja igual a X o grafo é conexo, caso contrário desconexo.

- **Para o caso orientado:**
  - Exemplo:
    - Dados de Entrada:
      - Conjunto de vértices:
        X = {x1.x2,x3,x4,x5,x6}
      - Conjunto de arcos:
        U = {(x1,x2),(x2,x3),(x3,x1),(x4,x5),(x5,x6),(x2,x4)}
    - A implementação deve seguir os seguintes passos:
      1.  – Simetrizar o grafo adicionando ao conjunto de arcos todos os arcos simétricos.
      2.  – Construir a lista de sucessores a partir dos dados de entrada.
      3.  – Computar o fecho transitivo direto de um vértice escolhido, podendo ser qualquer
        um. Para tanto, utilizar o algoritmo de busca em profundidade. Não precisa visitar os
        arestas.
      4.  – Caso o conjunto do fecho seja igual a X o grafo é conexo, caso contrário desconexo.

## Segunda parte: 
- Utilizar o prompt em uma LLM e pedir a geração do código de um ou
dois algoritmos que resolva o problema para os dois casos: orientado e não orientado.
A entrada dos dados poderá ser feita da mesma forma. 
- Realizar simulações e comparar a complexidade teórica e prática com os algoritmos implementados.

## Segunda implementação: Prompt: Dado um grafo formado por um conjunto de vértices e um conjunto de arcos desenvolver um algoritmo que verifique se o mesmo possui um circuito.

### Primeira parte: Implementar os dois algoritmos e fazer análise da complexidade teórica e prática com a simulação de outros exemplos. Plotar os gráficos de tempo.

- **Exemplo:**
  - Dados de Entrada:
    - Conjunto de vértices:
      - X = {x1.x2,x3,x4,x5,x6}
    - Conjunto de arcos:
      - U = {(x1,x2),(x2,x3),(x3,x1),(x4,x5),(x5,x6),(x2,x4)}
  - A implementação deve seguir os seguintes passos:

## Para o algoritmo Apagador, utilizar o PDF apagador :
1. – Construir a matriz de adjacência a partir dos dados de entrada.
2. – Computar o vetor de cardinalidade do conjunto de antecessores ou do grau de Recebimento de cada vértice somando as entradas das colunas da matriz.
3. – Verificar a existência de valores nulos no vetor. Caso o vetor não possua valores Nulos, pare o grafo contém circuito.
4. – Caso contrário, atualizar o vetor de cardinalidade subtraindo do mesmo as linhas
da matriz correspondentes aos elementos nulos.
5. – Retornar ao passo 3.

## Para o algoritmo de Busca, utilizar o PDF busca circuito:
1. – Construir a lista de sucessores a partir dos dados de entrada.
2. – Realizar uma busca em profundidade até detectar um circuito. 
  - Caso um circuito não seja detectado, o algoritmo termina imprimindo fracasso. Do contrário, termina imprimindo sucesso.

## Segunda parte: Utilizar o prompt em uma LLM e pedir a geração do código de um algoritmo que resolva o problema. A entrada dos dados poderá ser feita da mesma forma.
  - Realizar simulações e comparar a complexidade teórica e prática com os
  - Algoritmos implementados.


# Divisão:
- Algoritmo que verifica conectividade: Igor
- Algoritmo apagador: Thales
- Algoritmo de busca: Victor
- 2 Algs com IA + apresentação: Filipe
