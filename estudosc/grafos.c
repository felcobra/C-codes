#include <stdio.h>
#include <stdlib.h>
//estrutura para um Lista de adjacência
typedef struct no {
    int dest;   // destino da ARESTA
    struct no* prox; // prox no da lista
} no;
//estrutura pra um vertice no grafo
typedef struct Vertex {
    int id;    //id do vertice
    no* head;   // cabeça da lista de adjacência
}Vertex;
// estutura para grafo
typedef struct Graph {
    int numVertices;   //numero de vertices
    Vertex* vertices;    //Array de vértices
}Graph;
Graph* Creategraph(int numVertices) {
    Graph* graph= (Graph*) malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->vertices = (Vertex*)malloc(numVertices * sizeof(Vertex));

    for (int i = 0; i < numVertices; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].head = NULL;
    }
    return graph;
} //essa função aloca memória para o grafo e inicializa cada vértice com um indentificador e uma lista de adjacência vazia

void addEdge(Graph* graph, int src, int dest) {

}