
openMP: openMP.c
	icc -std=c99 -c rnd.c
	icc -std=c99 -c ziggurat.c
	icc -openmp -std=c99 -c openMP.c
	icc  -openmp -std=c99 rnd.o ziggurat.o openMP.o -o openMP

.phony: clean
clean:
	rm -f openMP
