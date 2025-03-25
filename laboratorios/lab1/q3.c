#include <stdio.h>

int main()
{
  float valor;
  printf("Insira o valor em reais: R$ ");
  scanf("%f", &valor);

  int centavos = (int)(valor * 100);

  printf("Valor: R$ %.2f\n", valor);
  printf("Notas e Moedas:\n");

  if (centavos >= 20000)
  {
    int notas200 = centavos / 20000;
    printf("Notas de R$ 200: %d\n", notas200);
    centavos %= 20000;
  }

  if (centavos >= 10000)
  {
    int notas100 = centavos / 10000;
    printf("Notas de R$ 100: %d\n", notas100);
    centavos %= 10000;
  }

  if (centavos >= 5000)
  {
    int notas50 = centavos / 5000;
    printf("Notas de R$ 50: %d\n", notas50);
    centavos %= 5000;
  }

  if (centavos >= 2000)
  {
    int notas20 = centavos / 2000;
    printf("Notas de R$ 20: %d\n", notas20);
    centavos %= 2000;
  }

  if (centavos >= 1000)
  {
    int notas10 = centavos / 1000;
    printf("Notas de R$ 10: %d\n", notas10);
    centavos %= 1000;
  }

  if (centavos >= 500)
  {
    int notas5 = centavos / 500;
    printf("Notas de R$ 5: %d\n", notas5);
    centavos %= 500;
  }

  if (centavos >= 200)
  {
    int notas2 = centavos / 200;
    printf("Notas de R$ 2: %d\n", notas2);
    centavos %= 200;
  }

  if (centavos >= 100)
  {
    int moedas1 = centavos / 100;
    printf("Moedas de R$ 1: %d\n", moedas1);
    centavos %= 100;
  }

  if (centavos >= 50)
  {
    int moedas50 = centavos / 50;
    printf("Moedas de R$ 0,50: %d\n", moedas50);
    centavos %= 50;
  }

  if (centavos >= 25)
  {
    int moedas25 = centavos / 25;
    printf("Moedas de R$ 0,25: %d\n", moedas25);
    centavos %= 25;
  }

  if (centavos >= 10)
  {
    int moedas10 = centavos / 10;
    printf("Moedas de R$ 0,10: %d\n", moedas10);
    centavos %= 10;
  }

  if (centavos >= 5)
  {
    int moedas5 = centavos / 5;
    printf("Moedas de R$ 0,05: %d\n", moedas5);
    centavos %= 5;
  }

  if (centavos >= 1)
  {
    printf("Moedas de R$ 0,01: %d\n", centavos);
  }

  return 0;
}