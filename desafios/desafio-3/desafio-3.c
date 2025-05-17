#include <stdio.h>
#include <string.h>

void substituicao(const char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    if (str[i] == 'r' || str[i] == 'R')
    {
      if ((str[i + 1] == 'r') || (str[i + 1] == 'R'))
      {
        putchar(str[i] == 'R' ? 'L' : 'l');
        i += 2;
      }
      else
      {
        putchar(str[i] == 'R' ? 'L' : 'l');
        i++;
      }
    }
    else
    {
      putchar(str[i]);
      i++;
    }
  }
  putchar('\n');
}

int main()
{
  char texto[1000];

  printf("Digite um texto para fazer a substituição de R por L: ");
  fgets(texto, sizeof(texto), stdin);

  size_t len = strlen(texto);
  if (len > 0 && texto[len - 1] == '\n')
  {
    texto[len - 1] = '\0';
  }
  printf("Texto após a substituição: ");
  substituicao(texto);

  return 0;
}