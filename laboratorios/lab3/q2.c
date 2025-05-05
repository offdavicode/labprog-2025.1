#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int gerar()
{
  static int numeros[100];
  static int embaralhado = 0;
  static int indice = 0;

  if (!embaralhado)
  {
    for (int i = 0; i < 100; i++)
    {
      numeros[i] = i;
    }

    srand(time(NULL));
    for (int i = 99; i > 0; i--)
    {
      int j = rand() % (i + 1);
      int temp = numeros[i];
      numeros[i] = numeros[j];
      numeros[j] = temp;
    }

    embaralhado = 1;
  }

  if (indice < 100)
  {
    return numeros[indice++];
  }
  else
  {
    return -1;
  }
}

int main()
{
  for (int i = 0; i < 100; i++)
  {
    printf("%d ", gerar());
  }
  return 0;
}