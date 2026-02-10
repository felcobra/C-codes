#include "heap.h"

Heap heap;

void inicializar_heap() {
    heap.capacidade = TOTAL_REGISTROS + 1000;
    heap.registros = malloc(sizeof(Registro) * heap.capacidade);
    heap.tamanho = 0;
}

void carregar_arquivo_geral_heap() {
    FILE* arq = fopen("registros_geral.txt", "r");
    if (!arq) {
        printf("Erro ao abrir arquivo geral\n");
        return;
    }

    printf("Carregando registros para Heap...\n");

    while (!feof(arq)) {
        Registro reg;

        if (fscanf(arq, "%lld %s %d ", &reg.chave, reg.nome, &reg.nota) != 3) {
            break;
        }

        for (int i = 0; i < 11; i++) {
            if (fscanf(arq, "%1d", &reg.cpf[i]) != 1) {
                break;
            }
        }

        fscanf(arq, "\n");

        inserir_heap(&reg);
    }

    fclose(arq);
    printf("Dados carregados na Heap com sucesso!\n");
}

void salvar_arquivo_heap() {
    FILE* arq = fopen("registros_heap.txt", "w");
    if (!arq) {
        printf("Erro ao criar arquivo da Heap\n");
        return;
    }

    for (int i = 0; i < heap.tamanho; i++) {
        escreve_registro_arquivo(arq, &heap.registros[i]);
    }

    fclose(arq);
    printf("Arquivo Heap salvo com sucesso!\n");
}

int pai(int i) {
    return (i - 1) / 2;
}

int filho_esquerdo(int i) {
    return 2 * i + 1;
}

int filho_direito(int i) {
    return 2 * i + 2;
}

void heapify_up(int indice) {
    while (indice > 0) {
        int pai_idx = pai(indice);
        if (heap.registros[indice].nota <= heap.registros[pai_idx].nota) {
            break;
        }

        // Troca
        Registro temp = heap.registros[indice];
        heap.registros[indice] = heap.registros[pai_idx];
        heap.registros[pai_idx] = temp;

        indice = pai_idx;
    }
}

void heapify_down(int indice) {
    while (1) {
        int maior = indice;
        int esq = filho_esquerdo(indice);
        int dir = filho_direito(indice);

        if (esq < heap.tamanho && heap.registros[esq].nota > heap.registros[maior].nota) {
            maior = esq;
        }

        if (dir < heap.tamanho && heap.registros[dir].nota > heap.registros[maior].nota) {
            maior = dir;
        }

        if (maior == indice) {
            break;
        }

        // Troca
        Registro temp = heap.registros[indice];
        heap.registros[indice] = heap.registros[maior];
        heap.registros[maior] = temp;

        indice = maior;
    }
}

int inserir_heap(Registro* reg) {
    if (heap.tamanho >= heap.capacidade) {
        heap.capacidade *= 2;
        heap.registros = realloc(heap.registros, sizeof(Registro) * heap.capacidade);
    }

    heap.registros[heap.tamanho] = *reg;
    heapify_up(heap.tamanho);
    heap.tamanho++;

    return 1;
}

Registro* consultar_heap(long long chave) {
    for (int i = 0; i < heap.tamanho; i++) {
        if (heap.registros[i].chave == chave) {
            return &heap.registros[i];
        }
    }
    return NULL;
}

int remover_heap(long long chave) {
    int indice = -1;
    for (int i = 0; i < heap.tamanho; i++) {
        if (heap.registros[i].chave == chave) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        return 0;
    }

    // Substitui o elemento removido pelo último
    heap.registros[indice] = heap.registros[heap.tamanho - 1];
    heap.tamanho--;

    // Rebalanceia
    if (indice < heap.tamanho) {
        heapify_down(indice);
        heapify_up(indice);
    }

    return 1;
}

void listar_registros_heap() {
    printf("=== LISTAGEM DOS REGISTROS HEAP (ordenado por nota) ===\n");

    // Cria uma cópia para não destruir a heap original
    Registro* temp = malloc(sizeof(Registro) * heap.tamanho);
    int temp_size = heap.tamanho;

    for (int i = 0; i < heap.tamanho; i++) {
        temp[i] = heap.registros[i];
    }

    // Ordena por nota (bubble sort simples)
    for (int i = 0; i < temp_size - 1; i++) {
        for (int j = 0; j < temp_size - i - 1; j++) {
            if (temp[j].nota < temp[j + 1].nota) {
                Registro aux = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = aux;
            }
        }
    }

    for (int i = 0; i < temp_size; i++) {
        imprime_registro(&temp[i]);
    }

    free(temp);
    printf("Total de registros: %d\n", heap.tamanho);
}

void liberar_heap() {
    free(heap.registros);
    heap.tamanho = 0;
}

void menu_heap() {
    int resp;
    long long chave;

    do {
        printf("\n=== MENU HEAP ===\n");
        printf("1. Consultar registro\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos os registros\n");
        printf("5. Salvar arquivo Heap\n");
        printf("6. Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d", &resp);

        switch(resp) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave);
                imprime_registro(consultar_heap(chave));
                break;

            case 2: {
                Registro novo_reg;
                gerar_nome(novo_reg.nome);
                gerar_cpf(novo_reg.cpf);
                novo_reg.nota = gerar_nota();
                novo_reg.chave = converte_cpf_para_chave(novo_reg.cpf);

                if (inserir_heap(&novo_reg)) {
                    printf("Registro inserido:\n");
                    imprime_registro(&novo_reg);
                } else {
                    printf("Erro ao inserir registro!\n");
                }
                break;
            }

            case 3:
                printf("Chave para remover: ");
                scanf("%lld", &chave);
                if (remover_heap(chave)) {
                    printf("Registro removido com sucesso!\n");
                } else {
                    printf("Registro nao encontrado!\n");
                }
                break;

            case 4:
                listar_registros_heap();
                break;

            case 5:
                salvar_arquivo_heap();
                break;

            case 6:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (resp != 6);
}