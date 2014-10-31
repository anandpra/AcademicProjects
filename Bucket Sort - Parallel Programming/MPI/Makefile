
MPI: MPI.c
	mpiicc -std=c99 -c rnd.c
	mpiicc -std=c99 -c ziggurat.c
	mpiicc -std=c99 -c MPI.c
	mpiicc -std=c99 rnd.o ziggurat.o MPI.o -o MPI

.phony: clean
clean:
	rm -f MPI
