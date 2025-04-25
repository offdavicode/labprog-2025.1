#include <stdio.h>

double raiz_quadrada(double numero)
{
  if (numero < 0)
  {
    printf("Número negativo não tem raiz quadrada real, tente com um número positivo.\n");
    return -1;
  }

  double chute = numero;
  double precisao = 0.001;

  if (numero == 0 || numero == 1)
    return numero;

  while ((chute * chute - numero) > precisao || (numero - chute * chute) > precisao)
  {
    chute = (chute + numero / chute) / 2;
  }

  return chute;
}

int main()
{
  double numero;

  printf("Digite um número para calcular a raiz quadrada: ");
  scanf("%lf", &numero);

  double resultado = raiz_quadrada(numero);

  if (resultado != -1)
  {
    printf("A raiz quadrada aproximada de %.4lf é %.4lf\n", numero, resultado);
  }

  return 0;
}