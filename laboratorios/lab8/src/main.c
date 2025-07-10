#include "menu.h"
#include "persistencia.h"

int main()
{
  carregarPedidosDoArquivo();
  menuPrincipal();
  salvarPedidosEmArquivo();
  return 0;
}