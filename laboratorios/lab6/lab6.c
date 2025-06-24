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

typedef enum
{
  PIX,
  CREDITO,
  DEBITO,
  DINHEIRO
} FormaDePagamento;
typedef enum
{
  NORMAL,
  PROMOCIONAL
} TipoDePedido;

typedef struct
{
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

void limparBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void trim(char *str)
{
  char *end;
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
  {
    end--;
  }
  end[1] = '\0';
  char *start = str;
  while (*start != '\0' && isspace((unsigned char)*start))
  {
    start++;
  }
  memmove(str, start, strlen(start) + 1);
}

void formatarData(char *entrada, char *saida)
{
  char numeros[9];
  int j = 0;
  for (int i = 0; entrada[i] != '\0'; i++)
  {
    if (isdigit(entrada[i]) && j < 8)
    {
      numeros[j++] = entrada[i];
    }
  }
  numeros[j] = '\0';
  if (strlen(numeros) != 8)
  {
    strcpy(saida, "00/00/0000");
    return;
  }
  snprintf(saida, 11, "%.2s/%.2s/%.4s", numeros, numeros + 2, numeros + 4);
}

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

time_t converterDataParaTimestamp(const char *dataStr)
{
  struct tm tm = {0};
  if (sscanf(dataStr, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) == 3)
  {
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    return mktime(&tm);
  }
  return -1;
}

const char *encontrarDataPrimeiroPedido(const char *nomeCliente, int limiteIndex)
{
  const char *dataMaisAntiga = NULL;
  time_t timestampMaisAntigo = -1;
  for (int i = 0; i < limiteIndex; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
    {
      time_t timestampAtual = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (timestampAtual != -1)
      {
        if (dataMaisAntiga == NULL || timestampAtual < timestampMaisAntigo)
        {
          timestampMaisAntigo = timestampAtual;
          dataMaisAntiga = pedidos[i].dataDoPedido;
        }
      }
    }
  }
  return dataMaisAntiga;
}

float calcularTotalPontosGerados(const char *nomeCliente, time_t dataLimite)
{
  float totalGerado = 0;
  for (int i = 0; i < total_pedidos; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
    {
      time_t t_pedido = converterDataParaTimestamp(pedidos[i].dataDoPedido);
      if (t_pedido >= dataLimite || t_pedido == -1)
        continue;

      int diasDesdePedido = difftime(dataLimite, t_pedido) / (60 * 60 * 24);
      if (diasDesdePedido > 0 && pedidos[i].taxaDePontosAnual > 0)
      {
        double fatorDiario = pow(1.0 + pedidos[i].taxaDePontosAnual, 1.0 / 365.0);
        double valorAcumulado = pedidos[i].valorOriginalDoPedido * pow(fatorDiario, diasDesdePedido);
        totalGerado += (valorAcumulado - pedidos[i].valorOriginalDoPedido);
      }
    }
  }
  return totalGerado;
}

float calcularTotalPontosResgatados(const char *nomeCliente, int limiteIndex)
{
  float totalResgatado = 0;
  for (int i = 0; i < limiteIndex; i++)
  {
    if (strcmp(pedidos[i].nomeDoCliente, nomeCliente) == 0)
    {
      totalResgatado += pedidos[i].pontosResgatados;
    }
  }
  return totalResgatado;
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
    printf("Data inválida! Abortando pedido.\n");
    return;
  }

  printf("Forma de Pagamento (0:PIX, 1:Credito, 2:Debito, 3:Dinheiro): ");
  int fp_in;
  scanf("%d", &fp_in);
  p->formaDePagamento = (FormaDePagamento)fp_in;
  limparBuffer();

  printf("Nome do item: ");
  scanf(" %[^\n]", p->nomeDoItem);
  limparBuffer();

  printf("Tipo de Pedido (0: Normal, 1: Promocional): ");
  int tipo_in;
  scanf("%d", &tipo_in);
  p->tipoPedido = (TipoDePedido)tipo_in;
  limparBuffer();

  if (p->tipoPedido == PROMOCIONAL)
  {
    p->taxaDePontosAnual = TAXA_PROMOCIONAL;
  }
  else
  {
    p->taxaDePontosAnual = TAXA_NORMAL;
  }

  printf("Valor Original (R$): ");
  scanf("%f", &p->valorOriginalDoPedido);
  limparBuffer();

  const char *dataPrimeiroPedidoStr = encontrarDataPrimeiroPedido(p->nomeDoCliente, total_pedidos);
  float taxa_base = 15.00;
  if (dataPrimeiroPedidoStr)
  {
    time_t t_primeiro = converterDataParaTimestamp(dataPrimeiroPedidoStr);
    int diasCorridos = difftime(t_novo_pedido, t_primeiro) / (60 * 60 * 24);

    if (diasCorridos > 180)
      taxa_base = 5.00;
    else if (diasCorridos > 90)
      taxa_base = 8.00;
    else if (diasCorridos > 30)
      taxa_base = 12.00;
    else
      taxa_base = 15.00;
  }
  p->taxaDeEntregaAplicada = taxa_base;

  float total_gerado = calcularTotalPontosGerados(p->nomeDoCliente, t_novo_pedido);
  float total_resgatado_anteriormente = calcularTotalPontosResgatados(p->nomeDoCliente, total_pedidos);
  float saldo_atual_pontos = total_gerado - total_resgatado_anteriormente;
  if (saldo_atual_pontos < 0)
    saldo_atual_pontos = 0;

  float desconto_disponivel_reais = saldo_atual_pontos * PONTOS_PARA_REAL;
  p->pontosResgatados = 0;

  if (desconto_disponivel_reais >= 0.01)
  {
    int usar_pontos;
    printf("\n---------------------------------------------------\n");
    printf("Saldo do cliente: %.0f pontos (equivale a R$ %.2f).\n", saldo_atual_pontos, desconto_disponivel_reais);
    printf("Deseja usar os pontos para abater na entrega? (1 para SIM / 0 para NAO): ");
    scanf("%d", &usar_pontos);
    limparBuffer();

    if (usar_pontos == 1)
    {
      float desconto_em_reais = fmin(desconto_disponivel_reais, p->taxaDeEntregaAplicada);
      p->pontosResgatados = desconto_em_reais / PONTOS_PARA_REAL;
    }
  }

  float desconto_aplicado_reais = p->pontosResgatados * PONTOS_PARA_REAL;
  float taxa_final = p->taxaDeEntregaAplicada - desconto_aplicado_reais;
  p->valorTotalPago = p->valorOriginalDoPedido + taxa_final;

  printf("\n--- Resumo da Transacao ---\n");
  printf("Taxa de Entrega Base (por fidelidade): R$ %.2f\n", p->taxaDeEntregaAplicada);
  if (p->pontosResgatados > 0)
  {
    printf("Desconto dos Pontos Aplicado: R$ -%.2f (%.0f pontos)\n", desconto_aplicado_reais, p->pontosResgatados);
  }
  printf("Valor Final da Entrega: R$ %.2f\n", taxa_final);
  printf("TOTAL A PAGAR (Pedido + Entrega): R$ %.2f\n", p->valorTotalPago);
  printf("---------------------------\n");

  total_pedidos++;
  printf("\nPedido FINALIZADO e adicionado com sucesso!\n");
}

