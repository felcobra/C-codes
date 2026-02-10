# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = main

# Arquivos fonte (.c)
SOURCES = main.c bmais.c hash.c heap.c

# Arquivos objeto (.o) - gerados automaticamente a partir dos .c
OBJECTS = $(SOURCES:.c=.o)

# Regra principal: criar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Compilação concluída! Execute com: ./$(TARGET)"

# Regra para criar arquivos .o a partir dos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos limpos!"

# Regra para recompilar tudo do zero
rebuild: clean $(TARGET)

# Regra para mostrar ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto"
	@echo "  make clean  - Remove arquivos gerados"
	@echo "  make rebuild - Limpa e recompila tudo"
	@echo "  make help   - Mostra esta ajuda"

# Indica que estas regras não criam arquivos
.PHONY: clean rebuild help