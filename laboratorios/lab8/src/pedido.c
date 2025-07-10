#include "pedido.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PONTOS_PARA_REAL 0.5
#define TAXA_PROMOCIONAL 0.20
#define TAXA_NORMAL 0.05

Pedido pedidos[MAX_PEDIDOS];
int total_pedidos = 0;
char dataAtualGlobal[11] = "24/06/2025";

const char *getFormaDePagamentoString(FormaDePagamento fp)
{
  switch (fp)
  {
  case PIX:
    return "PIX";
  case CREDITO:
    return "Credito";
  case DEBITO:
    return "Debito";
  case DINHEIRO:
    return "Dinheiro";
  default:
    return "N/A";
  }
}

const char *getTipoPedidoString(TipoDePedido tp)
{
  switch (tp)
  {
  case NORMAL:
    return "Normal";
  case PROMOCIONAL:
    return "Promocional";
  default:
    return "N/A";
  }
}

time_t converterDataParaTimestamp(const char *dataStr);

const char *encontrarDataPrimeiroPedido(const char *nomeCliente, int limiteIndex)
{
  const char *dataMaisAntiga = NULL;
  time_t timestampMaisAntigo = -1;
  for (int i = 0; i < limiteIndex; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
    {
      time_t t = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (t != -1 && (dataMaisAntiga == NULL || t < timestampMaisAntigo))
      {
        timestampMaisAntigo = t;
        dataMaisAntiga = pedidos[i].dataDoPedido;
      }
    }
  }
  return dataMaisAntiga;
}

float calcularTotalPontosGerados(const char *nomeCliente, time_t dataLimite)
{
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
    {
      time_t t = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (t >= dataLimite || t == -1)
        continue;
      int dias = difftime(dataLimite, t) / (60 * 60 * 24);
      if (dias > 0 && pedidos[i].taxaDePontosAnual > 0)
      {
        double fator = pow(1.0 + pedidos[i].taxaDePontosAnual, 1.0 / 365.0);
        double acumulado = pedidos[i].valorOriginalDoPedido * pow(fator, dias);
        total += (acumulado - pedidos[i].valorOriginalDoPedido);
      }
    }
  }
  return total;
}

float calcularTotalPontosResgatados(const char *nomeCliente, int limiteIndex)
{
  float total = 0;
  for (int i = 0; i < limiteIndex; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
      total += pedidos[i].pontosResgatados;
  }
  return total;
}

void adicionarPedido()
{
  if (total_pedidos >= MAX_PEDIDOS)
  {
    printf("Limite de pedidos atingido!\n");
    return;
  }

  Pedido *p = &pedidos[total_pedidos];
  char data_input[20];

  printf("Nome do Cliente: ");
  scanf(" %[^\n]", p->nomeDoCliente);
  limparBuffer();
  trim(p->nomeDoCliente);

  printf("Data do pedido (DD/MM/AAAA): ");
  scanf(" %[^\n]", data_input);
  limparBuffer();
  formatarData(data_input, p->dataDoPedido);
  time_t t_novo_pedido = converterDataParaTimestamp(p->dataDoPedido);
  if (t_novo_pedido == -1)
  {
    printf("Data inválida!\n");
    return;
  }

  printf("Forma de Pagamento (0:PIX, 1:Credito, 2:Debito, 3:Dinheiro): ");
  int forma;
  scanf("%d", &forma);
  limparBuffer();
  p->formaDePagamento = (FormaDePagamento)forma;

  printf("Nome do item: ");
  scanf(" %[^\n]", p->nomeDoItem);
  limparBuffer();

  printf("Tipo de Pedido (0: Normal, 1: Promocional): ");
  int tipo;
  scanf("%d", &tipo);
  limparBuffer();
  p->tipoPedido = (TipoDePedido)tipo;
  p->taxaDePontosAnual = (p->tipoPedido == PROMOCIONAL) ? TAXA_PROMOCIONAL : TAXA_NORMAL;

  printf("Valor Original (R$): ");
  scanf("%f", &p->valorOriginalDoPedido);
  limparBuffer();

  const char *dataPrimeiro = encontrarDataPrimeiroPedido(p->nomeDoCliente, total_pedidos);
  float taxa_base = 15.0;
  if (dataPrimeiro)
  {
    time_t t_primeiro = converterDataParaTimestamp(dataPrimeiro);
    int dias = difftime(t_novo_pedido, t_primeiro) / (60 * 60 * 24);
    if (dias > 180)
      taxa_base = 5.00;
    else if (dias > 90)
      taxa_base = 8.00;
    else if (dias > 30)
      taxa_base = 12.00;
  }
  p->taxaDeEntregaAplicada = taxa_base;

  float total_gerado = calcularTotalPontosGerados(p->nomeDoCliente, t_novo_pedido);
  float resgatado = calcularTotalPontosResgatados(p->nomeDoCliente, total_pedidos);
  float saldo = total_gerado - resgatado;

  if (saldo < 0)
    saldo = 0;

  float desconto = saldo * PONTOS_PARA_REAL;
  p->pontosResgatados = 0;
  if (desconto >= 0.01)
  {
    int usar;
    printf("\nCliente tem %.0f pontos (R$ %.2f). Usar? (1: SIM, 0: NAO): ", saldo, desconto);
    scanf("%d", &usar);
    limparBuffer();
    if (usar == 1)
    {
      float desc = fmin(desconto, p->taxaDeEntregaAplicada);
      p->pontosResgatados = desc / PONTOS_PARA_REAL;
    }
  }

  float aplicado = p->pontosResgatados * PONTOS_PARA_REAL;
  float taxa_final = p->taxaDeEntregaAplicada - aplicado;
  p->valorTotalPago = p->valorOriginalDoPedido + taxa_final;

  printf("\nResumo:\nEntrega: R$ %.2f\nDesconto: R$ %.2f\nTotal: R$ %.2f\n",
         p->taxaDeEntregaAplicada, aplicado, p->valorTotalPago);

  total_pedidos++;
  printf("Pedido adicionado!\n");
}

