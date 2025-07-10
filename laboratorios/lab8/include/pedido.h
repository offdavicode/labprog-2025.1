#ifndef PEDIDO_H
#define PEDIDO_H

#include <time.h>

#define MAX_PEDIDOS 100

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

extern Pedido pedidos[MAX_PEDIDOS];
extern int total_pedidos;

void adicionarPedido();
void editarPedido();
void deletarPedido();
void exibirPedidos();
void ordenarPedidos(int criterio);
void agruparPorFormaDePagamentoOrdenar(int porValor);
void imprimirValorBrutoTotal();
void imprimirValorTotalAPagar();
const char *getFormaDePagamentoString(FormaDePagamento fp);
const char *getTipoPedidoString(TipoDePedido tp);
float calcularTotalPontosGerados(const char *nomeCliente, time_t dataLimite);
float calcularTotalPontosResgatados(const char *nomeCliente, int limiteIndex);
const char *encontrarDataPrimeiroPedido(const char *nomeCliente, int limiteIndex);

#endif