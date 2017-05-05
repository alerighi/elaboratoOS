CC=gcc
CFLAGS=-Wall -O2
BINNAME=elaborato
OBJECTS=obj/main.o obj/util.o obj/file.o obj/io.o
HEADERS=src/include/util.h src/include/file.h src/include/io.h

obj/%.o: src/%.c $(HEADERS)
	@echo "Compiling $<"
	@$(CC) -c -o $@ $< $(CFLAGS) 

$(BINNAME): $(OBJECTS)
	@echo "Linking $(BINNAME)"
	@$(CC) -o $@ $^

clean: 
	@echo "Pulizia sorgenti"
	@rm obj/*
	@rm $(BINNAME)



