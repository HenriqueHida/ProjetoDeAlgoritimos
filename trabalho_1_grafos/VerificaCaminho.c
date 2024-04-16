/*
Código em C para verificar se existe um caminho entre dois nós em um grafo direcionado.
Exemplo de input para teste:
5
2
0 1 2
1 2 2
3 4 1
2 4 1
0 0 0
0 4
1 3
4 0

Inicialmente se passa o numero de vertices do grafo, depois é necessario escolher qual função de busca
no grafo será usada (BFS ou DFS), logo em seguida são adicionados as arestas
num formato (aresta1 aresta2 direção), sendo que essa direção pode ser 1 (apenas ida) ou 2 (ida e volta).
e por fim são passados os nós que se deseja verificar se existe um caminho entre eles. O programa também
retorna o tempo de execução a fins de teste entre a bfs e dfs.

exemplo apenas de criação de grafo com 20 nós:
0 5 2
1 7 1
2 8 2
3 10 1
4 13 2
5 14 1
6 15 2
7 16 1
8 17 2
9 18 1
10 19 1
11 0 2
12 1 1
13 2 2
14 3 1
15 4 2
16 6 1
17 9 2
18 11 1
19 12 2
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX
#define MAX_NOS 200

// Estrutura que representa o grafo
typedef struct Grafo {
    int** matriz;
    int numVertices;
} Grafo;

// Ponteiro de função para a escolha da busca
typedef bool (*SearchFunction)(Grafo*, int, int);


void inicializarGrafo(Grafo* grafo, int numVertices) {
    grafo->numVertices = numVertices;
    grafo->matriz = malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        grafo->matriz[i] = malloc(numVertices * sizeof(int));
        for (int j = 0; j < numVertices; j++) {
            grafo->matriz[i][j] = 0;
        }
    }
}

// Função para adicionar uma aresta direcionada entre dois nós
void adicionarAresta(Grafo* grafo, int start, int goal) {
    grafo->matriz[start][goal] = 1;
}

// Função para imprimir o grafo
void imprimirGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = 0; j < grafo->numVertices; j++) {
            printf("%d ", grafo->matriz[i][j]);
        }
        printf("\n");
    }
}

// Função para verificar se existe um caminho entre dois nós usando busca em largura
bool BFS(Grafo* grafo, int start, int goal) {
    bool visitados[MAX_NOS] = { false };
    int fila[MAX_NOS];
    int frente = 0, tras = 0;

    fila[tras++] = start;
    visitados[start] = true;

    while (frente != tras) {
        int noAtual = fila[frente++];
        if (noAtual == goal) {
            return true; // Caminho encontrado
        }

        for (int i = 0; i < grafo->numVertices; i++) {
            if (grafo->matriz[noAtual][i] == 1 && !visitados[i]) {
                fila[tras++] = i;
                visitados[i] = true;
            }
        }
    }

    return false; // Caminho não encontrado
}

// Função para verificar se existe um caminho entre dois nós usando busca em profundidade
bool DFS_util(Grafo* grafo, int current, int goal, bool visited[]) {
    visited[current] = true;
    if (current == goal) {
        return true; // Caminho encontrado
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->matriz[current][i] == 1 && !visited[i]) {
            if (DFS_util(grafo, i, goal, visited)) {
                return true;
            }
        }
    }

    return false; // Caminho não encontrado
}

bool DFS(Grafo* grafo, int start, int goal) {
    bool visited[MAX_NOS] = { false };
    return DFS_util(grafo, start, goal, visited);
}

// Criação da função para medir tempo de execução
clock_t inicio, fim;
double cpu_time_used;

// Função principal
int main() {
    Grafo grafo;
    int numVertices, v, w, d;

    scanf("%d", &numVertices);
    inicializarGrafo(&grafo, numVertices);

    //Definição do método de busca
    SearchFunction search;
    int userSelection;

    printf("Escolha o método de busca:\n1. BFS\n2. DFS\n");
    scanf("%d", &userSelection);

    switch(userSelection) {
    case 1:
        search = BFS;
        printf("BFS foi escolhido\n");
        break;
    case 2:
        search = DFS;
        printf("DFS foi escolhido\n");
        break;
    default:
        printf("Invalido. Default: BFS.\n");
        search = BFS;
        break;
    }
    printf("Adicione arestas:\n");
    // Adicionando arestas
    while((scanf("%d %d %d", &v, &w, &d) != EOF) && ((v != 0) || (w != 0)  || (d != 0))){
        if (v < numVertices && w < numVertices) {
            if(d == 1){
                adicionarAresta(&grafo, v, w);
            }
            else if(d == 2){
                adicionarAresta(&grafo, v, w);
                adicionarAresta(&grafo, w, v);
            }
            else {
                printf("insira um valor válido para D\n");
            }
        }
        else {
            printf("insira um valor válido para v e w\n");
        }
    }
    int t,x;

    // Verificar se existe um caminho entre dois nós
    printf("Nós que deseja verificar:\n");
    while(scanf("%d %d", &t, &x) != EOF){
        if(t < numVertices && x < numVertices) {
            inicio = clock();
            bool ida,volta;
            ida = search(&grafo, t, x);
            volta = search(&grafo, x, t);
            if(ida && volta) {
                printf("Ida e volta\n");
            }
            else if(ida) {
                printf("Apenas ida\n");
            }
            else if(volta) {
                printf("Apenas volta\n");
            }
            else {
                printf("Não existe caminho\n");
            }
            fim = clock();
            cpu_time_used = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
            printf("Tempo de execução: %f seg\n", cpu_time_used);
        }
        else {
            printf("insira um valor válido, segfault\n");
        }
    }
}

