#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX_PEDIDOS 100

const float PONTOS_PARA_REAL = 0.5;
const float TAXA_PROMOCIONAL = 0.20;
const float TAXA_NORMAL = 0.05;

typedef enum { PIX, CREDITO, DEBITO, DINHEIRO } FormaDePagamento;
typedef enum { NORMAL, PROMOCIONAL } TipoDePedido;

typedef struct {
  char nomeDoCliente[50];
  char dataDoPedido[11];
  float valorOriginalDoPedido;
  char nomeDoItem[50];
  FormaDePagamento formaDePagamento;
  TipoDePedido tipoPedido;
  float taxaDePontosAnual;
  float taxaDeEntregaAplicada;
  float pontosResgatados;
  float valorTotalPago;
} Pedido;

Pedido pedidos[MAX_PEDIDOS];
int total_pedidos = 0;
char dataAtualGlobal[11] = "24/06/2025";

void limparBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void trim(char *str) {
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';
  char *start = str;
  while (*start && isspace((unsigned char)*start)) start++;
  memmove(str, start, strlen(start) + 1);
}

void formatarData(char *entrada, char *saida) {
  char numeros[9]; int j = 0;
  for (int i = 0; entrada[i] != '\0'; i++)
    if (isdigit(entrada[i]) && j < 8) numeros[j++] = entrada[i];
  numeros[j] = '\0';
  if (strlen(numeros) != 8) strcpy(saida, "00/00/0000");
  else snprintf(saida, 11, "%.2s/%.2s/%.4s", numeros, numeros + 2, numeros + 4);
}

const char *getFormaDePagamentoString(FormaDePagamento fp) {
  switch (fp) {
    case PIX: return "PIX";
    case CREDITO: return "Credito";
    case DEBITO: return "Debito";
    case DINHEIRO: return "Dinheiro";
    default: return "N/A";
  }
}

const char *getTipoPedidoString(TipoDePedido tp) {
  switch (tp) {
    case NORMAL: return "Normal";
    case PROMOCIONAL: return "Promocional";
    default: return "N/A";
  }
}

time_t converterDataParaTimestamp(const char *dataStr) {
  struct tm tm = {0};
  if (sscanf(dataStr, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) == 3) {
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    return mktime(&tm);
  }
  return -1;
}

const char *encontrarDataPrimeiroPedido(const char *nomeCliente, int limiteIndex) {
  const char *dataMaisAntiga = NULL;
  time_t timestampMaisAntigo = -1;
  for (int i = 0; i < limiteIndex; i++) {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0) {
      time_t timestampAtual = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (timestampAtual != -1 && (dataMaisAntiga == NULL || timestampAtual < timestampMaisAntigo)) {
        timestampMaisAntigo = timestampAtual;
        dataMaisAntiga = pedidos[i].dataDoPedido;
      }
    }
  }
  return dataMaisAntiga;
}

float calcularTotalPontosGerados(const char *nomeCliente, time_t dataLimite) {
  float total = 0;
  for (int i = 0; i < total_pedidos; i++) {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0) {
      time_t t = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (t >= dataLimite || t == -1) continue;
      int dias = difftime(dataLimite, t) / (60 * 60 * 24);
      if (dias > 0 && pedidos[i].taxaDePontosAnual > 0) {
        double fator = pow(1.0 + pedidos[i].taxaDePontosAnual, 1.0 / 365.0);
        double acumulado = pedidos[i].valorOriginalDoPedido * pow(fator, dias);
        total += (acumulado - pedidos[i].valorOriginalDoPedido);
      }
    }
  }
  return total;
}

float calcularTotalPontosResgatados(const char *nomeCliente, int limiteIndex) {
  float total = 0;
  for (int i = 0; i < limiteIndex; i++)
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
      total += pedidos[i].pontosResgatados;
  return total;
}

