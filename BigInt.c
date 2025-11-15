#include "BigInt.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX 16 
// Usamos MAX = 16 para reduzir a quantidade de nós na lista.
// Porque blocos pequenos precisam ser criados em grande quantidade para comportar grandes números,
// na arquitetura do meu pc(64-bit), um Node com MAX=16 ocupa uns 24 bytes (16 do bloco(char numbers[MAX]) + 8 do ponteiro(Node *next)).
// só pra merito de comparação estas seriam a quantidade de nós para um numero com bastante dígitos
// um número com 256 dígitos precisaria de:
// MAX = 4 --> 64 nós
// MAX = 8 --> 32 nós
// MAX = 16 --> 16 nós
// então decidi por 16 que já da uma boa diminuida na quantidade de nós e melhora a eficiencia de memória sem deixar a implementação tão complexa



typedef struct node {
  char numbers[MAX]; // Bloco de numeros (12312312312312312)
  struct node *next; //ponteiro para o próximo bloco de números
} Node;

//Estrutura principal da lista
struct BigInt{
  int size; //numero de digitos válidos(sem considerar os 0 à esquerda) utilizados
  char sinal; //-1 para negativo e 1 para positivo
  Node *head; //Aponta para o primeiro bloco de números
};


BigInt *criar(){
  BigInt *big = malloc(sizeof(BigInt));
  if (!big) {
		return NULL; //erro na alocação
	}

	// se não der erro, inicializa campos
	big->head = NULL; // nenhum numero criado
	big->size = 0;
  big->sinal = 1; //padrão antes de um número ser adicionado

	return big;
}

BigInt *definir(BigInt *numero, const char *decimal) {
    if (!numero || !decimal) return NULL;

    // Limpa a lista antiga, se houver
    if (numero->head) {
        Node *atual = numero->head;
        while (atual) {
            Node *proximo = atual->next;
            free(atual);
            atual = proximo;
        }
        numero->head = NULL;
    }


    int len = strlen(decimal);
    int inicio = 0;

    
    if (decimal[0] == '-') {
        numero->sinal = -1;
        inicio++;
    } else if (decimal[0] == '+') {
        numero->sinal = 1;
        inicio++;
    } else {
        numero->sinal = 1;
    }

    //passa pro próximo índice até encontrar o primeiro número diferente de 0
    while (decimal[inicio] == '0' && inicio < len - 1){
      inicio++;
    }

    /*decimal tem o endereço do primeiro digito, então a gente direto pro mais significante
    ex: o endereço de *num é 120, e o usuário escreveu 000352321, inicio vai ser = 3(pq tem 3 zeros)
    então *num se torna 3 + 120 ficando 123 que seria o endereço do primeiro digito significativo*/
    
    const char *num = decimal + inicio;
    len = strlen(num);

    // Se número é "0", tratar como caso especial
    if (strcmp(num, "0") == 0) {
        Node *n = malloc(sizeof(Node));
        if (!n) return NULL;
        memset(n->numbers, '0', MAX);//ocupa a lista com zeros
        n->next = NULL;
        numero->head = n;
        numero->size = 1;
        numero->sinal = 1; // Normaliza "0" para positivo
        return numero;
    }

    // Criação dos blocos de trás pra frente
    int i = len;
    Node *head = NULL;

    while (i > 0) {

        int bloco_inicio = i - MAX;
        if (bloco_inicio < 0) bloco_inicio = 0;

        int bloco_tamanho = i - bloco_inicio;

        Node *novo = malloc(sizeof(Node));
        if (!novo) return NULL; // falha na alocação
        memset(novo->numbers, '0', MAX);

        // Copiar os dígitos para o final do bloco
        memcpy(novo->numbers + (MAX - bloco_tamanho),num + bloco_inicio,bloco_tamanho);

        novo->next = head;
        head = novo;

        i -= bloco_tamanho;
    }

    numero->head = head;
    numero->size = len;

    return numero;
}

void destruir(BigInt **numero) {
    if (!numero || !*numero) return;

    Node *atual = (*numero)->head;
    //deleta todos os nós pra depois liberar o BigInt
    while (atual) {
        Node *proximo = atual->next;
        free(atual); // Libera o nó
        atual = proximo;
    }
    
    free(*numero); // Libera a estrutura principal
    *numero = NULL; // Define o ponteiro original como nulo
}



