#include <stdio.h>

void valorHexa(int num)
{
  printf("0x");
  for (int i = 28; i >= 0; i -= 4)
  {
    int valor = (num >> i) & 0xF;
    if (valor < 10)
    {
      putchar('0' + valor);
    }
    else
    {
      putchar('A' + (valor - 10));
    }
  }
}

int main()
{
  int num;
  printf("Insira um número inteiro: ");
  scanf("%d", &num);

  printf("\nValor em hexadecimal : ");
  valorHexa(num);

  printf("\n");
  return 0;
}