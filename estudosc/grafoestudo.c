#include <stdio.h>
#include <stdlib.h>
//
// Created by srvit on 17/05/2025.
//
typedef struct vizinho {
    int id_vizinho;
    struct vizinho *prox;
}Tvizinho;

typedef struct grafo {
    int id_vertice;
    Tvizinho *prim_vizinho;
    struct grafo *prox;
}Tgrafo;

Tgrafo *inicializa() {
    return NULL;
}

void imprime (Tgrafo *g) {
    Tgrafo *atual = g;
    while (atual != NULL) {
        printf("vértice: %d \n", g->id_vertice);
        printf("vizinho: ");
        Tvizinho *v = g->prim_vizinho;
        while (v!=NULL) {
            printf("%d ", v->id_vizinho);
            v = v->prox;
        }
        printf("\n\n");
        g = g->prox;
    }
}
void libera_vizinho(Tvizinho *v) {
    while (v != NULL) {
        Tvizinho *temp= v;
        v= v->prox;
        free(temp);
    }
}
void libera_grafo(Tgrafo *g) {
    while (g!=NULL) {
        libera_vizinho(g->prim_vizinho);
        Tgrafo *temp= g;
        g = g->prox;
        free(temp);
    }
}
Tgrafo* busca_vertice(Tgrafo *g, int x) {
    while ((g!=NULL) && (g->id_vertice!=x)) {
        g = g->prox;
    }
    return g;
}
Tvizinho* busca_aresta(Tgrafo *g, int v1, int v2) {
    Tgrafo *temp = busca_vertice(g,v1);
    Tgrafo *temp2 = busca_vertice(g,v2);
    Tvizinho *resp = NULL;
    if ((temp!=NULL) && (temp2!=NULL)) {
        resp = temp->prim_vizinho;
        while ((resp!=NULL) && (resp->id_vizinho!=v2)) {
            resp = resp->prox;
        }
    }
    return resp;
}
Tgrafo *insere_vertice(Tgrafo *g, int x) {
    Tgrafo *p= busca_vertice(g,x);
    if (p==NULL) {
        p = (Tgrafo*) malloc(sizeof(Tgrafo));
        p->id_vertice = x;
        p->prox = g;
        p->prim_vizinho = NULL;
        g=p;
    }
    return g;
}
void insere_um_sentido(Tgrafo *g, int v1,int v2) {
    Tgrafo *p = busca_vertice(g,v1);
    Tvizinho *nova = (Tvizinho*) malloc(sizeof(Tvizinho));
    nova->id_vizinho = v2;
    nova->prox = p->prim_vizinho;
    p->prim_vizinho = nova;
}
//caso seja um grafo nao orientado basta vc buscar a aresta dos sentidos desejados, se for null vc só usa a função
// insere um sentido no sentido v1 v2 e v2 v1, uma inserção de aresta pra cada sentido tlgd

void retira_um_sentido(Tgrafo *g, int v1, int v2) {
    Tgrafo *p = busca_vertice(g, v1);
    if (p!=NULL) {
        Tvizinho *ant = NULL;
        Tvizinho *atual = p->prim_vizinho;
        while ((atual!=NULL) && (atual->id_vizinho!=v2)) {
            ant = atual;
            atual = atual->prox;
        }
        if (ant==NULL) //v2 era o primeiro nó da lista
            p->prim_vizinho = atual->prox;
        else
            ant->prox = atual->prox;
        free(atual);
    }
}
void retira_aresta(Tgrafo *g, int v1, int v2) {
    Tvizinho *v = busca_aresta(g,v1,v2);
    if (v!=NULL) {
        retira_um_sentido(g,v1,v2);
        //retira_um_sentido(g,v2,v1); caso seja não orientado
    }
}
void retira_vertice(Tgrafo *g, int v) {
    Tgrafo *atual = busca_vertice(g,v);
    while (atual!=NULL) {
        retira_um_sentido(g,atual->id_vertice,v);
        atual = atual->prox;
    } //aqui tamo tirando o v de todas as listas dos outros vertices, tirando o v como vizinho dos outros
    Tgrafo *ant=NULL;
    atual = g;
    while ((atual!=NULL) && (atual->id_vertice!=v)) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual!=NULL) {
        libera_grafo(atual->prim_vizinho);
        if (ant==NULL) {
            g = atual->prox;
        }
        else {
            ant->prox = atual->prox;
        }
        free(atual);
    }
}


int main() {

}