/*
 * Nome: Augusto Bento Carvalho
 *
 * Sistema de Inventário - Mochila de Sobrevivência
 * 
 * Objetivo: Gerenciar itens coletados usando Structs e Lista Sequencial (Vetor).
 * Estrutura de Dados: Vetor estático de tamanho 10.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10 // Capacidade máxima da mochila

// Definição da struct Item (Agregador de dados)a
typedef struct {
    char nome[30];
    char tipo[20];     // Ex: arma, munição, Medicamentos
    int quantidade;
} Item;

// --- Funções Auxiliares ---

// Limpa o buffer do teclado para evitar erros entre scanf e fgets
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove o caractere '\n' (Enter) que o fgets captura
void removerEnter(char *str) {
    str[strcspn(str, "\n")] = 0;
}

// --- Funções Principais do Sistema ---

/*
 * Função: inserirItem
 * Adiciona um item na primeira posição livre do vetor.
 * Parâmetros: vetor de itens e ponteiro para a quantidade atual.
 */
void inserirItem(Item mochila[], int *qtdAtual) {
    if (*qtdAtual >= MAX_ITENS) {
        printf("\n>>> Mochila cheia! (Max: %d)\n", MAX_ITENS);
        return;
    }

    printf("\n--- Novo Item ---\n");
    
    printf("Nome: ");
    fgets(mochila[*qtdAtual].nome, 30, stdin);
    removerEnter(mochila[*qtdAtual].nome);

    printf("Tipo (arma/munição/Medicamentos): ");
    fgets(mochila[*qtdAtual].tipo, 20, stdin);
    removerEnter(mochila[*qtdAtual].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[*qtdAtual].quantidade);
    limparBuffer(); // Importante após ler inteiros

    (*qtdAtual)++; // Incrementa o contador de itens na mochila
    printf(">>> Item cadastrado com sucesso!\n");
}

/*
 * Função: listarItens
 * Percorre o vetor e exibe todos os itens válidos.
 */
void listarItens(Item mochila[], int qtdAtual) {
    printf("\n=== Inventário (%d/%d) ===\n", qtdAtual, MAX_ITENS);
    if (qtdAtual == 0) {
        printf("(Vazio)\n");
    } else {
        printf("%-20s | %-15s | %s\n", "Nome", "Tipo", "Qtd");
        printf("------------------------------------------------\n");
        for (int i = 0; i < qtdAtual; i++) {
            printf("%-20s | %-15s | %d\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("==========================\n");
}

/*
 * Função: removerItem
 * Busca um item pelo nome e o remove, reorganizando o vetor (shift left).
 */
void removerItem(Item mochila[], int *qtdAtual) {
    char nomeBusca[30];
    printf("\nNome do item para remover: ");
    fgets(nomeBusca, 30, stdin);
    removerEnter(nomeBusca);

    int indice = -1;

    // Busca Sequencial para achar o índice
    for (int i = 0; i < *qtdAtual; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        // Desloca os itens da direita para a esquerda para tapar o buraco
        for (int i = indice; i < *qtdAtual - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*qtdAtual)--; // Decrementa o total de itens
        printf(">>> Item removido!\n");
    } else {
        printf(">>> Item não encontrado.\n");
    }
}

/*
 * Função: buscarItem
 * Busca sequencial simples para exibir detalhes de um item específico.
 */
void buscarItem(Item mochila[], int qtdAtual) {
    char nomeBusca[30];
    printf("\nBuscar por nome: ");
    fgets(nomeBusca, 30, stdin);
    removerEnter(nomeBusca);

    for (int i = 0; i < qtdAtual; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n>>> Item Encontrado:\n");
            printf("Nome: %s\nTipo: %s\nQtd:  %d\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }
    printf(">>> Item não encontrado na mochila.\n");
}

int main() {

    Item mochila[MAX_ITENS]; // Vetor de structs (Lista Sequencial)
    int qtdItens = 0;        // Controla quantos itens existem atualmente
    int opcao;

    printf("======= Mochila de Sobrevivência =======\n");

    do {
        printf("\n1. Cadastrar Item\n2. Remover Item\n3. Listar Itens\n4. Buscar Item\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: inserirItem(mochila, &qtdItens); listarItens(mochila, qtdItens); break;
            case 2: removerItem(mochila, &qtdItens); listarItens(mochila, qtdItens); break;
            case 3: listarItens(mochila, qtdItens); break;
            case 4: buscarItem(mochila, qtdItens); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
