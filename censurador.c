#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define T 5000

int ehLetra(char c) {
    return isalpha((unsigned char)c);
}

void censurar(char *txt) {
    const char *palavras[] = {"batata", "janela", "mesa"};
    char buf[T];
    int i = 0, j = 0;
    while (txt[i]) {
        if (ehLetra(txt[i])) {
            char p[100]; int ini = i, k = 0;
            while (ehLetra(txt[i])) p[k++] = tolower((unsigned char)txt[i++]);
            p[k] = 0;
            int censura = 0;
            for (int w = 0; w < 3; w++) {
                char pl[100];
                strcpy(pl, palavras[w]);
                if (!strcmp(p, pl) || (strcat(pl, "s"), !strcmp(p, pl))) {
                    censura = 1;
                    break;
                }
            }
            for (int m = 0; m < k; m++)
                buf[j++] = censura ? '*' : txt[ini + m];
        } else {
            buf[j++] = txt[i++];
        }
    }
    buf[j] = '\0';
    strcpy(txt, buf);
}

int main() {
    char txt[T];
    printf("Digite o texto :\n");
    fgets(txt, T, stdin);
    while (strlen(txt) < 1024) {
        printf("Texto muito curto. Tente novamente:\n");
        fgets(txt, T, stdin);
    }
    censurar(txt);
    printf("\nTexto censurado:\n%s\n", txt);
    return 0;
}