all: test_knn

test_knn: database.o knn.o test_knn.c
	gcc -lm -g -Wall -Wextra -o test_knn database.o knn.o test_knn.c

knn.o: knn.c knn.h
	gcc -c -lm -g -Wall -Wextra knn.c knn.h

database.o: database.c database.h
	gcc -c -g -Wall -Wextra database.c database.h

clean:
	rm knn.o knn.h.gch test_knn database.h.gch database.o
