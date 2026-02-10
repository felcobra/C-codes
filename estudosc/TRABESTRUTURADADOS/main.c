#include "registro.h"
#include "bmais.h"
#include "hash.h"
#include "heap.h"

void menu_principal() {
    int resp;

    do {
        printf("\n========================================\n");
        printf("     SISTEMA DE ESTRUTURAS DE DADOS\n");
        printf("========================================\n");
        printf("1. Gerar arquivo geral de registros\n");
        printf("2. Menu Arvore B+\n");
        printf("3. Menu Hash\n");
        printf("4. Menu Heap\n");
        printf("5. Sair\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &resp);

        switch(resp) {
            case 1:
                printf("Gerando arquivo geral...\n");
                gerar_arquivo_geral();
                break;

            case 2:
                printf("Inicializando Arvore B+...\n");
                inicializar_bmais();
                carregar_arquivo_geral_bmais();
                menu_bmais();
                liberar_memoria();
                break;

            case 3:
                printf("Inicializando Hash...\n");
                inicializar_hash();
                carregar_arquivo_geral_hash();
                menu_hash();
                liberar_hash();
                break;

            case 4:
                printf("Inicializando Heap...\n");
                inicializar_heap();
                carregar_arquivo_geral_heap();
                menu_heap();
                liberar_heap();
                break;

            case 5:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (resp != 5);
}

int main() {
    srand(time(NULL));

    printf("========================================\n");
    printf("  BEM-VINDO AO SISTEMA DE ESTRUTURAS!\n");
    printf("========================================\n");
    printf("Trabalho de Estruturas de Dados\n");
    printf("Implementacao: B+, Hash e Heap\n");
    printf("========================================\n");

    menu_principal();

    printf("Obrigado por usar o sistema!\n");
    return 0;
}