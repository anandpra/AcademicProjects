
sequential: sequential.c
	icc -std=c99 -c rnd.c
	icc -std=c99 -c ziggurat.c
	icc -std=c99 -c sequential.c
	icc -std=c99 rnd.o ziggurat.o sequential.o -o sequential

.phony: clean
clean:
	rm -f sequential
