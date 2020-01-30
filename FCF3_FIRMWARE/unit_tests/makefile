INCLUDE_DIR= "-I../Inc/user/"
SALLOC_DIR= "../Src/user/salloc.c"
COMPLEMENENTARY_FILTER_DIR= "../Src/user/complementary_filter.c"
tests_main: tests_main.cpp
			g++ -c tests_main.cpp -o tests_main.o 
tests: tests_main.o
		g++  $(INCLUDE_DIR) -c $(SALLOC_DIR) -o salloc.o
		g++   $(INCLUDE_DIR) -c tests_salloc.cpp -o tests_salloc.o
		g++    tests_main.o salloc.o tests_salloc.o   -o tests.out
		./tests.out
clean: 
		rm -rf *.o *.out
tests_complementary_filter: tests_main.o
							g++ $(INCLUDE_DIR) -c $(COMPLEMENENTARY_FILTER_DIR) -o complementary_filter.o
							g++   $(INCLUDE_DIR) -c tests_complementary_filter.cpp -o tests_complementary_filter.o
							g++    tests_main.o complementary_filter.o tests_complementary_filter.o   -o tests.out