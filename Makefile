# Definim numele programului și al fișierelor sursă
TARGET = SATsolver
SRC = SATsolver.c

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Regula pentru build
build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regula pentru run
run: $(TARGET)
	./$(TARGET) $(INPUT) > $(OUTPUT)

# Regula pentru clean
clean:
	rm -f $(TARGET)
