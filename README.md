# 🚀 Números Astronômicos (BigInt)

> Projeto desenvolvido para a disciplina de Estruturas de Dados I (ICMC-USP), com o objetivo de implementar uma biblioteca em C para manipulação de números inteiros de tamanho arbitrário.

## 📖 Descrição

Tipos primitivos em C, como `int` ou `long`, possuem um limite fixo de bytes e, portanto, não conseguem representar números de magnitude astronômica, como o maior primo já calculado (com mais de 24 milhões de dígitos) ou a idade do universo em segundos.

Este projeto resolve esse problema implementando uma estrutura de **BigInt** (Inteiro Grande) que utiliza uma lista encadeada para armazenar os dígitos, permitindo que um número tenha um tamanho arbitrário, limitado apenas pela memória disponível.

## 🔧 Implementação

A estrutura de dados principal é uma lista encadeada. Para otimizar o uso de memória e reduzir a complexidade, em vez de cada nó da lista armazenar um único dígito, ele armazena um **bloco de dígitos**.

Nesta implementação (`BigInt.c`), cada nó da lista guarda um bloco de até `MAX = 16` dígitos (representados como caracteres).

## ✨ Funcionalidades

A biblioteca é capaz de realizar as seguintes operações aritméticas e lógicas:

* **`soma(a, b)`**: Realiza a soma de dois números (ex: `a + b`).
* **`maior(a, b)`**: Compara se o primeiro número é maior que o segundo (ex: `a > b`).
* **`menor(a, b)`**: Compara se o primeiro número é menor que o segundo (ex: `a < b`).
* **`igual(a, b)`**: Compara se os dois números são iguais (ex: `a == b`).

O programa também trata corretamente números negativos, números de diferentes tamanhos e zeros à esquerda.

## 🛠️ Pré-requisitos

Para compilar e executar o projeto, você precisará ter `gcc` e `make` instalados no seu sistema.

* `gcc` (GNU Compiler Collection)
* `make` (GNU Make utility)

## ⚙️ Como Usar

O `Makefile` fornecido foi configurado para compilar e executar o projeto de forma simples.

### 1. Compilando o Projeto

Para compilar o código-fonte e gerar o executável `bigint_app`, execute:

```bash
make all
```
### 2. Executando o Programa

Para executar o programa após a compilação, use:
```bash
make run
```
### 3. Formato de Entrada

O programa utiliza a entrada padrão (stdin) e segue um formato específico:

 1. A primeira linha deve conter um único inteiro n, que representa a quantidade de operações que serão executadas.
 2. As próximas n linhas devem conter o comando (ex: soma), o primeiro número (a) e o segundo número (b).
    
#### Exemplo de Entrada:
```bash
12
soma 9 3
soma 225 225
soma 11123456789 11987654321
soma 101498473623545 10234586723
soma 1123456 1123459
maior -10 1
menor 012143 110
maior 1123456 112345664
igual 123456789745 123456789745
soma 050 050
soma 2500 113567
igual 09870 098700
```
### 4. Formato de Saída

#### Exemplo de Saída: (Correspondente à entrada acima)
Para cada operação lida, o programa imprimirá o resultado no formato Resultado: <valor>. Para operações lógicas (maior, menor, igual), o resultado será True ou False.
```bash
Resultado: 12
Resultado: 450
Resultado: 23111111110
Resultado: 101508708210268
Resultado: 2246915
Resultado: False
Resultado: False
Resultado: False
Resultado: True
Resultado: 100
Resultado: 116067
Resultado: False
```
