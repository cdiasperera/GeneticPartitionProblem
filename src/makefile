fileName = partitionProblem
$(fileName): $(fileName).o main.c
	gcc main.c $(fileName).o -o $(fileName)

$(fileName).o: $(fileName).c $(fileName).h makefile
	gcc -std=c99 -g -Wall -pedantic -c $(fileName).c -o $(fileName).o -I.

test: $(fileName)
	echo "1 2 3 4" | ./$(fileName) > output.txt

clean:
	rm -f $(fileName).o $(fileName)
