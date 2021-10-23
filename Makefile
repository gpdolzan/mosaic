# GRR20209948 Gabriel Pimentel Dolzan
# Makefile padrao

#Variaveis
CFLAGS = -std=gnu99 -Wall
LDLIBS = -lm

#arquivos-objeto
objects = main.o paramsIO.o ppmManager.o arrayManager.o

#regra default
all: mosaico

mosaico: $(objects)
	gcc $(CFLAGS) $(objects) -o mosaico

main.o: main.c
	cc -c main.c $(CFLAGS)

paramsIO.o: paramsIO.c paramsIO.h
	cc -c paramsIO.c $(CFLAGS)

ppmManager.o: ppmManager.c ppmManager.h
	cc -c ppmManager.c $(CFLAGS) $(LDLIBS)

arrayManager.o: arrayManager.c arrayManager.h
	cc -c arrayManager.c $(CFLAGS)

#remove arquivos temporarios
clean:
	rm -f *~ *.o

#remove arquivos temporarios
purge: clean
	rm -f mosaico