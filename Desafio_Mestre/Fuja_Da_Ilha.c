/*
 * ======================================================================================
 * NOME DO PROJETO: DESAFIO FINAL - A FUGA DA ILHA
 * DESCRIÇÃO: Sistema de organização de componentes para a torre de resgate.
 *            Inclui ordenação, busca binária e análise de desempenho.
 * AUTOR: Augusto Bento Carvalho
 * ======================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições de constantes
#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

// 1. Criação da Struct Componente
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade; // 1 (Baixa) a 10 (Alta)
} Componente;

// Protótipos das funções
void lerString(char *buffer, int tamanho);
void mostrarComponentes(Componente vetor[], int qtd);
void bubbleSortNome(Componente vetor[], int qtd, long *comparacoes);
void insertionSortTipo(Componente vetor[], int qtd, long *comparacoes);
void selectionSortPrioridade(Componente vetor[], int qtd, long *comparacoes);
int buscaBinariaPorNome(Componente vetor[], int qtd, char chave[]);
void medirTempo(void (*algoritmo)(Componente[], int, long*), Componente vetor[], int qtd, char *nomeAlgo);
void preencherDadosTeste(Componente vetor[], int *qtd);

int main() {
    Componente lista[MAX_COMPONENTES];
    int qtd = 0;
    int opcao;
    int ordenadoPorNome = 0; // Flag para validar busca binária
    int dadosTesteCarregados = 0; // Flag para impedir recarga dos dados de teste
    char busca[TAM_NOME];

    // Seed para testes aleatórios se necessário, mas usaremos dados fixos ou entrada manual
    srand(time(NULL));

    do {
        printf("\n======================================================\n");
        printf("       DESAFIO FINAL: A FUGA DA ILHA - MENU\n");
        printf("======================================================\n");
        printf("1. Adicionar Componente Manualmente\n");
        printf("2. Carregar Dados de Teste (Auto-Preencher)\n");
        printf("3. Ordenar por NOME (Bubble Sort)\n");
        printf("4. Ordenar por TIPO (Insertion Sort)\n");
        printf("5. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6. Buscar Componente-Chave (Busca Binária)\n");
        printf("7. Mostrar Lista Atual\n");
        printf("0. Sair\n");
        printf("Escolha uma estratégia: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer do teclado

        switch (opcao) {
            case 1:
                if (qtd < MAX_COMPONENTES) {
                    printf("\n--- Novo Componente ---\n");
                    printf("Nome: ");
                    lerString(lista[qtd].nome, TAM_NOME);
                    printf("Tipo (ex: controle, suporte, propulsão): ");
                    lerString(lista[qtd].tipo, TAM_TIPO);
                    printf("Prioridade (1-10): ");
                    scanf("%d", &lista[qtd].prioridade);
                    getchar(); // Limpar buffer
                    qtd++;
                    ordenadoPorNome = 0; // Nova inserção desordena a lista
                    printf("Componente adicionado com sucesso!\n");
                } else {
                    printf("\n[ERRO] Capacidade máxima da torre atingida!\n");
                }
                break;

            case 2:
                if (dadosTesteCarregados) {
                    printf("\n[AVISO] Os dados de teste já foram carregados e não podem ser duplicados.\n");
                } else {
                    int qtdAntes = qtd;
                    preencherDadosTeste(lista, &qtd);
                    // Só marca como carregado se a quantidade realmente aumentou (sucesso)
                    if (qtd > qtdAntes) {
                        dadosTesteCarregados = 1;
                        ordenadoPorNome = 0;
                        printf("\nDados de teste carregados. Total: %d componentes.\n", qtd);
                    }
                }
                break;

            case 3:
                // Passamos a função bubbleSortNome como parâmetro para medirTempo
                if (qtd > 0) {
                    medirTempo(bubbleSortNome, lista, qtd, "Bubble Sort (Nome)");
                    ordenadoPorNome = 1; // Agora está ordenado por nome
                } else printf("\nLista vazia!\n");
                break;

            case 4:
                if (qtd > 0) {
                    medirTempo(insertionSortTipo, lista, qtd, "Insertion Sort (Tipo)");
                    ordenadoPorNome = 0; // Ordenado por tipo, não por nome
                } else printf("\nLista vazia!\n");
                break;

            case 5:
                if (qtd > 0) {
                    medirTempo(selectionSortPrioridade, lista, qtd, "Selection Sort (Prioridade)");
                    ordenadoPorNome = 0; // Ordenado por prioridade, não por nome
                } else printf("\nLista vazia!\n");
                break;

            case 6:
                if (qtd == 0) {
                    printf("\nLista vazia!\n");
                } else if (!ordenadoPorNome) {
                    printf("\n[ALERTA] A Busca Binária exige ordenação por NOME primeiro.\n");
                    printf("Execute a opção 3 (Bubble Sort) antes de buscar.\n");
                } else {
                    printf("\nDigite o nome do componente-chave para buscar: ");
                    lerString(busca, TAM_NOME);
                    
                    // Medição de tempo simples para a busca
                    clock_t inicio = clock();
                    int pos = buscaBinariaPorNome(lista, qtd, busca);
                    clock_t fim = clock();
                    double tempoBusca = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                    if (pos != -1) {
                        printf("\n[SUCESSO] Componente '%s' encontrado na posição %d!\n", lista[pos].nome, pos + 1);
                        printf("Detalhes: Tipo: %s | Prioridade: %d\n", lista[pos].tipo, lista[pos].prioridade);
                        printf("Torre pronta para receber este módulo.\n");
                    } else {
                        printf("\n[FALHA] Componente '%s' não encontrado nos escombros.\n", busca);
                    }
                    printf("Tempo de busca: %f segundos\n", tempoBusca);
                }
                break;

            case 7:
                mostrarComponentes(lista, qtd);
                break;

            case 0:
                printf("\nEncerrando sistema da torre... Boa sorte na fuga!\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// ======================================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ======================================================================================

// Função auxiliar para ler strings e remover o \n do final
void lerString(char *buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// 2. e 6. Função para mostrar os componentes formatados
void mostrarComponentes(Componente vetor[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    printf("\n%-4s | %-25s | %-15s | %s\n", "ID", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-4d | %-25s | %-15s | %d\n", i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].prioridade);
    }
    printf("----------------------------------------------------------------\n");
}

// 5. Função Wrapper para medir tempo e comparações
// Recebe um ponteiro para função de ordenação
void medirTempo(void (*algoritmo)(Componente[], int, long*), Componente vetor[], int qtd, char *nomeAlgo) {
    long comparacoes = 0;
    clock_t inicio, fim;
    double tempo_gasto;

    printf("\nExecutando %s...\n", nomeAlgo);
    
    inicio = clock();
    algoritmo(vetor, qtd, &comparacoes);
    fim = clock();

    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Ordenação concluída!\n");
    printf("Tempo de execução: %f segundos\n", tempo_gasto);
    printf("Número de comparações: %ld\n", comparacoes);
    
    // Feedback educacional
    if (comparacoes > qtd * qtd / 2) {
        printf("[DICA] O número de comparações foi alto. Algoritmos quadráticos sofrem com muitos dados.\n");
    } else {
        printf("[DICA] Bom desempenho para a quantidade de dados atual.\n");
    }
}

// 3. Bubble Sort - Ordenar por NOME
void bubbleSortNome(Componente vetor[], int qtd, long *comparacoes) {
    int i, j;
    Componente temp;
    for (i = 0; i < qtd - 1; i++) {
        for (j = 0; j < qtd - i - 1; j++) {
            (*comparacoes)++; // Contabiliza a comparação do if abaixo
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                // Troca
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

// 3. Insertion Sort - Ordenar por TIPO
void insertionSortTipo(Componente vetor[], int qtd, long *comparacoes) {
    int i, j;
    Componente chave;
    for (i = 1; i < qtd; i++) {
        chave = vetor[i];
        j = i - 1;

        // O loop while faz comparações a cada iteração
        // Contamos +1 para a verificação que falha ou entra no loop
        (*comparacoes)++; 
        while (j >= 0 && strcmp(vetor[j].tipo, chave.tipo) > 0) {
            (*comparacoes)++; // Contabiliza comparações adicionais dentro do while
            vetor[j + 1] = vetor[j];
            j = j - 1;
        }
        vetor[j + 1] = chave;
    }
}

// 3. Selection Sort - Ordenar por PRIORIDADE (Decrescente: Maior prioridade primeiro)
// Nota: O requisito pedia "Selection Sort por prioridade". 
// Assumimos que prioridade 10 é maior que 1, então ordenamos do maior para o menor para facilitar a "fuga".
// Se quiser ordem crescente (1, 2, 3...), basta mudar o sinal de < para > no if.
void selectionSortPrioridade(Componente vetor[], int qtd, long *comparacoes) {
    int i, j, min_idx;
    Componente temp;

    for (i = 0; i < qtd - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < qtd; j++) {
            (*comparacoes)++; // Contabiliza comparação
            // Ordenando de forma DECRESCENTE (maior prioridade no topo)
            if (vetor[j].prioridade > vetor[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = vetor[i];
            vetor[i] = vetor[min_idx];
            vetor[min_idx] = temp;
        }
    }
}

// 4. Busca Binária - Por NOME
// Requisito: Vetor deve estar ordenado por nome
int buscaBinariaPorNome(Componente vetor[], int qtd, char chave[]) {
    int inicio = 0;
    int fim = qtd - 1;
    int meio;
    int cmp;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        cmp = strcmp(chave, vetor[meio].nome);

        if (cmp == 0) {
            return meio; // Encontrou
        } else if (cmp < 0) {
            fim = meio - 1; // Busca na metade esquerda
        } else {
            inicio = meio + 1; // Busca na metade direita
        }
    }
    return -1; // Não encontrou
}

// Função utilitária para preencher dados automaticamente (para testes rápidos)
void preencherDadosTeste(Componente vetor[], int *qtd) {
    if (*qtd + 5 > MAX_COMPONENTES) {
        printf("Não há espaço suficiente para dados de teste.\n");
        return;
    }
    
    strcpy(vetor[*qtd].nome, "Gerador de Plasma");
    strcpy(vetor[*qtd].tipo, "Energia");
    vetor[*qtd].prioridade = 10;
    (*qtd)++;

    strcpy(vetor[*qtd].nome, "Antena Longo Alcance");
    strcpy(vetor[*qtd].tipo, "Comunicacao");
    vetor[*qtd].prioridade = 8;
    (*qtd)++;

    strcpy(vetor[*qtd].nome, "Chip Central");
    strcpy(vetor[*qtd].tipo, "Controle");
    vetor[*qtd].prioridade = 9;
    (*qtd)++;

    strcpy(vetor[*qtd].nome, "Bateria Auxiliar");
    strcpy(vetor[*qtd].tipo, "Suporte");
    vetor[*qtd].prioridade = 5;
    (*qtd)++;

    strcpy(vetor[*qtd].nome, "Propulsor Ionico");
    strcpy(vetor[*qtd].tipo, "Propulsao");
    vetor[*qtd].prioridade = 7;
    (*qtd)++;
}
