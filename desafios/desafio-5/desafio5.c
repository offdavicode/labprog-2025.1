#include <stdio.h>

float lerNota(const char* nome) {
    float nota;
    do {
        printf("Digite a nota da %s: ", nome);
        scanf("%f", &nota);

        if (nota < 0 || nota > 10) {
            printf("Nota inválida! Tente novamente.\n");
        }
    } while (nota < 0 || nota > 10);
    return nota;
}

int main() {
    float av1, av2, av3, av4, av5;
    float mediaFinal, mediaParcial, notaNecessaria;
    int opcao;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Calcular média final (MF)\n");
        printf("2 - Calcular nota necessária na AV5 para passar na média\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                av1 = lerNota("AV1");
                av2 = lerNota("AV2");
                av3 = lerNota("AV3");
                av4 = lerNota("AV4");
                av5 = lerNota("AV5");

                mediaFinal = (av1*1 + av2*3 + av3*2 + av4*1 + av5*3) / 10.0;
                printf("\nMédia final: %.2f\n", mediaFinal);
                break;

            case 2:
                av1 = lerNota("AV1");
                av2 = lerNota("AV2");
                av3 = lerNota("AV3");
                av4 = lerNota("AV4");

                mediaParcial = av1*1 + av2*3 + av3*2 + av4*1;
                notaNecessaria = (7.0 * 10 - mediaParcial) / 3.0;

                if (notaNecessaria > 10) {
                    printf("\nInfelizmente, não é possível alcançar média 7.0, mesmo tirando nota máxima.\n");
                } else if (notaNecessaria <= 0) {
                    printf("\nVocê já atingiu média suficiente para passar!\n");
                } else {
                    printf("\nVocê precisa tirar %.2f na AV5 para passar com média 7.0\n", notaNecessaria);
                }
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}