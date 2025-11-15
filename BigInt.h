#ifndef BIGINT_H
#define BIGINT_H

typedef struct BigInt BigInt;

BigInt *criar();
void destruir(BigInt **numero);

BigInt *definir(BigInt *numero, const char *decimal);

BigInt *soma(const BigInt *a, const BigInt *b);

int maior(const BigInt *a, const BigInt *b);
int menor(const BigInt *a, const BigInt *b);
int igual(const BigInt *a, const BigInt *b);
void imprimir(const BigInt *numero);


#endif  