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

int is_sublist(int *const arrayA, int n_size, int *const arrayB, int m_size)
{
  if (m_size == 0)
    return 0;

  for (int i = 0; i <= n_size - m_size; i++)
  {
    int j = 0;
    while (j < m_size && arrayA[i + j] == arrayB[j])
    {
      j++;
    }
    if (j == m_size)
    {
      return i;
    }
  }
  return -1;
}

int main()
{
  srand(time(NULL));

  int tamanhosA[3] = {10, 15, 20};
  int tamanhosB[3] = {2, 3, 4};

  for (int i = 0; i < 3; i++)
  {
    int n = tamanhosA[i];
    int m = tamanhosB[i];

    int arrayA[n];
    int arrayB[m];

    gerarV(arrayA, n);
    gerarV(arrayB, m);

    printf("\n--- Vetor %d ---\n", i + 1);
    exibirV("A", arrayA, n);
    exibirV("B", arrayB, m);

    int resultado = is_sublist(arrayA, n, arrayB, m);
    if (resultado != -1)
    {
      printf("B é sublista de A, começando no índice %d.\n", resultado);
    }
    else
    {
      printf("B NÃO é sublista de A.\n");
    }
  }

  return 0;
}