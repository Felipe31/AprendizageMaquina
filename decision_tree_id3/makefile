all: test_decision_tree_id3

test_database: test_database.c database.c database.h
	gcc -g -Wall -Wextra -o test_database test_database.c database.c database.h

database.o: database.c database.h
	gcc -c -g -Wall -Wextra database.c database.h

functions_decision_tree_id3: database.o test_functions_decision_tree_id3.c functions_decision_tree_id3.c functions_decision_tree_id3.h
	gcc -g -lm -Wall -Wextra -o test_functions_decision_tree_id3 test_functions_decision_tree_id3.c functions_decision_tree_id3.c functions_decision_tree_id3.h database.o

functions_decision_tree_id3.o: functions_decision_tree_id3.c functions_decision_tree_id3.h
	gcc -c -g -lm -Wall -Wextra functions_decision_tree_id3.c functions_decision_tree_id3.h

list_insertion_sort.o: database.o functions_decision_tree_id3.o decision_tree_id3.h list_insertion_sort.c list_insertion_sort.h
	gcc -c -g -lm -Wall -Wextra list_insertion_sort.c list_insertion_sort.h decision_tree_id3.h

test_decision_tree_id3: functions_decision_tree_id3.o database.o list_insertion_sort.o decision_tree_id3.c decision_tree_id3.h test_decision_tree_id3.c
	gcc -g -lm -Wall -Wextra -o test_decision_tree_id3 test_decision_tree_id3.c functions_decision_tree_id3.o database.o list_insertion_sort.o decision_tree_id3.c decision_tree_id3.h

test_list_insertion_sort: database.o functions_decision_tree_id3.o decision_tree_id3.h test_list_insertion_sort.c list_insertion_sort.c list_insertion_sort.h
	gcc -g -lm -Wall -Wextra -o test_list_insertion_sort test_list_insertion_sort.c list_insertion_sort.c list_insertion_sort.h database.o functions_decision_tree_id3.o decision_tree_id3.h

clean:
	rm database.o database.h.gch functions_decision_tree_id3.o functions_decision_tree_id3.h.gch test_decision_tree_id3 list_insertion_sort.o list_insertion_sort.h.gch decision_tree_id3.h.gch
