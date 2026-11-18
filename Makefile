# utilize "#" para fazer comentarios (PYTHONNNNNNNNNNNNN)
# Para compilar no windows, use mingw32-make (COMPILAR), use mingw32-make run (RODAR) e use mingw32-make clean (LIMPAR)

# IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE 
# |
# V
# O QUE VOCÊ DIGITOU NÃO VAI SER COMPILADO SE VOCÊ NÃO SALVAR AS ALTERAÇÕES NO VSCODE
#
# E SEMPRE CHEQUE SE O TERMINAL ESTA NA ***MESMA*** PASTA QUE OS ARQUIVOS DO PROGRAMA (se não, use "cd nome-do-arq")
#
# NÃO SABE ONDE O TERMINAL ESTA? USE "dir" PARA VER OS ARQUIVOS DENTRO DO ARQUIVO QUE O TERMINAL ESTÁ
#
# SE APARECER A MENSAGEM "Nothing to be done for 'all'" USE **CLEAN** PARA VER O RESULTADO DA COMPILAÇÃO NOVAMENTE 
#
# Obrigado pela sua atenção :P

# define o compilador (gcc ou clang)
CC = gcc
# -Wall e Wextra avisam sobre possíveis bugs no código
# -g inclui informações para debug
CFLAGS = -Wall -Wextra -g

# Lista dos artigos código fonte (.c nesse caso)
SRCS = main.c cadastro.c arquivo.c jogo.c limpabuffer.c 
# Lista dos cabeçalhos (.h) - avisa o make caso teve alguma alteração
HEADERS = pergunta.h cadastro.h arquivo.h jogo.h limpabuffer.h 
# Substituição altomatica de texto (.c -> .o)
OBJS = $(SRCS:.c=.o)

# Checa a variavel interna do sistema:
# Windows - (OS) = Windows_NT
# Linux / MacOS - Vazio
ifeq ($(OS),Windows_NT) # V Windows (CMD / MinGW)  (óbvio)
# variaveis e diretorios do Windows
    TARGET = jogo.exe
    RUN_CMD = .\$(TARGET)
    CLEAN_CMD = del /Q $(OBJS) $(TARGET) 2>nul || exit 0
else # V Linux / MacOS
# variaveis e diretorios do Linux
    TARGET = jogo
    RUN_CMD = ./$(TARGET)
    CLEAN_CMD = rm -f $(OBJS) $(TARGET)
endif

# Mensagem pro sistema: "Esses lixos aq em baixo SÃO AÇÕES e não arquivos"
.PHONY: all clean run

# Regra padrão (Exemplo: migw32-make)
all: $(TARGET)

# verifica se os arquivos .o (object) existem e juntam todos
$(TARGET): $(OBJS)
# caso ocorra alguma alteração no arquivo .c ou .h, atualiza o .o para todos de baixo
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# garante compilação antes de rodar
run: all
	$(RUN_CMD)
# limpa a pasta de arquivos .o e o executavel final
clean:
	$(CLEAN_CMD)