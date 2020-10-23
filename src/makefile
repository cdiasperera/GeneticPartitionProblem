fileName = partitionProblem
$(fileName): $(fileName).o simconfig.o output.o main.c simconfig.h makefile
	gcc main.c $(fileName).o simconfig.o output.o -o $(fileName).exe

$(fileName).o: $(fileName).c $(fileName).h makefile simconfig.h
	gcc -std=c99 -g -Wall -pedantic -c $(fileName).c -o $(fileName).o -I.

simconfig.o: simconfig.c simconfig.h makefile
	gcc -std=c99 -g -Wall -pedantic -c simconfig.c -o simconfig.o -I.

output.o: output.c output.h makefile
	gcc -std=c99 -g -Wall -pedantic -c output.c -o output.o -I.

test: $(fileName)
	./$(fileName).exe < test.in

clean:
	rm -f *.o $(fileName).exe