// Função pra ajudar a comparar valores absolutos
// Retorna: 1 se |a| > |b|, -1 se |a| < |b|, 0 se |a| == |b|
static int comparacao_absoluta(const BigInt *a, const BigInt *b) {
    if (a->size > b->size) return 1;
    if (a->size < b->size) return -1;

    // Se os tamanhos são iguais, compara bloco a bloco
    Node *pa = a->head;
    Node *pb = b->head;
    while (pa) { // Não precisa checar pb, pq os tamanhos são iguais
        // Compara os blocos como strings
        int cmp = strncmp(pa->numbers, pb->numbers, MAX);
        if (cmp > 0) return 1;
        if (cmp < 0) return -1;
        
        pa = pa->next;
        pb = pb->next;
    }
    return 0; // São absolutamente iguais
}

int igual(const BigInt *a, const BigInt *b) {
    if (!a || !b) return 0;

    // Se os sinais são diferentes, a e b são imediatamente diferentes.
    if (a->sinal != b->sinal) return 0;

    // Se os sinais são iguais, basta comparar o valor absoluto
    return comparacao_absoluta(a, b) == 0;
}

//compara se a é maior que b
int maior(const BigInt *a, const BigInt *b) {
    if (!a || !b) return 0;

    // Sinais diferentes já dão a resposta automaticamente
    if (a->sinal > b->sinal) return 1; // a é positivo, b é negativo
    if (a->sinal < b->sinal) return 0; // a é negativo, b é positivo

    // Sinais iguais, compara o valor absoluto
    int cmp_abs = comparacao_absoluta(a, b);

    if (a->sinal == 1) { // Ambos positivos
        return cmp_abs > 0; // retorn 1 se |a| > |b|
    } else { // Ambos negativos
        return cmp_abs < 0; // retorna -1 se |a| < |b|
    }
}


int menor(const BigInt *a, const BigInt *b) {
    if (!a || !b) return 0;
    /*como já temos funões para validar se é igual ou maior
      se 'a' -->NÃO<-- for maior nem igual, ele é menor */
    return (!maior(a, b) && !igual(a, b));
}


// Função pra auxiliar a reverter uma string
static char* str_reverter(char *str) {
    int i = 0;
    int j = strlen(str) - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
    return str;
}

// Função pra auxiliar a somar duas strings de números positivos
static char* soma_strings(const char* s1, const char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int maxLen = (len1 > len2 ? len1 : len2);
    char* resultado = malloc(maxLen + 2); // +1 pq o número pode aumentar em uma casa e +1 para '\0'
    memset(resultado, 0, maxLen + 2);

    int carry = 0, i = len1 - 1, j = len2 - 1, k = 0;

    while (i >= 0 || j >= 0 || carry) {
        int dig1 = (i >= 0) ? (s1[i--] - '0') : 0;
        int dig2 = (j >= 0) ? (s2[j--] - '0') : 0;
        
        int soma = dig1 + dig2 + carry;
        resultado[k++] = (soma % 10) + '0';
        carry = soma / 10;
    }
    
    resultado[k] = '\0';
    return str_reverter(resultado);
}

// Função auxiliar para subtrair s2 de s1 onde o s1 vai sempre ser >= s2
static char* subtracao_strings(const char* s1, const char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    char* resultado = malloc(len1 + 1);
    memset(resultado, 0, len1 + 1);

    int emprestar = 0, i = len1 - 1, j = len2 - 1, k = 0;

    while (i >= 0) {
        int dig1 = s1[i--] - '0';
        int dig2 = (j >= 0) ? (s2[j--] - '0') : 0;

        int sub = dig1 - dig2 - emprestar;
        if (sub < 0) {
            sub = sub + 10;
            emprestar = 1;
        } else {
            emprestar = 0;
        }
        resultado[k++] = sub + '0';
    }

    resultado[k] = '\0';
    str_reverter(resultado);

    // Remove zeros à esquerda do resultado da subtração (ex: 100 - 99 = "001")
    int inicio = 0;
    while (resultado[inicio] == '0' && inicio < k - 1) {
        inicio++;
    }

    // Se 'inicio' for > 0, move a string para a esquerda
    if (inicio > 0) {
        int l = 0;
        while (resultado[inicio + l]) {
            resultado[l] = resultado[inicio + l];
            l++;
        }
        resultado[l] = '\0';
    }
    
    return resultado;
}

