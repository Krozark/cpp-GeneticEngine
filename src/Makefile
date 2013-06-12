all: main.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ "$(OBJ_DIR)"

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ "$(OBJ_DIR)"


main.exe: GeneticEngine.hpp GeneticThread.hpp GeneticThread.o benchmarks-func.o gnuplot/src/gnuplot_i.o
#	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f *.exe
	@rm -f gnuplot/src/gnuplot_i.o
