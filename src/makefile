fileName = partitionProblem

$(fileName): $(fileName).o simconfig.o output.o testing.o main.c makefile
	gcc main.c $(fileName).o simconfig.o output.o testing.o -o $(fileName).exe

$(fileName).o: $(fileName).c $(fileName).h makefile $(wildcard *.h)
	gcc -std=c99 -g -Wall -pedantic -c $(fileName).c -o $(fileName).o -I.

simconfig.o: simconfig.c $(wildcard *.h) makefile
	gcc -std=c99 -g -Wall -pedantic -c simconfig.c -o simconfig.o -I.

output.o: output.c $(wildcard *.h) makefile
	gcc -std=c99 -g -Wall -pedantic -c output.c -o output.o -I.

testing.o: testing.c $(wildcard *.h) makefile
	gcc -std=c99 -g -Wall -pedantic -c testing.c -o testing.o -I.

test: $(fileName)
	./$(fileName).exe < tcADS0.in
	echo 
	./$(fileName).exe < tcADSn0.in
	echo
	./$(fileName).exe < tcADSLarge.in
	echo
	./$(fileName).exe < tcSameNumber.in
	echo
	./$(fileName).exe < tcRandom.in

run: $(fileName)
	./$(fileName).exe

grabTrends: $(fileName)
	./$(fileName).exe > ../results/trends.txt

clean:
	rm -f *.o $(fileName).exe
