#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerarV(int *vetor, int tamanho)
{
  for (int i = 0; i < tamanho; i++)
  {
    vetor[i] = rand() % 10;
  }
}

int compute_frequency(int *const arrayA, int n_size, int *const arrayB, int m_size)
{
  for (int i = 0; i < m_size; i++)
  {
    int count = 0;
    for (int j = 0; j < n_size; j++)
    {
      if (arrayB[i] == arrayA[j])
      {
        count++;
      }
    }
    arrayB[i] = count;
  }
  return 0;
}

int existeV(int *vetor, int tamanho, int valor)
{
  for (int i = 0; i < tamanho; i++)
  {
    if (vetor[i] == valor)
      return 1;
  }
  return 0;
}

int copiar_sem_repeticoes(int *origem, int tamanho_origem, int *destino)
{
  int tamanho_destino = 0;
  for (int i = 0; i < tamanho_origem; i++)
  {
    if (!existeV(destino, tamanho_destino, origem[i]))
    {
      destino[tamanho_destino++] = origem[i];
    }
  }
  return tamanho_destino;
}

void exibirV(const char *nome, int *vetor, int tamanho)
{
  printf("%s = [", nome);
  for (int i = 0; i < tamanho; i++)
  {
    printf("%d", vetor[i]);
    if (i < tamanho - 1)
      printf(", ");
  }
  printf("]\n");
}

void imprimirAst(int *A, int n, int *B, int m)
{
  int C[m];
  for (int i = 0; i < m; i++)
  {
    C[i] = B[i];
  }

  compute_frequency(A, n, C, m);

  for (int i = 0; i < m; i++)
  {
    printf("%d: ", B[i]);
    for (int j = 0; j < C[i]; j++)
    {
      printf("*");
    }
    printf("\n");
  }
}

int main()
{
  srand(time(NULL));

  int n = 15;
  int A[n];
  gerarV(A, n);

  int B[n];
  int m = copiar_sem_repeticoes(A, n, B);

  exibirV("Vetor A", A, n);

  printf("\n");
  imprimirAst(A, n, B, m);

  return 0;
}