void adicionarPedido() {
  if (total_pedidos >= MAX_PEDIDOS) {
    printf("Limite de pedidos atingido!\n");
    return;
  }

  Pedido *p = &pedidos[total_pedidos];
  char data_input[20];

  printf("Nome do Cliente: ");
  scanf(" %[^\n]", p->nomeDoCliente); limparBuffer(); trim(p->nomeDoCliente);

  printf("Data do pedido (DD/MM/AAAA): ");
  scanf(" %[^\n]", data_input); limparBuffer(); formatarData(data_input, p->dataDoPedido);
  time_t t_novo_pedido = converterDataParaTimestamp(p->dataDoPedido);
  if (t_novo_pedido == -1) { printf("Data inválida!\n"); return; }

  printf("Forma de Pagamento (0:PIX, 1:Credito, 2:Debito, 3:Dinheiro): ");
  int fp_in; scanf("%d", &fp_in); p->formaDePagamento = (FormaDePagamento)fp_in; limparBuffer();

  printf("Nome do item: ");
  scanf(" %[^\n]", p->nomeDoItem); limparBuffer();

  printf("Tipo de Pedido (0: Normal, 1: Promocional): ");
  int tipo_in; scanf("%d", &tipo_in); p->tipoPedido = (TipoDePedido)tipo_in; limparBuffer();
  p->taxaDePontosAnual = (p->tipoPedido == PROMOCIONAL) ? TAXA_PROMOCIONAL : TAXA_NORMAL;

  printf("Valor Original (R$): ");
  scanf("%f", &p->valorOriginalDoPedido); limparBuffer();

  const char *dataPrimeiroPedidoStr = encontrarDataPrimeiroPedido(p->nomeDoCliente, total_pedidos);
  float taxa_base = 15.0;
  if (dataPrimeiroPedidoStr) {
    time_t t_primeiro = converterDataParaTimestamp(dataPrimeiroPedidoStr);
    int dias = difftime(t_novo_pedido, t_primeiro) / (60 * 60 * 24);
    if (dias > 180) taxa_base = 5.00;
    else if (dias > 90) taxa_base = 8.00;
    else if (dias > 30) taxa_base = 12.00;
  }
  p->taxaDeEntregaAplicada = taxa_base;

  float total_gerado = calcularTotalPontosGerados(p->nomeDoCliente, t_novo_pedido);
  float total_resgatado = calcularTotalPontosResgatados(p->nomeDoCliente, total_pedidos);
  float saldo = total_gerado - total_resgatado;
  if (saldo < 0) saldo = 0;
  float desconto_disponivel = saldo * PONTOS_PARA_REAL;
  p->pontosResgatados = 0;

  if (desconto_disponivel >= 0.01) {
    int usar;
    printf("\nCliente tem %.0f pontos (R$ %.2f). Usar? (1: SIM, 0: NAO): ", saldo, desconto_disponivel);
    scanf("%d", &usar); limparBuffer();
    if (usar == 1) {
      float desc = fmin(desconto_disponivel, p->taxaDeEntregaAplicada);
      p->pontosResgatados = desc / PONTOS_PARA_REAL;
    }
  }

  float desconto_aplicado = p->pontosResgatados * PONTOS_PARA_REAL;
  float taxa_final = p->taxaDeEntregaAplicada - desconto_aplicado;
  p->valorTotalPago = p->valorOriginalDoPedido + taxa_final;

  printf("\nResumo:\nEntrega: R$ %.2f\nDesconto: R$ %.2f\nTotal: R$ %.2f\n",
         p->taxaDeEntregaAplicada, desconto_aplicado, p->valorTotalPago);

  total_pedidos++;
  printf("Pedido adicionado!\n");
}

void exibirPedidos() {
  printf("\n================ HISTÓRICO DE PEDIDOS ================\n");
  for (int i = 0; i < total_pedidos; i++) {
    Pedido p = pedidos[i];
    printf("[%d] %s | %s | %s | %s | %.2f | Tx: %.2f | Pts: %.0f | Total: R$ %.2f\n",
           i, p.nomeDoCliente, p.dataDoPedido, p.nomeDoItem,
           getFormaDePagamentoString(p.formaDePagamento),
           p.valorOriginalDoPedido, p.taxaDeEntregaAplicada,
           p.pontosResgatados, p.valorTotalPago);
  }
  printf("======================================================\n");
}