// Função auxiliar para converter BigInt para string
static char* bigint_para_string(const BigInt *n) {
    if (!n || !n->head) return NULL;
    
    // Caso especial "0"
    if (n->size == 1 && n->head->numbers[MAX-1] == '0') {
        char *str_zero = malloc(2);
        strcpy(str_zero, "0");
        return str_zero;
    }

    char *str = malloc(n->size + 1); // +1 para '\0'
    if (!str) return NULL;
    char *ptr = str;

    // A 'size' é o número de dígitos válidos.
    // O primeiro bloco pode não estar cheio.
    int tam_primeiro_bloco = n->size % MAX;
    if (tam_primeiro_bloco == 0 && n->size > 0) {
        tam_primeiro_bloco = MAX;
    }
    int inicio_primeiro_bloco = MAX - tam_primeiro_bloco;

    Node *no = n->head;
    
    // Copia o primeiro bloco (sem o os 0 à esquerda)
    memcpy(ptr, no->numbers + inicio_primeiro_bloco, tam_primeiro_bloco);
    ptr += tam_primeiro_bloco;
    no = no->next;

    // Copia o restante dos blocos (inteiros)
    while (no) {
        memcpy(ptr, no->numbers, MAX);
        ptr += MAX;
        no = no->next;
    }
    *ptr = '\0'; // Finaliza a string
    return str;
}


BigInt *soma(const BigInt *a, const BigInt *b) {
    if (!a || !b) return NULL;

    BigInt *res = criar();
    if (!res) return NULL;

    // Caso 1: a + b (ambos positivos) OU (-a) + (-b) = -(a + b)
    if (a->sinal == b->sinal) {
        char *s_a = bigint_para_string(a);
        char *s_b = bigint_para_string(b);
        char *s_res_abs = soma_strings(s_a, s_b);

        if (a->sinal == 1) {
            definir(res, s_res_abs);
        } else {
            // Adiciona o sinal de '-'
            char *s_res_final = malloc(strlen(s_res_abs) + 2);
            s_res_final[0] = '-';
            strcpy(s_res_final + 1, s_res_abs);
            definir(res, s_res_final);
            free(s_res_final);
        }
        free(s_a); free(s_b); free(s_res_abs);
        return res;
    }

    // Caso 2: Sinais opostos (subtração)
    int cmp_abs = comparacao_absoluta(a, b);
    
    // Se |a| == |b| e sinais opostos, o resultado é 0
    if (cmp_abs == 0) {
        definir(res, "0");
        return res;
    }

    char *s_a = bigint_para_string(a);
    char *s_b = bigint_para_string(b);
    char *s_res_abs;

    // Subtrai o menor do maior
    if (cmp_abs > 0) { // |a| > |b|
        s_res_abs = subtracao_strings(s_a, s_b);
        // O sinal do resultado é o sinal do maior (a)
        if (a->sinal == 1) {
            definir(res, s_res_abs);
        } else {
            char *s_res_final = malloc(strlen(s_res_abs) + 2);
            s_res_final[0] = '-';
            strcpy(s_res_final + 1, s_res_abs);
            definir(res, s_res_final);
            free(s_res_final);
        }

    } else { // |b| > |a|
        s_res_abs = subtracao_strings(s_b, s_a);
        // O sinal do resultado é o sinal do maior (b)
        if (b->sinal == 1) {
            definir(res, s_res_abs);
        } else {
            char *s_res_final = malloc(strlen(s_res_abs) + 2);
            s_res_final[0] = '-';
            strcpy(s_res_final + 1, s_res_abs);
            definir(res, s_res_final);
            free(s_res_final);
        }
    }
    
    free(s_a); free(s_b); free(s_res_abs);
    return res;
}



void imprimir(const BigInt *numero) {
    if (!numero) {
        printf("NULL");
        return;
    }

    // Trata caso especial "0"
    if (numero->size == 1 && numero->head && numero->head->numbers[MAX-1] == '0') {
        printf("0");
        return;
    }

    // Imprime o sinal se for negativo
    if (numero->sinal == -1) {
        printf("-");
    }
    
    if (!numero->head) return; // Vazio

    // Usa a mesma lógica de bigint_para_string para imprimir
    int tam_primeiro_bloco = numero->size % MAX;
    if (tam_primeiro_bloco == 0 && numero->size > 0) {
        tam_primeiro_bloco = MAX;
    }
    int inicio_primeiro_bloco = MAX - tam_primeiro_bloco;

    Node *no = numero->head;
    
    // Imprime primeiro bloco
    for (int i = inicio_primeiro_bloco; i < MAX; i++) {
        printf("%c", no->numbers[i]);
    }
    no = no->next;

    // Imprime os outros blocos
    while (no) {
        // Imprime todos os 16 chars
        for(int i = 0; i < MAX; i++) {
            printf("%c", no->numbers[i]);
        }
        no = no->next;
    }
}