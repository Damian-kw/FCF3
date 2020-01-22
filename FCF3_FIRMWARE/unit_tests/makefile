INCLUDE_DIR= "-I../Inc/user/"
SALLOC_DIR= "../Src/user/salloc.c"
tests_main: tests_main.cpp
			g++ -c tests_main.cpp -o tests_main.o 
tests: tests_main.o
		g++  $(INCLUDE_DIR) -c $(SALLOC_DIR) -o salloc.o
		g++   $(INCLUDE_DIR) -c tests_salloc.cpp  -o tests_salloc.o
		g++  salloc.o tests_main.o tests_salloc.o -o tests.out
clean: 
		rm -rf *.o *.out