#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

time_t converterDataParaTimestamp(const char *dataStr)
{
  struct tm tm = {0};
  if (sscanf(dataStr, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) == 3)
  {
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    return mktime(&tm);
  }
  return -1;
}

void limparBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void trim(char *str)
{
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;
  end[1] = '\0';
  char *start = str;
  while (*start && isspace((unsigned char)*start))
    start++;
  memmove(str, start, strlen(start) + 1);
}

void formatarData(char *entrada, char *saida)
{
  char numeros[9];
  int j = 0;
  for (int i = 0; entrada[i] != '\0'; i++)
    if (isdigit(entrada[i]) && j < 8)
      numeros[j++] = entrada[i];
  numeros[j] = '\0';
  if (strlen(numeros) != 8)
    strcpy(saida, "00/00/0000");
  else
    snprintf(saida, 11, "%.2s/%.2s/%.4s", numeros, numeros + 2, numeros + 4);
}