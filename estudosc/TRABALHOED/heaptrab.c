#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TOTAL 10000
typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
    int ativo;
} Titem;
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
void gerar_registros_automaticos() {
    FILE* arq = fopen("registros.txt", "w");
    if(!arq) {
        exit(1);
    }
    for (int i=0; i<TOTAL; i++){
        Titem p;
        gerar_nome(p.nome);
        gerar_cpf(p.cpf);
        p.nota=gerar_nota();
        p.chave=converte_cpf_para_chave(p.cpf);
        p.ativo = 1;

        fprintf(arq, "%d %lld %s %d ", p.ativo, p.chave, p.nome, p.nota);
        for(int k=0; k<11; k++)
            fprintf(arq, "%d", p.cpf[k]);
        fprintf(arq, "\n");
    }
    fclose(arq);
    printf("Arquivo criado com %d registros.\n", TOTAL);
}
typedef struct {
    Titem* A;
    int tamanho_atual;
    int capacidade;
} THeap;
void imprime_item(Titem* item) {
    if (item == NULL) {
        printf("Registro nao encontrado.\n");
        return;
    }
    printf("Registro\n");
    printf("Chave: %lld\n", item->chave);
    printf("Nome: %s\n", item->nome);
    printf("Nota: %d \n", item->nota);
    printf("CPF: ");
    for(int i=0;i<11;i++)
        printf("%d",item->cpf[i]);
    printf("\n=======================\n");
}
void imprime_tudo_heap(THeap* heap) {
    int count = 0;
    for (int i = 0; i < heap->tamanho_atual; i++) {
        if (heap->A[i].ativo == 1) {
            imprime_item(&heap->A[i]);
            count++;
        }
    }
    printf("Total de registros: %d\n", count);
}
THeap* cria_heap() {
    THeap* h=(THeap*)malloc(sizeof(THeap));
    h->capacidade = TOTAL * 2;
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
        if(h->A[i].chave==c && h->A[i].ativo==1)
            return &h->A[i];
    return NULL;
}
int contar_registros_ativos() {
    FILE* f = fopen("registros.txt", "r");
    if(!f) return 0;
    int count = 0;
    int ativo;
    long long chave;
    char nome[50];
    int nota;
    int cpf[11];
    while(fscanf(f, "%d %lld %s %d ", &ativo, &chave, nome, &nota) != EOF) {
        for(int k = 0; k < 11; k++)
            fscanf(f, "%1d", &cpf[k]);
        if(ativo == 1) {
            count++;
        }
    }
    fclose(f);
    return count;
}
void carregar_dados(THeap* heap) {
    FILE* f=fopen("registros.txt","r");
    if(!f){
        printf("Erro\n");
        exit(1);
    }
    int registros_ativos = contar_registros_ativos();
    printf("Registros no arquivo: %d\n", registros_ativos);
    Titem p;
    int registros_carregados = 0;
    while(fscanf(f,"%d %lld %s %d ",&p.ativo, &p.chave, p.nome, &p.nota)!=EOF) {
        for(int k=0;k<11;k++)
            fscanf(f,"%1d",&p.cpf[k]);
        if(p.ativo == 1 && registros_carregados < heap->capacidade) {
            insere_heap(heap,p);
            registros_carregados++;
        }
    }
    fclose(f);
    printf(" %d registros ativos.\n", registros_carregados);
}
void salvar_dados(THeap* heap) {
    FILE* f = fopen("registros.txt", "w");
    if(!f) {
        printf("Erro\n");
        return;
    }
    int registros_salvos = 0;
    for(int i = 0; i < heap->tamanho_atual; i++) {
        if(heap->A[i].ativo == 1) {
            fprintf(f, "%d %lld %s %d ", heap->A[i].ativo, heap->A[i].chave,
                   heap->A[i].nome, heap->A[i].nota);
            for(int k = 0; k < 11; k++)
                fprintf(f, "%d", heap->A[i].cpf[k]);
            fprintf(f, "\n");
            registros_salvos++;
        }
    }
    fclose(f);
    printf("%d registros salvos.\n", registros_salvos);
}
void adicionar_ao_arquivo(Titem* item) {
    FILE* f = fopen("registros.txt", "a");
    if(!f) {
        return;
    }
    fprintf(f, "%d %lld %s %d ", item->ativo, item->chave, item->nome, item->nota);
    for(int k = 0; k < 11; k++)
        fprintf(f, "%d", item->cpf[k]);
    fprintf(f, "\n");
    fclose(f);
}
int remover_do_arquivo(long long chave) {
    FILE* f = fopen("registros.txt", "r");
    FILE* temp = fopen("temp_registros.txt", "w");
    if(!f || !temp) {
        return 0;
    }
    int encontrado = 0;
    Titem p;
    while(fscanf(f,"%d %lld %s %d ",&p.ativo, &p.chave, p.nome, &p.nota)!=EOF) {
        for(int k=0;k<11;k++)
            fscanf(f,"%1d",&p.cpf[k]);
        if(p.chave == chave && p.ativo == 1) {
            p.ativo = 0;
            encontrado = 1;
        }
        fprintf(temp, "%d %lld %s %d ", p.ativo, p.chave, p.nome, p.nota);
        for(int k = 0; k < 11; k++)
            fprintf(temp, "%d", p.cpf[k]);
        fprintf(temp, "\n");
    }
    fclose(f);
    fclose(temp);
    remove("registros.txt");
    rename("temp_registros.txt", "registros.txt");
    return encontrado;
}
void reorganizar_arquivo(THeap* heap) {
    FILE* f = fopen("registros.txt", "w");
    if(!f) {
        return;
    }
    int registros_ativos = 0;
    for(int i = 0; i < heap->tamanho_atual; i++) {
        if(heap->A[i].ativo == 1) {
            fprintf(f, "%d %lld %s %d ", heap->A[i].ativo, heap->A[i].chave,
                   heap->A[i].nome, heap->A[i].nota);
            for(int k = 0; k < 11; k++)
                fprintf(f, "%d", heap->A[i].cpf[k]);
            fprintf(f, "\n");
            registros_ativos++;
        }
    }
    fclose(f);
    printf("Registros mantidos: %d\n", registros_ativos);
}
int main() {
    srand(time(NULL));
    THeap* meu_heap;
    FILE* teste = fopen("registros.txt", "r");
    if (teste) {
        fclose(teste);
        int opcao_arquivo;
        printf("1. Usar arquivo existente (manter dados)\n");
        printf("2. Criar novo arquivo (Sobrepoe o que ja existe)\n");
        printf("Opcao: ");
        scanf("%d", &opcao_arquivo);
        if (opcao_arquivo == 1) {
            printf("Carregando dados do arquivo existente\n");
            meu_heap = cria_heap();
            carregar_dados(meu_heap);
        } else if (opcao_arquivo == 2) {
            gerar_registros_automaticos();
            meu_heap = cria_heap();
            carregar_dados(meu_heap);
        } else {
            return 1;
        }
    } else {
        gerar_registros_automaticos();
        meu_heap = cria_heap();
        carregar_dados(meu_heap);
    }
    int opcao;
    long long chave_op;
    do {
        printf("1. Consulta\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover (heap max)\n");
        printf("4. Listar todos registros\n");
        printf("5. Reorganizar arquivo\n");
        printf("6. Salvar dados manualmente\n");
        printf("7. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        switch(opcao) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave_op);
                imprime_item(consulta_heap(meu_heap, chave_op));
                break;

            case 2:
                {
                    Titem p;
                    gerar_nome(p.nome);
                    gerar_cpf(p.cpf);
                    p.nota=gerar_nota();
                    p.chave=converte_cpf_para_chave(p.cpf);
                    p.ativo = 1;

                    insere_heap(meu_heap, p);
                    adicionar_ao_arquivo(&p);
                    printf("\nNovo registro inserido:\n");
                    imprime_item(&p);
                }
                break;
            case 3:
                if(meu_heap->tamanho_atual > 0) {
                    Titem r=remove_do_heap(meu_heap);
                    r.ativo = 0;
                    if(remover_do_arquivo(r.chave)) {
                        printf("Removido\n");
                        imprime_item(&r);
                    } else {
                        printf("Erro ao remover\n");
                    }
                } else {
                    printf("Heap vazio\n");
                }
                break;
            case 4:
                imprime_tudo_heap(meu_heap);
                break;
            case 5:
                reorganizar_arquivo(meu_heap);
                break;
            case 6:
                salvar_dados(meu_heap);
                break;
            case 7:
                salvar_dados(meu_heap);
                free(meu_heap->A);
                free(meu_heap);
                break;
            default:
        }
    } while(opcao != 7);
    return 0;
}
