CC=gcc
CFLAGS=-Wall -O2
BINNAME=elaborato
OBJECTS=obj/main.o obj/util.o obj/file.o obj/io.o obj/ipc.o obj/math.o obj/worker.o
HEADERS=src/include/util.h src/include/file.h src/include/io.h src/include/ipc.h src/include/math.h src/include/worker.h

obj/%.o: src/%.c $(HEADERS)
	@echo "Compilazione $<"
	@$(CC) -c -o $@ $< $(CFLAGS) 

$(BINNAME): $(OBJECTS)
	@echo "Linking $(BINNAME)"
	@$(CC) -o $@ $^

clean: 
	@echo "Pulizia sorgenti"
	@rm obj/*
	@rm $(BINNAME)



