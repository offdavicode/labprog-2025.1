#include <stdio.h>

double pot(double base, int exp)
{
  double resultado = 1.0;
  int positivo = exp >= 0 ? exp : -exp;

  for (int i = 0; i < positivo; i++)
  {
    resultado *= base;
  }

  if (exp < 0)
  {
    return 1.0 / resultado;
  }

  return resultado;
}

int main()
{
  double base;
  int expoente;

  printf("Digite a base: ");
  scanf("%lf", &base);

  printf("Digite o expoente: ");
  scanf("%d", &expoente);

  double resultado = pot(base, expoente);
  printf("%.2lf elevado a %d é igual a %.2lf\n", base, expoente, resultado);

  return 0;
}