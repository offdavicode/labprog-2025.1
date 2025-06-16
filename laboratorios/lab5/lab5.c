#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PEDIDOS 100

typedef struct {
    char data[11];
    float valor;
    char tipo[30];
    char nome[50];
} Pedido;

Pedido pedidos[MAX_PEDIDOS];
int total_pedidos = 0;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void formatarData(char *entrada, char *saida) {
    char numeros[9];
    int j = 0;

    for (int i = 0; entrada[i] != '\0'; i++) {
        if (isdigit(entrada[i]) && j < 8) {
            numeros[j++] = entrada[i];
        }
    }
    numeros[j] = '\0';

    if (strlen(numeros) != 8) {
        strcpy(saida, "00/00/0000");
        return;
    }

    snprintf(saida, 11, "%.2s/%.2s/%.4s", numeros, numeros + 2, numeros + 4);
}

void adicionarPedido() {
    if (total_pedidos >= MAX_PEDIDOS) {
        printf("Limite de pedidos atingido!\n");
        return;
    }

    char data_input[20], data_formatada[11];
    float valor;
    char tipo[30], nome[50];

    printf("Data do pedido: ");
    scanf(" %[^\n]", data_input);
    limparBuffer();
    formatarData(data_input, data_formatada);

    printf("Tipo de comida: ");
    scanf(" %[^\n]", tipo);
    limparBuffer();

    printf("Nome do item: ");
    scanf(" %[^\n]", nome);
    limparBuffer();

    printf("Valor (R$): ");
    scanf("%f", &valor);
    limparBuffer();

    Pedido *p = &pedidos[total_pedidos++];
    strcpy(p->data, data_formatada);
    p->valor = valor;
    strcpy(p->tipo, tipo);
    strcpy(p->nome, nome);

    printf("Pedido adicionado com sucesso!\n");
}

void exibirPedidos() {
    printf("\nPedidos registrados:\n");
    printf("Data       | Valor   | Tipo             | Nome\n");
    printf("-----------|---------|------------------|----------------\n");
    for (int i = 0; i < total_pedidos; i++) {
        Pedido p = pedidos[i];
        printf("%-10s | R$%-6.2f | %-16s | %-16s\n", p.data, p.valor, p.tipo, p.nome);
    }
}

int compararPorData(const void *a, const void *b) {
    return strcmp(((Pedido *)a)->data, ((Pedido *)b)->data);
}

int compararPorTipo(const void *a, const void *b) {
    return strcmp(((Pedido *)a)->tipo, ((Pedido *)b)->tipo);
}

int compararPorValor(const void *a, const void *b) {
    float diff = ((Pedido *)a)->valor - ((Pedido *)b)->valor;
    return (diff > 0) - (diff < 0);
}

void ordenarPedidos(int criterio) {
    if (criterio == 1)
        qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorData);
    else if (criterio == 2)
        qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorTipo);
    else if (criterio == 3)
        qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorValor);
    exibirPedidos();
}

void agruparPorTipoOrdenar(int porValor) {
    qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorTipo);
    int i = 0;
    while (i < total_pedidos) {
        int j = i + 1;
        while (j < total_pedidos && strcmp(pedidos[i].tipo, pedidos[j].tipo) == 0) {
            j++;
        }
        if (porValor)
            qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorValor);
        else
            qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorData);
        i = j;
    }
    exibirPedidos();
}

void menuPrincipal() {
    int op;
    do {
        printf("\n==== SISTEMA DE PEDIDOS ====\n");
        printf("1. Adicionar pedido\n");
        printf("2. Exibir todos os pedidos\n");
        printf("3. Ordenar por data\n");
        printf("4. Ordenar por tipo\n");
        printf("5. Ordenar por valor\n");
        printf("6. Agrupar por tipo e ordenar por valor\n");
        printf("7. Agrupar por tipo e ordenar por data\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: adicionarPedido(); break;
            case 2: exibirPedidos(); break;
            case 3: ordenarPedidos(1); break;
            case 4: ordenarPedidos(2); break;
            case 5: ordenarPedidos(3); break;
            case 6: agruparPorTipoOrdenar(1); break;
            case 7: agruparPorTipoOrdenar(0); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (op != 0);
}

int main() {
    menuPrincipal();
    return 0;
}