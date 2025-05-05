#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void embaralhar(int vetor[], int n)
{
  srand(time(NULL));
  for (int i = n - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    int temp = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = temp;
  }
}

void imprimirVetor(int vetor[], int n)
{
  printf("[");
  for (int i = 0; i < n; i++)
  {
    printf("%d", vetor[i]);
    if (i < n - 1)
    {
      printf(", ");
    }
  }
  printf("]\n");
}

int main()
{
  int vetor[] = {1, 2, 4, 5, 7, 8, 9};
  int n = sizeof(vetor) / sizeof(vetor[0]);

  printf("Antes de embaralhar: ");
  imprimirVetor(vetor, n);

  embaralhar(vetor, n);

  printf("Depois de embaralhar: ");
  imprimirVetor(vetor, n);

  return 0;
}