void imprimirValorBrutoTotal() {
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
    total += pedidos[i].valorOriginalDoPedido;
  printf("Total bruto dos pedidos: R$ %.2f\n", total);
}

void imprimirValorTotalAPagar() {
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
    total += pedidos[i].valorTotalPago;
  printf("Total pago pelos clientes: R$ %.2f\n", total);
}

int compararPorData(const void *a, const void *b) {
  return strcmp(((Pedido *)a)->dataDoPedido, ((Pedido *)b)->dataDoPedido);
}
int compararPorFormaDePagamento(const void *a, const void *b) {
  return ((Pedido *)a)->formaDePagamento - ((Pedido *)b)->formaDePagamento;
}
int compararPorValor(const void *a, const void *b) {
  float diff = ((Pedido *)a)->valorOriginalDoPedido - ((Pedido *)b)->valorOriginalDoPedido;
  return (diff > 0) - (diff < 0);
}

void ordenarPedidos(int criterio) {
  if (criterio == 1) qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorData);
  else if (criterio == 2) qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorFormaDePagamento);
  else if (criterio == 3) qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorValor);
  printf("Pedidos ordenados.\n"); exibirPedidos();
}

void agruparPorFormaDePagamentoOrdenar(int porValor) {
  qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorFormaDePagamento);
  int i = 0;
  while (i < total_pedidos) {
    int j = i + 1;
    while (j < total_pedidos && pedidos[i].formaDePagamento == pedidos[j].formaDePagamento) j++;
    if (porValor)
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorValor);
    else
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorData);
    i = j;
  }
  printf("Pedidos agrupados e ordenados.\n"); exibirPedidos();
}

void editarPedido() {
  if (total_pedidos == 0) { printf("Nada para editar.\n"); return; }
  exibirPedidos();
  int index;
  printf("Número do pedido a editar: ");
  scanf("%d", &index); limparBuffer();
  if (index < 0 || index >= total_pedidos) return;
  total_pedidos--; 
  adicionarPedido(); 
}

void deletarPedido() {
  if (total_pedidos == 0) { printf("Nada para deletar.\n"); return; }
  exibirPedidos();
  int index;
  printf("Número do pedido a deletar: ");
  scanf("%d", &index); limparBuffer();
  if (index < 0 || index >= total_pedidos) return;
  for (int i = index; i < total_pedidos - 1; i++)
    pedidos[i] = pedidos[i + 1];
  total_pedidos--;
  printf("Pedido removido.\n");
}

void menuPrincipal() {
  int op;
  do {
    printf("\n=== MENU ===\n");
    printf("1. Adicionar pedido\n2. Exibir pedidos\n3. Faturamento bruto\n4. Faturamento total\n");
    printf("5. Ordenar por data\n6. Ordenar por forma\n7. Ordenar por valor\n");
    printf("8. Agrupar forma/valor\n9. Agrupar forma/data\n10. Editar pedido\n11. Deletar pedido\n0. Sair\nEscolha: ");
    scanf("%d", &op); limparBuffer();

    switch (op) {
      case 1: adicionarPedido(); break;
      case 2: exibirPedidos(); break;
      case 3: imprimirValorBrutoTotal(); break;
      case 4: imprimirValorTotalAPagar(); break;
      case 5: ordenarPedidos(1); break;
      case 6: ordenarPedidos(2); break;
      case 7: ordenarPedidos(3); break;
      case 8: agruparPorFormaDePagamentoOrdenar(1); break;
      case 9: agruparPorFormaDePagamentoOrdenar(0); break;
      case 10: editarPedido(); break;
      case 11: deletarPedido(); break;
      case 0: printf("Encerrando...\n"); break;
      default: printf("Opção inválida!\n");
    }
  } while (op != 0);
}

int main() {
  menuPrincipal();
  return 0;
}
