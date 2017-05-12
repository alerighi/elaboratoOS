# Makefile per la compilazione elaborato Sistemi Operativi
# Alessandro Righi 

CC=gcc
CFLAGS=-Wall -O2 -Wextra 
LDFLAGS=
BINNAME=elaborato
SRCDIR=src
OBJDIR=obj

ifeq ($(THREAD),1)
	OBJECTS=obj/main_thread.o obj/file.o obj/io.o obj/math.o
	HEADERS=src/include/file.h src/include/io.h src/include/math.h
	LDFLAGS+=-lpthread
	CFLAGS+=-DTHREAD
else 
	OBJECTS=obj/main.o obj/util.o obj/file.o obj/io.o obj/ipc.o obj/math.o obj/worker.o
	HEADERS=src/include/util.h src/include/file.h src/include/io.h src/include/ipc.h src/include/math.h src/include/worker.h
endif

.PHONY: all clean help rebuild doc test

all: $(BINNAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) $(OBJDIR)
	@echo "Compilazione $<"
	@$(CC) -c -o $@ $< $(CFLAGS) 

$(BINNAME): $(OBJECTS)	
	@echo "Linking $(BINNAME)"
	@$(CC) -o $@ $^ $(LDFLAGS)

clean: 
	@echo "Pulizia sorgenti"
	@rm -rf $(OBJDIR)
	@rm -rf $(BINNAME)

rebuild: clean $(BINNAME)

doc: 
	@doxygen doc/Doxyfile

test: clean $(BINNAME)
	@echo "----------------------"
	@echo " ESECUZIONE PROGRAMMA "
	@echo "----------------------"
ifeq ($(THREAD),1)
	./$(BINNAME) test/matA.csv test/matB.csv test/ris.csv 5
else
	./$(BINNAME) test/matA.csv test/matB.csv test/ris.csv 5 10
endif

help:
	@echo "Usage:"
	@echo "    make            compila progetto standard"
	@echo "    make THREAD=1   compila usando i thread"
	@echo "    make clean      pulisce i sorgenti"
	@echo "    make rebuild    ricompila il progetto"
	@echo "    make doc        genera la documentazione doxygen"
	@echo "    make test       esegue un test del programma"
