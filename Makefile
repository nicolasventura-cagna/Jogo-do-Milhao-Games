# utilize "#" para fazer comentarios (PYTHONNNNNNNNNNNNN)
# Windows: mingw32-make (COMPILAR), mingw32-make run (RODAR)
# Linux/MacOS: make (COMPILAR), make run (RODAR)
# Para LIMPAR: make clean ou mingw32-make clean

# IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE
#
# O QUE VOCÊ DIGITOU NÃO VAI SER COMPILADO SE VOCÊ NÃO
# SALVAR AS ALTERAÇÕES NO VSCODE
#
# E SEMPRE CHEQUE SE O TERMINAL ESTÁ NA MESMA PASTA
# QUE OS ARQUIVOS DO PROGRAMA.
#
# SE APARECER "Nothing to be done for 'all'",
# USE CLEAN PARA COMPILAR NOVAMENTE.
#
# Obrigado pela sua atenção :P


# define o compilador (gcc ou clang)
CC = gcc

# -Wall e -Wextra avisam sobre possíveis bugs no código
# -g inclui informações para debug
CFLAGS = -Wall -Wextra -g


# Lista dos arquivos de código fonte
SRCS = main.c cadastro.c arquivo.c jogo.c limpabuffer.c

# Lista dos cabeçalhos
HEADERS = pergunta.h cadastro.h arquivo.h jogo.h limpabuffer.h

# Substituição automática de texto (.c -> .o)
OBJS = $(SRCS:.c=.o)


# Identifica o sistema operacional
ifeq ($(OS),Windows_NT)

    # Windows
    TARGET = jogo.exe
    RUN_CMD = .\$(TARGET)
    CLEAN_CMD = cmd /C "del /Q $(OBJS) $(TARGET) 2>nul"

else

    # Linux / macOS
    TARGET = jogo
    RUN_CMD = ./$(TARGET)
    CLEAN_CMD = rm -f $(OBJS) $(TARGET)

endif


# Informa que estas regras não representam arquivos
.PHONY: all clean run


# Regra padrão: compilar
all: $(TARGET)


# Junta todos os arquivos objeto e cria o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


# Compila os arquivos .c em arquivos .o
# Também recompila quando algum cabeçalho é alterado
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


# Garante a compilação antes de executar
run: all
	$(RUN_CMD)


# Remove os arquivos objeto e o executável
clean:
	$(CLEAN_CMD)