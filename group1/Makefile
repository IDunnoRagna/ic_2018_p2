# Os diretorios
BIN := bin
SRC := src
INC := include
OBJ := obj

# Tudo para fazer
EXEC    := $(BIN)/main
SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(subst $(SRC), $(OBJ), $(SOURCES:.c=.o))
DEPS    := $(OBJECTS:.o=.d)

# Flags
CFLAGS  := -pedantic -Wall -Wextra -Wno-unused -I $(INC) -MMD -MP

# Ponto de entrada
all: $(EXEC)
	@echo "Tudo em ordem"

# O executavel
$(EXEC): $(OBJECTS)
	@echo "Compilar executavel: $@"
	@$(CC) $^ -o $@

# Os ficheiros de objeto
$(OBJ)/%.o: $(SRC)/%.c
	@echo "Compilar objeto $@"
	@$(CC) $(CFLAGS) -c $(filter %.c,$^) -o $@

# Incluir todos os .o.d
-include $(DEPS)
