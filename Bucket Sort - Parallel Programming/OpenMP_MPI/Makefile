
OpenMP_MPI: OpenMP_MPI.c
	mpiicc -openmp -DMAX_SIZE=1 -std=c99 -c rnd.c
	mpiicc -openmp -DMAX_SIZE=1 -std=c99 -c ziggurat.c
	mpiicc -openmp -DMAX_SIZE=1 -std=c99 -c OpenMP_MPI.c
	mpiicc -openmp -DMAX_SIZE=1 -std=c99 rnd.o ziggurat.o OpenMP_MPI.o -o OpenMP_MPI

.phony: clean
clean:
	rm -f sequential
