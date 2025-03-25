#include <stdio.h>

int main()
{
  char c;
  int decimal = 0;
  int i = 0;
  int negativo = 0;
  int bit;

  printf("Insira um número binário (até 8 bits): ");

  while ((c = getchar()) != '\n')
  {
    if (i >= 8)
    {
      printf("Número binário maior que 8 bits.\n");
      return 1;
    }

    if (c == '0' || c == '1')
    {
      bit = c - '0';
      if (i == 0 && bit == 1)
      {
        negativo = 1;
      }
      decimal = decimal * 2 + bit;
      i++;
    }
    else if (c != '\n')
    {
      printf("Insira apenas 1 ou 0.\n");
      return 1;
    }
  }

  if (negativo)
  {
    decimal = ~decimal + 1;
    decimal = decimal & ((1 << i) - 1);
    decimal = -decimal;
  }

  printf("Número em decimal: %d\n", decimal);
  return 0;
}