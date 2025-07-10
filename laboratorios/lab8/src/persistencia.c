#include "persistencia.h"
#include "pedido.h"
#include <stdio.h>

void salvarPedidosEmArquivo()
{
  FILE *f = fopen("data/pedidos.dat", "wb");
  if (!f)
  {
    printf("Erro ao salvar pedidos (verifique se a pasta 'data/' existe).\n");
    return;
  }
  fwrite(&total_pedidos, sizeof(int), 1, f);
  fwrite(pedidos, sizeof(Pedido), total_pedidos, f);
  fclose(f);
}

void carregarPedidosDoArquivo()
{
  FILE *f = fopen("data/pedidos.dat", "rb");
  if (!f)
  {
    total_pedidos = 0;
    return;
  }
  fread(&total_pedidos, sizeof(int), 1, f);
  if (total_pedidos < 0 || total_pedidos > MAX_PEDIDOS)
  {
    printf("Erro no arquivo de dados, iniciando do zero.\n");
    total_pedidos = 0;
  }
  fread(pedidos, sizeof(Pedido), total_pedidos, f);
  fclose(f);
}
