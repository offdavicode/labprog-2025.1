#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
#define SENHA_INICIAL 1000

typedef struct
{
  int senhas[MAX];
  int inicio;
  int fim;
  int total;
} Fila;

Fila filaNormal, filaPrioridade;
int contadorNormal = SENHA_INICIAL;
int contadorPrioridade = SENHA_INICIAL;
int turno = 0;

void inicializarFila(Fila *fila);
int inserirFila(Fila *fila, int senha);
int removerFila(Fila *fila, int *senha);
void inserirNormal();
void inserirPrioridade();
void consumirSenha();
void visualizarFilas(Fila normal, Fila prioridade);
void menu();

int main()
{
  inicializarFila(&filaNormal);
  inicializarFila(&filaPrioridade);
  menu();
  return 0;
}

void menu()
{
  char opcao;
  do
  {
    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("\nMENU\n");
    printf("N - Inserir senha NORMAL\n");
    printf("P - Inserir senha PRIORIDADE\n");
    printf("C - Consumir senha\n");
    printf("V - Visualizar senhas\n");
    printf("S - Sair do programa\n");
    printf("\n");
    printf("Escolha uma opção: ");
    scanf(" %c", &opcao);
    printf("\n");

    switch (opcao)
    {
    case 'N':
    case 'n':
      inserirNormal();
      break;
    case 'P':
    case 'p':
      inserirPrioridade();
      break;
    case 'C':
    case 'c':
      consumirSenha();
      break;
    case 'V':
    case 'v':
      visualizarFilas(filaNormal, filaPrioridade);
      break;
    case 'S':
    case 's':
      printf("Encerrando o programa...\n");
      break;
    default:
      printf("Opção inválida. Tente inserir uma das opções disponíveis.\n");
    }
  } while (opcao != 'S' && opcao != 's');
}

void inicializarFila(Fila *fila)
{
  fila->inicio = 0;
  fila->fim = 0;
  fila->total = 0;
}

int inserirFila(Fila *fila, int senha)
{
  if (fila->total >= MAX)
  {
    return 0;
  }
  fila->senhas[fila->fim] = senha;
  fila->fim = (fila->fim + 1) % MAX;
  fila->total++;
  return 1;
}

int removerFila(Fila *fila, int *senha)
{
  if (fila->total == 0)
  {
    return 0;
  }
  *senha = fila->senhas[fila->inicio];
  fila->inicio = (fila->inicio + 1) % MAX;
  fila->total--;
  return 1;
}

void inserirNormal()
{
  if (inserirFila(&filaNormal, contadorNormal))
  {
    printf("Você recebeu a senha normal n° N%d\n", contadorNormal);
    contadorNormal++;
  }
  else
  {
    printf("Fila de senhas normais está cheia!\n");
  }
}

void inserirPrioridade()
{
  if (inserirFila(&filaPrioridade, contadorPrioridade))
  {
    printf("Você recebeu a senha prioridade n° P%d\n", contadorPrioridade);
    contadorPrioridade++;
  }
  else
  {
    printf("Fila de senhas prioritárias está cheia!\n");
  }
}

void consumirSenha()
{
  int senha;
  int consumiu = 0;

  if (turno == 0)
  {
    consumiu = removerFila(&filaPrioridade, &senha);
    if (consumiu)
    {
      printf("Chamando senha prioridade n° P%d\n", senha);
      turno = 1;
    }
    else
    {
      consumiu = removerFila(&filaNormal, &senha);
      if (consumiu)
        printf("Chamando senha normal n° N%d\n", senha);
      else
        printf("Nenhuma senha para chamar.\n");
    }
  }
  else
  {
    consumiu = removerFila(&filaNormal, &senha);
    if (consumiu)
    {
      printf("Chamando senha normal n° N%d\n", senha);
      turno = 0;
    }
    else
    {
      consumiu = removerFila(&filaPrioridade, &senha);
      if (consumiu)
        printf("Chamando senha prioridade n° P%d\n", senha);
      else
        printf("Nenhuma senha para chamar.\n");
    }
  }
}

void visualizarFilas(Fila normal, Fila prioridade)
{
  int i, pos;

  printf("\nSenhas prioridades: \n");
  if (prioridade.total == 0)
  {
    printf("Nenhuma senha prioridade gerada.\n");
  }
  else
  {
    pos = prioridade.inicio;
    for (i = 0; i < prioridade.total; i++)
    {
      printf("P%d | ", prioridade.senhas[pos]);
      pos = (pos + 1) % MAX;
    }
    printf("\n");
  }

  printf("\nSenhas normais: \n");
  if (normal.total == 0)
  {
    printf("Nenhuma senha normal gerada.\n");
  }
  else
  {
    pos = normal.inicio;
    for (i = 0; i < normal.total; i++)
    {
      printf("N%d | ", normal.senhas[pos]);
      pos = (pos + 1) % MAX;
    }
    printf("\n");
  }
}