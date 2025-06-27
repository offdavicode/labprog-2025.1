#include <stdio.h>

float lerNota(char* nome_avaliacao) {
    float nota;
    do {
        printf("Digite a nota da %s: ", nome_avaliacao);
        scanf("%f", &nota);
        if (nota < 0 || nota > 10) {
            printf("Nota inválida! Digite um valor entre 0 e 10.\n");
        }
    } while (nota < 0 || nota > 10);
    return nota;
}

float lerMediaDesejada() {
    float media;
    do {
        printf("Digite a media desejada: ");
        scanf("%f", &media);
        if (media < 0 || media > 10) {
            printf("Media inválida! Digite um valor entre 0 e 10.\n");
        }
    } while (media < 0 || media > 10);
    return media;
}

int main() {
    float av1, av2, av3, media_desejada;

    av1 = lerNota("AV1");
    av2 = lerNota("AV2");
    av3 = lerNota("AV3");
    media_desejada = lerMediaDesejada();

    float n1 = (av1 + av2) / 2.0;
    float n2_necessario = (media_desejada * 5 - 2 * n1) / 3.0;
    float av4_necessario = 2 * n2_necessario - av3;

    if (av4_necessario > 10) {
        printf("\nInfelizmente, não é possível atingir a média desejada mesmo tirando 10 na AV4.\n");
    } else if (av4_necessario < 0) {
        printf("\nVocê já garantiu a média desejada, mesmo se tirar 0 na AV4!\n");
    } else {
        printf("\nVocê precisa tirar pelo menos %.2f na AV4 para atingir a média %.2f\n", av4_necessario, media_desejada);
    }

    return 0;
}
