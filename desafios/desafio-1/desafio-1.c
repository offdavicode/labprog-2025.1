#include <stdio.h>
#include <stdlib.h>

int main()
{
  int num1, num2;

  printf("Digite o primeiro número: ");
  scanf("%d", &num1);
  printf("Digite o segundo número: ");
  scanf("%d", &num2);

  int diferenca = num1 - num2;

  if (abs(diferenca) == diferenca)
  {
    printf("O primeiro número (%d) é maior que o segundo número (%d).\n", num1, num2);
  }
  else if (abs(diferenca) == -diferenca)
  {
    printf("O segundo número (%d) é maior que o primeiro número (%d).\n", num2, num1);
  }
  else
  {
    printf("Os dois números são iguais (%d).\n", num1);
  }

  return 0;
}