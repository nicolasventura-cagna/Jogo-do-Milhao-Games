# Makefile que permite organizar em pastas ;p

# ==================================================
# CONFIGURAÇÕES GERAIS
# ==================================================

CC = gcc

CFLAGS = -Wall -Wextra -g

INCLUDES = -Iinclude

# ==================================================
# CONFIGURAÇÃO DA RAYLIB
# ==================================================

RAYLIB_DIR = lib

LDFLAGS = -L$(RAYLIB_DIR)

ifeq ($(OS),Windows_NT)

    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

else

    LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

endif

# ==================================================
# ARQUIVOS DO PROJETO
# ==================================================

SRCS = main.c \
       src/cadastro.c \
       src/arquivo.c \
       src/jogo.c \
       src/limpabuffer.c \
       src/interface.c \
       src/minigames.c

# Headers da pasta include
HEADERS = $(wildcard include/*.h)

# Converte .c em .o
OBJS = $(patsubst %.c,build/%.o,$(SRCS))

# ==================================================
# CONFIGURAÇÕES DO SISTEMA OPERACIONAL
# ==================================================

ifeq ($(OS),Windows_NT)

    TARGET = jogo.exe

    RUN_CMD = ./$(TARGET)

    MKDIR = mkdir -p $(dir $@)

    CLEAN_CMD = rm -rf build $(TARGET)

else

    TARGET = jogo

    RUN_CMD = ./$(TARGET)

    MKDIR = mkdir -p $(dir $@)

    CLEAN_CMD = rm -rf build $(TARGET)

endif

# ==================================================
# REGRAS DO MAKE
# ==================================================

.PHONY: all clean run

# Compilação principal
all: $(TARGET)

# Geração do executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compilação dos arquivos objeto
build/%.o: %.c $(HEADERS)
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Execução
run: all
	$(RUN_CMD)

# Limpeza
clean:
	$(CLEAN_CMD)

# ==================================================
# FIM DO MAKEFILE
# ==================================================