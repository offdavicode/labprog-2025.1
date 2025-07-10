#include <stdio.h>
#include "pedido.h"
#include "persistencia.h"
#include "util.h"

void menuPrincipal()
{
  int op;
  do
  {
    printf("\n=== MENU ===\n");
    printf("1. Adicionar pedido\n2. Exibir pedidos\n3. Faturamento bruto\n4. Faturamento total\n");
    printf("5. Ordenar por data\n6. Ordenar por forma\n7. Ordenar por valor\n");
    printf("8. Agrupar forma/valor\n9. Agrupar forma/data\n10. Editar pedido\n11. Deletar pedido\n0. Sair\nEscolha: ");
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
    case 10:
      editarPedido();
      break;
    case 11:
      deletarPedido();
      break;
    case 0:
      printf("Encerrando...\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (op != 0);
}