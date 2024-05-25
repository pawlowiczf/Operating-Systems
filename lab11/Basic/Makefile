CC = gcc

# Lista plików źródłowych
SOURCES := $(wildcard *.c)

# Lista plików wykonywalnych
EXECUTABLES := $(SOURCES:.c=)

# Domyślne zadanie: kompilacja wszystkich plików wykonywalnych
all: $(EXECUTABLES)

# Reguła kompilacji plików źródłowych do plików wykonywalnych
%: %.c
	$(CC) $< -o $@

# Zadanie czyszczenia (usuwanie wszystkich plików wykonywalnych)
clean:
	rm -f $(EXECUTABLES)
