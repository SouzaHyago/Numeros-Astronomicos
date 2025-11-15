# --- Variáveis de Compilação ---

# Compilador C
CC = gcc

# Flags do compilador
CFLAGS = -g -Wall -Wextra -std=c99

# Nome do executável final
TARGET = bigint_app

# Lista de todos os arquivos fonte .c
SOURCES = main.c BigInt.c


# --- Regras para Runcodes ---

# Regra 'all': Contém apenas o comando de compilação.
# Esta regra compila e liga todos os arquivos fonte diretamente.
all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Regra 'run': Contém apenas o comando de execução.
run:
	./$(TARGET)


# --- Regra de Limpeza (Opcional, mas boa prática) ---
# O corretor automático não deve usar esta regra.
clean:
	rm -f $(TARGET)