#ifndef UTIL_H
#define UTIL_H

#include <time.h>

void limparBuffer();
void trim(char *str);
void formatarData(char *entrada, char *saida);
time_t converterDataParaTimestamp(const char *dataStr);

#endif