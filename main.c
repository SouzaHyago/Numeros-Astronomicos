#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInt.h"

// Define um tamanho máximo para leitura.
// O PDF menciona 24 milhões de dígitos[cite: 12], mas testar com isso
// pode estourar a pilha ou heap dependendo do sistema.
// Um buffer de 100.000 dígitos parece razoável para a maioria dos casos.
#define MAX_BUFFER 100000

int main() {
    int n;
    scanf("%d", &n);

    char operacao[10];
    
    // Aloca os buffers de entrada dinamicamente
    char *str_a = malloc(MAX_BUFFER);
    char *str_b = malloc(MAX_BUFFER);

    if (!str_a || !str_b) {
        printf("Erro: Falha ao alocar memoria para entrada.\n");
        return 1;
    }

    BigInt *a = criar();
    BigInt *b = criar();

    if (!a || !b) {
         printf("Erro: Falha ao alocar memoria para BigInt.\n");
         free(str_a);
         free(str_b);
         return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%s %s %s", operacao, str_a, str_b);
        
        definir(a, str_a);
        definir(b, str_b);

        if (strcmp(operacao, "soma") == 0) {
            BigInt *resultado = soma(a, b);
            printf("Resultado :: ");
            imprimir(resultado);
            printf("\n");
            destruir(&resultado); // Libera o resultado da soma
        
        } else if (strcmp(operacao, "maior") == 0) {
            int res = maior(a, b);
            printf("Resultado :: %s\n", res ? "True" : "False");
        
        } else if (strcmp(operacao, "menor") == 0) {
            int res = menor(a, b);
            printf("Resultado :: %s\n", res ? "True" : "False");
        
        } else if (strcmp(operacao, "igual") == 0) {
            int res = igual(a, b);
            printf("Resultado :: %s\n", res ? "True" : "False");
        }
    }

    // Libera a memória alocada
    destruir(&a);
    destruir(&b);
    free(str_a);
    free(str_b);

    return 0;
}