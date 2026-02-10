#include <stdio.h>
#include <stdlib.h>

typedef struct Sno {
    int chave;
    struct Sno* dir;
    struct Sno* esq;
} Tno;
Tno* rotacao_esquerda(Tno* no) {
    Tno *loc = no;
    Tno *aux = loc->dir;
    loc->dir = loc->esq;
    loc->esq = loc;
    loc = aux;
    return loc;
}
Tno* rotacao_direita(Tno* no) {
    Tno *loc = no;
    Tno *aux = loc->esq;
    loc->esq = loc->dir;
    loc->dir = loc;
    loc = aux;
    return loc;
}
Tno* rotacao_dupla_direita(Tno* no) {
    rotacao_esquerda(no->esq);
    rotacao_direita(no);
    return no;
}
Tno* rotacao_dupla_esquerda(Tno* no) {
    rotacao_direita(no->dir);
    rotacao_esquerda(no);
    return no;
}
int maior(int a, int b) {
    return (a > b) ? a : b;
}
int altura(Tno *no) {
    Tno *loc = no;
    if (loc == NULL) {
        return -1;
    }
    else {
        int esq = altura(loc->esq);
        int dir = altura(loc->dir);
        if (esq > dir)
            return esq + 1;
        else
            return dir + 1;
    }
}
int fb(Tno* no) {
    if (no!=NULL)
        return (altura(no->dir) - altura(no->esq));
    else
        return 0;
}
void balancear(Tno *no) {
    Tno *loc = no;
    int fatb = fb(loc);
    if (fatb < -1) { //rotacao a direita
        if (fb(loc->esq) > 0) { //verifica necessidade de rotacao dupla
            rotacao_esquerda(loc->esq);
        }
        rotacao_direita(loc);
    }
    else if (fatb > 1) {
        if (fb(loc->dir) < 0) {
            rotacao_direita(loc->dir);
        }
        rotacao_esquerda(loc);
    }
}
Tno* busca(Tno *no, int chave) {
    Tno *loc = no;
    int v = chave;
    if (loc == NULL)
        return NULL;
    else if (loc->chave == v)
        return loc;
    else if (loc->chave > v)
        return busca(loc->esq, v);
    else if (loc->chave < v)
        return busca(loc->dir, v);
    return NULL;
}
Tno* insere(Tno* no, int chave) {
    Tno *loc = no;
    int v = chave;
    if (loc == NULL) {
        loc->chave = v;
        loc->dir = NULL;
        loc->esq = NULL;
        return loc;
    }
    if (loc->chave < v)
        loc->dir = insere(loc->dir, chave);
    else if (loc->chave == v) {
        printf("erro fi");
        exit(1);
    }
    else
       loc->esq = insere(loc->esq, chave);
    balancear(no);
    return loc;
}
Tno* inverte(Tno* no) {
    if (no == NULL) return NULL;
        Tno *loc=no;
        Tno *aux = loc->dir;
        loc->dir = loc->esq;
        loc->esq = aux;
        inverte(loc->dir);
        inverte(loc->esq);
        return loc;
}
void print_ordenado(Tno* no) {
    if (no) {
        print_ordenado(no->esq);
        printf("%d", no->chave);
        print_ordenado(no->dir);
    }
}
int qt_nos(Tno* no) {
     (no == NULL) ? 0 : 1 + qt_nos(no->esq) + qt_nos(no->dir);
}
int qt_folhas(Tno* no) {
    if (no == NULL)
        return 0;
    else {
        if (no->esq == NULL && no->dir == NULL)
            return 1;
        else {
            return qt_folhas(no->esq) + qt_folhas(no->dir);
        }
    }
}
Tno* remove(Tno *no, int chave) {
    Tno *loc = no;
    int v = chave;
    if (loc == NULL) {
        printf("Valor nao encontrado \n");
        return NULL;
    }
    else {
        if (loc->chave == v) {
            if (loc->esq == NULL && loc->dir == NULL) {
                free(loc);
                return NULL;
            }
            else {
                if (loc->esq != NULL && loc->dir != NULL) {
                    Tno *aux= loc->esq;
                    while (aux->dir != NULL) {
                        aux = aux->dir;
                    }
                    loc->chave = aux->chave;
                    aux->chave = chave;
                    loc->esq = remove(loc->esq, chave);
                    free(aux);
                    balancear(no);
                    return loc;
                }
                else {
                    Tno *aux;
                    if (loc->esq != NULL)
                        aux = loc->esq;
                    else
                        aux = loc->dir;
                    free(loc);
                    balancear(no);
                    return aux;
                }
            }
        }
        else {
            if (loc->chave > v)
                loc->esq = remove(loc->esq, chave);
            else
                loc->dir = remove(loc->dir, chave);
            balancear(no);
            return loc;
        }
    }

}
void liberar_arvore(Tno *no) {
    Tno *loc = no;
    if (loc != NULL) {
        if (loc->esq != NULL) {
            liberar_arvore(loc->esq);
        }
        if (loc->dir != NULL) {
            liberar_arvore(loc->dir);
        }
        free(loc->esq);
        free(loc->dir);
        free(loc);
        loc = NULL;
    }
}
int main(void) {
    Tno *no = NULL;
    int opcao, valor;
    do {
        printf("\n 0 - sair \n  1 - inserir \n 2 - imprimir \n 3 - altura \n 4- remover");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("digite um valor");
                scanf("%d",&valor);
                no = insere(no, valor);
                break;
            case 2:
                print_ordenado(no);
                break;
            case 3:
                printf("altura da árvore: %d \n\n", altura(no));
                break;
            case 4:
                printf("digite um valor para ser removido: ");
                scanf("%d",&valor);
                no = remove(no, valor);
            default:
                if (opcao != 0)
                    printf("\n opcao invalida \n");
        }
    }while (opcao != 0);
    return 0;
}