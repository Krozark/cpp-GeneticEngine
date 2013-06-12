all: main.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ "$(OBJ_DIR)"

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ "$(OBJ_DIR)"


ex1.exe: ex1/main.cpp ex1/Individu.hpp ../obj/GeneticThread.o
	$(CC) $^ $(FLAGS) -o $@

ex2.exe: ex2/main.cpp ex2/Individu.hpp ../obj/gnuplot_i.o ../obj/benchmarks-func.o ../obj/GeneticThread.o
	#"$(TOP)/src/GeneticThread.o" "$(TOP)/src/benchmarks-func.o"
	$(CC) $^ $(FLAGS) -o $@

main.exe : ex1.exe ex2.exe


clean:
	@rm -f *.o
	@rm -f ex1/*.o
	@rm -f ex2/*.o
	@rm -f *.exe
	@rm -f gnuplot/src/gnuplot_i.o
