#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TOTAL 10000
#define TAM 100000
typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
    int ativo;
} Registro;
typedef struct {
    long long chave;
    long long posicao;
    int ocupado;
} EntradaHash;
EntradaHash tabela[TAM];
long long converte_cpf_para_chave(int cpf[11]) {
    long long c = 0, m = 1;
    for (int i = 8; i >= 0; i--) {
        c += cpf[i] * m;
        m *= 10;
    }
    return c;
}
void gerar_nome(char *nome) {
    char a[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 49; i++) {
        nome[i] = a[rand() % 26];
    }
    nome[49] = '\0';
}
void gerar_cpf(int* cpf) {
    for (int i = 0; i < 11; i++)
        cpf[i] = rand() % 10;
}
int gerar_nota() {
    return rand() % 101;
}
int funcao_hash(long long chave) {
    return (int)(chave % TAM);
}
void inicializa_tabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i].ocupado = 0;
        tabela[i].chave = 0;
        tabela[i].posicao = -1;
    }
}
void imprime_registro(Registro* reg) {
    if (!reg) {
        return;
    }
    printf("Registro\n");
    printf("Chave: %lld\n", reg->chave);
    printf("Nome: %s\n", reg->nome);
    printf("Nota: %d\n", reg->nota);
    printf("CPF: ");
    for (int i = 0; i < 11; i++)
        printf("%d", reg->cpf[i]);
    printf("\n===================\n");
}
void inserir_hash(Registro* reg, FILE* arq) {
    fseek(arq, 0, SEEK_END);
    long long pos = ftell(arq);
    fprintf(arq, "%d %lld %s %d ", reg->ativo, reg->chave, reg->nome, reg->nota);
    for (int i = 0; i < 11; i++)
        fprintf(arq, "%d", reg->cpf[i]);
    fprintf(arq, "\n");
    fflush(arq);
    int indc = funcao_hash(reg->chave);
    int inicio = indc;
    while (tabela[indc].ocupado == 1) {
        if (tabela[indc].chave == reg->chave) {
            tabela[indc].posicao = pos;
            return;
        }
        indc = (indc + 1) % TAM;
        if (indc == inicio) {
            printf("Tabela cheia\n");
            return;
        }
    }
    tabela[indc].chave = reg->chave;
    tabela[indc].posicao = pos;
    tabela[indc].ocupado = 1;
}
Registro* consultar_hash(long long chave, FILE* arq) {
    int indc = funcao_hash(chave);
    int inicio = indc;
    while (tabela[indc].ocupado != 0) {
        if (tabela[indc].ocupado == 1 && tabela[indc].chave == chave) {
            Registro* reg = (Registro*)malloc(sizeof(Registro));
            fseek(arq, tabela[indc].posicao, SEEK_SET);
            int ativo;
            fscanf(arq, "%d %lld %s %d ", &ativo, &reg->chave, reg->nome, &reg->nota);
            if (ativo == 0) {
                free(reg);
                return NULL;
            }
            for (int i = 0; i < 11; i++)
                fscanf(arq, "%1d", &reg->cpf[i]);
            reg->ativo = ativo;
            return reg;
        }
        indc = (indc + 1) % TAM;
        if (indc == inicio)
            break;
    }
    return NULL;
}
int marcar_registro_removido(long long chave, FILE* arq) {
    int indc = funcao_hash(chave);
    int inicio = indc;
    while (tabela[indc].ocupado != 0) {
        if (tabela[indc].ocupado == 1 && tabela[indc].chave == chave) {
            fseek(arq, tabela[indc].posicao, SEEK_SET);
            fprintf(arq, "0");
            fflush(arq);
            return 1;
        }
        indc = (indc + 1) % TAM;
        if (indc == inicio)
            break;
    }
    return 0;
}
int remover_hash(long long chave, FILE* arq) {
    int indc = funcao_hash(chave);
    int inicio = indc;
    while (tabela[indc].ocupado != 0) {
        if (tabela[indc].ocupado == 1 && tabela[indc].chave == chave) {
            if (marcar_registro_removido(chave, arq)) {
                tabela[indc].ocupado = -1;
                return 1;
            }
            return 0;
        }
        indc = (indc + 1) % TAM;
        if (indc == inicio)
            break;
    }
    return 0;
}
void listar_todos(FILE* arq) {
    int count = 0;
    printf(" Registros \n");
    for (int i = 0; i < TAM; i++) {
        if (tabela[i].ocupado == 1) {
            Registro* reg = (Registro*)malloc(sizeof(Registro));
            fseek(arq, tabela[i].posicao, SEEK_SET);
            int ativo;
            fscanf(arq, "%d %lld %s %d ", &ativo, &reg->chave, reg->nome, &reg->nota);
            if (ativo == 1) {
                for (int j = 0; j < 11; j++)
                    fscanf(arq, "%1d", &reg->cpf[j]);
                reg->ativo = ativo;
                imprime_registro(reg);
                count++;
            }
            free(reg);
        }
    }
    if (count == 0) {
        printf("Nenhum registro encontrado\n");
    } else {
        printf("Total de registros: %d\n", count);
    }
}
void reorganizar_arquivo(FILE** arq) {
    fclose(*arq);
    FILE* original = fopen("registros.txt", "r");
    FILE* temp = fopen("temp_registros.txt", "w");
    if (!original || !temp) {
        return;
    }
    inicializa_tabela();
    char linha[200];
    int registros_ativos = 0;
    while (fgets(linha, sizeof(linha), original)) {
        int ativo;
        long long chave;
        char nome[50];
        int nota;
        int cpf[11];
        sscanf(linha, "%d %lld %s %d", &ativo, &chave, nome, &nota);
        if (ativo == 1) {
            char* cpf_str = linha;
            for (int i = 0; i < 3; i++) {
                cpf_str = strchr(cpf_str, ' ') + 1;
            }
            for (int i = 0; i < 11; i++) {
                cpf[i] = cpf_str[i] - '0';
            }
            long long pos = ftell(temp);
            fprintf(temp, "%d %lld %s %d ", ativo, chave, nome, nota);
            for (int i = 0; i < 11; i++)
                fprintf(temp, "%d", cpf[i]);
            fprintf(temp, "\n");
            int indc = funcao_hash(chave);
            int inicio = indc;
            while (tabela[indc].ocupado == 1) {
                indc = (indc + 1) % TAM;
                if (indc == inicio) {
                    printf("Erro\n");
                    break;
                }
            }
            tabela[indc].chave = chave;
            tabela[indc].posicao = pos;
            tabela[indc].ocupado = 1;
            registros_ativos++;
        }
    }
    fclose(original);
    fclose(temp);
    remove("registros.txt");
    rename("temp_registros.txt", "registros.txt");
    *arq = fopen("registros.txt", "r+");
    printf("Registros ativos mantidos: %d\n", registros_ativos);
}
void reconstruir_tabela_hash(FILE* arq) {
    inicializa_tabela();
    fseek(arq, 0, SEEK_SET);
    char linha[200];
    int registros_carregados = 0;
    while (fgets(linha, sizeof(linha), arq)) {
        int ativo;
        long long chave;
        char nome[50];
        int nota;
        long long pos = ftell(arq) - strlen(linha);
        sscanf(linha, "%d %lld %s %d", &ativo, &chave, nome, &nota);
        if (ativo == 1) {
            int indc = funcao_hash(chave);
            int inicio = indc;
            while (tabela[indc].ocupado == 1) {
                if (tabela[indc].chave == chave) {
                    tabela[indc].posicao = pos;
                    break;
                }
                indc = (indc + 1) % TAM;
                if (indc == inicio) {
                    printf("Erro\n");
                    return;
                }
            }
            if (tabela[indc].ocupado != 1) {
                tabela[indc].chave = chave;
                tabela[indc].posicao = pos;
                tabela[indc].ocupado = 1;
            }
            registros_carregados++;
        }
    }
    printf("Tabela reconstruída com %d registros\n", registros_carregados);
}
void gerar_registros_automaticos() {
    FILE* arq = fopen("registros.txt", "w");
    if (!arq) {
        printf("Erro\n");
        exit(1);
    }
    inicializa_tabela();
    for (int i = 0; i < TOTAL; i++) {
        Registro reg;
        gerar_nome(reg.nome);
        gerar_cpf(reg.cpf);
        reg.nota = gerar_nota();
        reg.chave = converte_cpf_para_chave(reg.cpf);
        reg.ativo = 1;
        inserir_hash(&reg, arq);
    }
    fclose(arq);
    printf("Arquivo criado com %d registros.\n", TOTAL);
}
int main() {
    srand(time(NULL));
    FILE* arq;
    FILE* teste = fopen("registros.txt", "r");
    if (teste) {
        fclose(teste);
        int opcao_arquivo;
        printf("1. Usar arquivo existente\n");
        printf("2. Criar novo arquivo (sobrepoe o que ja existe)\n");
        printf("Opcao: ");
        scanf("%d", &opcao_arquivo);
        if (opcao_arquivo == 1) {
            arq = fopen("registros.txt", "r+");
            if (!arq) {
                printf("Erro\n");
                return 1;
            }
            reconstruir_tabela_hash(arq);
        } else if (opcao_arquivo == 2) {
            gerar_registros_automaticos();
            arq = fopen("registros.txt", "r+");
            if (!arq) {
                printf("Erro\n");
                return 1;
            }
        } else {
            return 1;
        }
    } else {
        gerar_registros_automaticos();
        arq = fopen("registros.txt", "r+");
        if (!arq) {
            printf("Erro\n");
            return 1;
        }
    }
    int opcao;
    long long chave;
    Registro temp;
    do {
        printf("\n MENU \n");
        printf("1. Consulta\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos registros\n");
        printf("5. Reorganizar arquivo\n");
        printf("6. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave);
                {
                    Registro* r = consultar_hash(chave, arq);
                    imprime_registro(r);
                    if (r) free(r);
                }
                break;
            case 2:
                gerar_nome(temp.nome);
                gerar_cpf(temp.cpf);
                temp.nota = gerar_nota();
                temp.chave = converte_cpf_para_chave(temp.cpf);
                temp.ativo = 1;
                inserir_hash(&temp, arq);
                printf("Registro inserido:\n");
                imprime_registro(&temp);
                break;
            case 3:
                printf("Chave para tirar: ");
                scanf("%lld", &chave);
                if (remover_hash(chave, arq))
                    printf("Removido\n");
                else
                    printf("Chave nao encontrada\n");
                break;
            case 4:
                listar_todos(arq);
                break;
            case 5:
                reorganizar_arquivo(&arq);
                break;
            case 6:
                break;
            default:
                break;
        }
    } while (opcao != 6);
    fclose(arq);
    return 0;
}