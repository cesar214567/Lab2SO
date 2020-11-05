all: barbers_problem barbers_valgrind

program = barbers_problem.c
program_v = barbers_valgrind.c

barbers_problem:
	gcc -pthread -o barbers.out $(program)

barbers_valgrind:
	gcc -pthread -o barbers_v.out $(program_v) -g
	valgrind --tool=helgrind -v --log-file=output.txt --trace-children=yes --read-var-info=yes ./barbers_v.out

clean:
	rm *.out *.txt
