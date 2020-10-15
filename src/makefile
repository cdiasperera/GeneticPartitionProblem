fileName = partitionProblem
$(fileName): $(fileName).o main.c
	gcc main.c $(fileName).o -o $(fileName).exe

$(fileName).o: $(fileName).c $(fileName).h makefile
	gcc -std=c99 -g -Wall -pedantic -c $(fileName).c -o $(fileName).o -I.

test: $(fileName)
	./$(fileName).exe < test.in

clean:
	rm -f $(fileName).o $(fileName) $(fileName).exe