void exibirPedidos()
{
  printf("\n=========== HISTÓRICO DE PEDIDOS ===========\n");
  for (int i = 0; i < total_pedidos; i++)
  {
    Pedido p = pedidos[i];
    printf("[%d] %s | %s | %s | %s | %.2f | Tx: %.2f | Pts: %.0f | Total: R$ %.2f\n",
           i, p.nomeDoCliente, p.dataDoPedido, p.nomeDoItem,
           getFormaDePagamentoString(p.formaDePagamento),
           p.valorOriginalDoPedido, p.taxaDeEntregaAplicada,
           p.pontosResgatados, p.valorTotalPago);
  }
  printf("============================================\n");
}

void imprimirValorBrutoTotal()
{
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
    total += pedidos[i].valorOriginalDoPedido;
  printf("Total bruto dos pedidos: R$ %.2f\n", total);
}

void imprimirValorTotalAPagar()
{
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
    total += pedidos[i].valorTotalPago;
  printf("Total pago pelos clientes: R$ %.2f\n", total);
}

int compararPorData(const void *a, const void *b)
{
  time_t x = converterDataParaTimestamp(((Pedido *)a)->dataDoPedido);
  time_t y = converterDataParaTimestamp(((Pedido *)b)->dataDoPedido);
  return (x > y) - (x < y);
}

int compararPorFormaDePagamento(const void *a, const void *b)
{
  return ((Pedido *)a)->formaDePagamento - ((Pedido *)b)->formaDePagamento;
}

int compararPorValor(const void *a, const void *b)
{
  float diff = ((Pedido *)a)->valorOriginalDoPedido - ((Pedido *)b)->valorOriginalDoPedido;
  return (diff > 0) - (diff < 0);
}

void ordenarPedidos(int criterio)
{
  if (criterio == 1)
    qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorData);
  else if (criterio == 2)
    qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorFormaDePagamento);
  else if (criterio == 3)
    qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorValor);
  printf("Pedidos ordenados.\n");
  exibirPedidos();
}

void agruparPorFormaDePagamentoOrdenar(int porValor)
{
  qsort(pedidos, total_pedidos, sizeof(Pedido), compararPorFormaDePagamento);
  int i = 0;
  while (i < total_pedidos)
  {
    int j = i + 1;
    while (j < total_pedidos && pedidos[i].formaDePagamento == pedidos[j].formaDePagamento)
      j++;
    if (porValor)
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorValor);
    else
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorData);
    i = j;
  }
  printf("Pedidos agrupados e ordenados.\n");
  exibirPedidos();
}

void editarPedido()
{
  if (total_pedidos == 0)
  {
    printf("Nada para editar.\n");
    return;
  }
  exibirPedidos();
  int index;
  printf("Número do pedido a editar: ");
  scanf("%d", &index);
  limparBuffer();
  if (index < 0 || index >= total_pedidos)
    return;
  total_pedidos--;
  adicionarPedido();
}

void deletarPedido()
{
  if (total_pedidos == 0)
  {
    printf("Nada para deletar.\n");
    return;
  }
  exibirPedidos();
  int index;
  printf("Número do pedido a deletar: ");
  scanf("%d", &index);
  limparBuffer();
  if (index < 0 || index >= total_pedidos)
    return;
  for (int i = index; i < total_pedidos - 1; i++)
    pedidos[i] = pedidos[i + 1];
  total_pedidos--;
  printf("Pedido removido.\n");
}
