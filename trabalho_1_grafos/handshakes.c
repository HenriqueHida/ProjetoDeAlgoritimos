/*
Como o exemplo usado em sala, o exercicio quer calcular quando apertos de mãos são necessários para que 
duas pessoas se encontrem. É feito a partir do algoritimo Djikstra que usa uma fila de prioridade para
exploração do grafo
exemplo de input:
Joao Maria
7 6
Joao
Maria
Carlos
Ana
Pedro
Luiza
Roberto
Joao Carlos
Carlos Pedro
Pedro Roberto
Roberto Maria
Maria Ana
Ana Luiza

primeiro é passado o nome inicial e o objetivo, o número de pessoas e suas conexões, depois o nome de cada
pessoa e por fim suas conexões, o programa retorna o número de apertos de mão necessários para que as duas pessoas
se encontrem
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 1005
#define INF 0x3f3f3f3f

typedef struct {
    int v, w;
} Edge;

typedef struct {
    Edge edges[MAXN];
    int size;
} PriorityQueue;

int N, M;
char names[MAXN][30];
int dist[MAXN];
int adj[MAXN][MAXN];
char S[30], E[30];

void push(PriorityQueue *pq, Edge e) {
    pq->edges[pq->size++] = e;
    int i = pq->size - 1;
    while (i > 0 && pq->edges[i].w < pq->edges[(i - 1) / 2].w) {
        Edge temp = pq->edges[i];
        pq->edges[i] = pq->edges[(i - 1) / 2];
        pq->edges[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Edge pop(PriorityQueue *pq) {
    Edge min = pq->edges[0];
    pq->edges[0] = pq->edges[--pq->size];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
        if (l < pq->size && pq->edges[l].w < pq->edges[smallest].w) smallest = l;
        if (r < pq->size && pq->edges[r].w < pq->edges[smallest].w) smallest = r;
        if (smallest == i) break;
        Edge temp = pq->edges[i];
        pq->edges[i] = pq->edges[smallest];
        pq->edges[smallest] = temp;
        i = smallest;
    }
    return min;
}

void dijkstra(int src) {
    PriorityQueue pq;
    pq.size = 0;
    for (int i = 0; i < N; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    push(&pq, (Edge){src, 0});
    while (pq.size) {
        Edge e = pop(&pq);
        if (dist[e.v] < e.w) continue;
        for (int i = 0; i < N; i++) {
            if (adj[e.v][i] && dist[e.v] + 1 < dist[i]) {
                dist[i] = dist[e.v] + 1;
                push(&pq, (Edge){i, dist[i]});
            }
        }
    }
}


int main() {
    scanf("%s %s", S, E);
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; i++) {
        scanf("%s", names[i]);
    }
    for (int i = 0; i < M; i++) {
        char V[30], W[30];
        scanf("%s %s", V, W);
        int v = -1, w = -1;
        for (int j = 0; j < N; j++) {
            if (strcmp(names[j], V) == 0) {
                v = j;
            }
            if (strcmp(names[j], W) == 0) {
                w = j;
            }
        }
        adj[v][w] = adj[w][v] = 1;
    }
    int s = -1, e = -1;
    for (int i = 0; i < N; i++) {
        if (strcmp(names[i], S) == 0) {
            s = i;
        }
        if (strcmp(names[i], E) == 0) {
            e = i;
        }
    }
    dijkstra(s);
    if (dist[e] == INF) {
        printf("Impossível alcaçar\n");
    } else {
        printf("Apertos de mão de distância: %d\n", dist[e]);
    }
    return 0;
}