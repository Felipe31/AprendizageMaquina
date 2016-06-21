all: perceptron

perceptron: database.o perceptron.o test_perceptron.c
	gcc -lm -g -Wall -Wextra -o test_perceptron database.o perceptron.o test_perceptron.c

perceptron.o: perceptron.c perceptron.h
	gcc -c -lm -g -Wall -Wextra perceptron.c perceptron.h

database.o: database.c database.h
	gcc -c -g -Wall -Wextra database.c database.h

clean:
	rm perceptron.o perceptron.h.gch test_perceptron database.h.gch database.o
