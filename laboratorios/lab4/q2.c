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
    exibirV("B antes", arrayB, m);

    compute_frequency(arrayA, n, arrayB, m);
    exibirV("B depois", arrayB, m);
  }

  return 0;
}