void exibirPedidos()
{
  printf("\n====================================================== HISTORICO DE PEDIDOS FINALIZADOS ======================================================\n");
  printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
  printf("%-20s | %-10s | %-15s | %-11s | %-10s | %-10s | %-12s | %-15s | %-12s\n",
         "Cliente", "Data", "Item", "Tipo", "Pagamento", "V. Orig.", "Tx. Aplicada", "Pts. Resgatados", "TOTAL PAGO");
  printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");

  for (int i = 0; i < total_pedidos; i++)
  {
    Pedido p = pedidos[i];
    printf("%-20s | %-10s | %-15s | %-11s | %-10s | R$%-8.2f | R$%-10.2f | %-15.0f | R$%-10.2f\n",
           p.nomeDoCliente,
           p.dataDoPedido,
           p.nomeDoItem,
           getTipoPedidoString(p.tipoPedido),
           getFormaDePagamentoString(p.formaDePagamento),
           p.valorOriginalDoPedido,
           p.taxaDeEntregaAplicada,
           p.pontosResgatados,
           p.valorTotalPago);
  }
  printf("==============================================================================================================================================\n");
}

void imprimirValorBrutoTotal()
{
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
  {
    total += pedidos[i].valorOriginalDoPedido;
  }
  printf("\n>>> SOMA DE TODOS OS VALORES ORIGINAIS DOS PEDIDOS: R$ %.2f\n", total);
}

void imprimirValorTotalAPagar()
{
  float total = 0;
  for (int i = 0; i < total_pedidos; i++)
  {
    total += pedidos[i].valorTotalPago;
  }
  printf("\n>>> SOMA DE TODOS OS VALORES TOTAIS PAGOS: R$ %.2f\n", total);
}

int compararPorData(const void *a, const void *b)
{
  return strcmp(((Pedido *)a)->dataDoPedido, ((Pedido *)b)->dataDoPedido);
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
  printf("\nPedidos ordenados com sucesso!\n");
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
    {
      j++;
    }
    if (porValor)
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorValor);
    else
      qsort(&pedidos[i], j - i, sizeof(Pedido), compararPorData);
    i = j;
  }
  printf("\nPedidos agrupados por forma de pagamento e reordenados!\n");
  exibirPedidos();
}

void menuPrincipal()
{
  int op;
  do
  {
    printf("\n==== SISTEMA DE PEDIDOS ====\n");
    printf(" 1. Adicionar e Finalizar Pedido\n");
    printf(" 2. Exibir Histórico de Pedidos\n");
    printf(" 3. Exibir Faturamento Bruto (soma dos pedidos)\n");
    printf(" 4. Exibir Faturamento Total (soma dos totais pagos)\n");
    printf(" 5. Ordenar por data\n");
    printf(" 6. Ordenar por forma de pagamento\n");
    printf(" 7. Ordenar por valor\n");
    printf(" 8. Agrupar por forma de pagamento e ordenar por valor\n");
    printf(" 9. Agrupar por forma de pagamento e ordenar por data\n");
    printf(" 0. Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    limparBuffer();

    switch (op)
    {
    case 1:
      adicionarPedido();
      break;
    case 2:
      exibirPedidos();
      break;
    case 3:
      imprimirValorBrutoTotal();
      break;
    case 4:
      imprimirValorTotalAPagar();
      break;
    case 5:
      ordenarPedidos(1);
      break;
    case 6:
      ordenarPedidos(2);
      break;
    case 7:
      ordenarPedidos(3);
      break;
    case 8:
      agruparPorFormaDePagamentoOrdenar(1);
      break;
    case 9:
      agruparPorFormaDePagamentoOrdenar(0);
      break;
    case 0:
      printf("Encerrando...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (op != 0);
}

int main()
{
  menuPrincipal();
  return 0;
}