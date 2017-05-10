CC=gcc
CFLAGS=-Wall -O2
BINNAME=elaborato

ifeq ($(IPC), thread)
	OBJECTS=obj/main_thread.o obj/file.o obj/io.o obj/math.o
	HEADERS=src/include/file.h src/include/io.h src/include/math.h
	EXE=obj/elaborato_thread
else 
	OBJECTS=obj/main.o obj/util.o obj/file.o obj/io.o obj/ipc.o obj/math.o obj/worker.o
	HEADERS=src/include/util.h src/include/file.h src/include/io.h src/include/ipc.h src/include/math.h src/include/worker.h
	EXE=obj/elaborato_ipc
	IPC=ipc
endif

all: $(EXE)
	@cp $(EXE) $(BINNAME)
	@echo "Compilato progetto usando $(IPC)"

obj/%.o: src/%.c $(HEADERS)
	@echo "Compilazione $<"
	@$(CC) -c -o $@ $< $(CFLAGS) 

$(EXE): $(OBJECTS)	
	@echo "Linking $(BINNAME)"
	@$(CC) -o $@ $^

clean: 
	@echo "Pulizia sorgenti"
	@rm obj/*

help:
	@echo "usage: make -- compila progetto standard"
	@echo "       make IPC=thread -- compila usando thread"
