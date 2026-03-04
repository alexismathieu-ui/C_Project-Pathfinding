# Nom du compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -O2

# Nom de l'exécutable
TARGET = pathfinding

# Fichiers source
SRC = pathfinding.c library.c

# Fichiers objets (automatiquement dérivés du .c)
OBJ = $(SRC:.c=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Règle pour compiler un .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers temporaires
clean:
	rm -f $(OBJ)

# Supprimer aussi l'exécutable
fclean: clean
	rm -f $(TARGET)

# Tout nettoyer et recompiler depuis zéro
re: fclean all
