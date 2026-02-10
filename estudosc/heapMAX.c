#include <stdio.h>
#include <stdlib.h>
//
// Created by srvit on 17/06/2025.
//
int pai(int i) {
    return(i/2);
}
int dir(int i) {
    return((i * 2) + 1);
}
int esq (int i) {
    return(i*2);
}
void descer(int *heap, int i, int n) {
    //desce o elemento de i na heap se necessario
    //descobre quem e o maior filho de i
    int d = dir(i);
    int e = esq(i);
    int maior = i;
    if (e<=n && heap[e] > heap[i]) {
        maior = e;
    }
    if (d<=n && heap[d] > heap[i]) {
        maior = d;
    }
    if (maior != 1) {
        //faz a descida trocando com o maior filho
        int temp = heap[i];
        heap [i] = heap[maior];
        heap[maior] = temp;
        descer(heap, maior, n);
    }
}
void constroi_heap_max(int *heap, int n) {
    int i;
    int j = (n/2);
    for (i=j; i>=1; i--) {
        descer(heap, i, n);
    }
}
void subir(int* heap, int i){
    int j = pai(i);
    if (j>= 1) {
        if (heap[i] > heap[j]) {
            // faz a subida
            int temp = heap[i];
            heap[i] = heap[j];
            heap[j] = temp;
            subir(heap, i);
        }
    }
}
void imprime(int *vet, int n) {
    for (int i=0; i<n; i++) {
        printf("%d, ", vet[i]);
    }
    printf("\n");
}
int insere(int *heap, int novo, int n) {
    //aumenta o tamanho do vetor
    heap = (int *)realloc(heap, sizeof(int) * (n+2));
    n = n + 1;
    heap[n] = novo;
    subir(heap, n);
    //retorna o valor novo de n
    return n;
}
int exclui(int *heap, int n) {
    heap[1] = heap[n];
    n = n - 1;
    //diminui o tamanho do vetor
    heap = (int*)realloc(heap, sizeof((int) * (n)));
    descer(heap, 1, n);
    return n;
}
int main(void) {
    int *heap;
    int n;
    printf("digite o tamanho do vetor de elementos: /n");
    scanf("%d",&n);
    if (n<=0)
        return 0;
    //vetor comecara em um, por isso alocamos n+1
    heap = (int*)malloc(sizeof(int) * (n+1));

}