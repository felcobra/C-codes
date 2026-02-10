#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TOTAL_REGISTROS 10000

// Estrutura do registro
typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
} Titem;
// funções para gerar dados
void gerar_nome(char *nome) {
    char a[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 49; i++) {
        nome[i] = a[rand() % 26];
    }
    nome[49] = '\0';
}
void gerar_cpf(int *cpf) {
    for(int i=0;i<11;i++)
        cpf[i]=rand()%10;
}
int gerar_nota() {
    return rand()%101;
}
long long converte_cpf_para_chave(int cpf[11]) {
    long long c=0,m=1;
    for(int i=8;i>=0;i--) {
        c+=cpf[i]*m;
        m*=10;
    }
    return c;
}

// Verifica se o arquivo de registros existe e o cria se necessário
void gerar_arquivo_se_necessario() {
    FILE* arq = fopen("registros.txt", "r");
    if (arq) {
        fseek(arq, 0, SEEK_END);
        if (ftell(arq) > 0) {
            printf("Arquivo encontrado\n");
            fclose(arq);
            return;
        }
        fclose(arq);
    }
    arq = fopen("registros.txt", "w");
    if(!arq) {
        printf("Erro\n");
        exit(1);
    }
    for (int i=0; i<TOTAL_REGISTROS; i++){
        Titem p;
        gerar_nome(p.nome);
        gerar_cpf(p.cpf);
        p.nota=gerar_nota();
        p.chave=converte_cpf_para_chave(p.cpf);
        fprintf(arq, "%lld %s %d ", p.chave, p.nome, p.nota);
        for(int k=0; k<11; k++)
            fprintf(arq, "%d", p.cpf[k]);
        fprintf(arq, "\n");
    }
    fclose(arq);
}
//começa a implementação do heap
typedef struct {
    Titem* A;
    int tamanho_atual;
    int capacidade;
} THeap;

void imprime_item(Titem* item) {
    if (item == NULL) {
        printf("\n");
        return;
    }
    printf("====================================\n");
    printf("Registro:\n");
    printf("Chave CPF: %lld\n", item->chave);
    printf("Nome: %s\n", item->nome);
    printf("Nota: %d \n", item->nota);
    printf("CPF Completo: ");
    for(int i=0;i<11;i++)
        printf("%d",item->cpf[i]);
    printf("\n=================================\n");
}

void imprime_tudo_heap(THeap* heap) {
    for (int i = 0; i < heap->tamanho_atual; i++)
        imprime_item(&heap->A[i]);
    printf("%d registros printados\n", heap->tamanho_atual);
}

THeap* cria_heap() {
    THeap* h=(THeap*)malloc(sizeof(THeap));
    h->capacidade=TOTAL_REGISTROS+100;
    h->tamanho_atual=0;
    h->A=(Titem*)malloc(h->capacidade*sizeof(Titem));
    return h;
}
int pai(int i) {
    return(i-1)/2;
}
void subir_no_heap(THeap* h,int i) {
    while(i>0 && h->A[pai(i)].nota<h->A[i].nota) {
        Titem t=h->A[i];
        h->A[i]=h->A[pai(i)];
        h->A[pai(i)]=t;
        i=pai(i);
    }
}
void descer_no_heap(THeap* h,int i) {
    int m=i,e=2*i+1,d=2*i+2;
    if(e<h->tamanho_atual&&h->A[e].nota>h->A[m].nota)
        m=e;
    if(d<h->tamanho_atual&&h->A[d].nota>h->A[m].nota)
        m=d;
    if(m!=i) {
        Titem t=h->A[i];
        h->A[i]=h->A[m];
        h->A[m]=t;
        descer_no_heap(h,m);
    }
}
void insere_heap(THeap* h,Titem item) {
    if(h->tamanho_atual==h->capacidade) {
        printf("Capacidade esgotada\n");
        return;
    }
    h->tamanho_atual++;
    h->A[h->tamanho_atual-1]=item;
    subir_no_heap(h,h->tamanho_atual-1);
}
Titem remove_do_heap(THeap* h) {
    Titem r=h->A[0];
    h->A[0]=h->A[h->tamanho_atual-1];
    h->tamanho_atual--;
    descer_no_heap(h,0);
    return r;
}
Titem* consulta_heap(THeap* h,long long c) {
    for(int i=0;i<h->tamanho_atual;i++)
        if(h->A[i].chave==c)
            return &h->A[i];
    return NULL;
}

void carregar_dados(THeap* heap) {
    FILE* f=fopen("registros.txt","r");
    if(!f){
        printf("Erro\n");
        exit(1);}
    Titem p;
    while(fscanf(f,"%lld %s %d ",&p.chave,p.nome,&p.nota)!=EOF) {
        for(int k=0;k<11;k++)
            fscanf(f,"%1d",&p.cpf[k]);
        insere_heap(heap,p);
    }
    fclose(f);
}
int main() {
    srand(time(NULL));
    gerar_arquivo_se_necessario();
    THeap* meu_heap=cria_heap();
    carregar_dados(meu_heap);

    int opcao;
    long long chave_op;
    do {
        printf("\n=== MENU HEAP ===\n");
        printf("1. Consultar\n");
        printf("2. Inserir (aleatorio)\n");
        printf("3. Remover (o de maior nota)\n");
        printf("4. Listar Tudo\n");
        printf("5. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave_op);
                imprime_item(consulta_heap(meu_heap, chave_op));
                break;
            case 2:
                Titem p;
                gerar_nome(p.nome);
                gerar_cpf(p.cpf);
                p.nota=gerar_nota();
                p.chave=converte_cpf_para_chave(p.cpf);
                insere_heap(meu_heap, p);
                printf("\n novo registro:\n");
                imprime_item(&p);
                break;
            case 3:
                if(meu_heap->tamanho_atual > 0) {
                    Titem r=remove_do_heap(meu_heap);
                    printf("Item de maior nota removido:\n");
                    imprime_item(&r);
                } else {
                    printf("Heap vazio\n");
                }
                break;
            case 4:
                imprime_tudo_heap(meu_heap);
                break;
            case 5:
                free(meu_heap->A);
                free(meu_heap);
                printf("Saida\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while(opcao != 5);
    return 0;
}