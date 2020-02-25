

build : gaussian_raw.o row_static.o row_guided.o row_dynamic.o block_static.o block_dynamic.o block_guided.o matrix_generator.o


matrix_generator.o : matrix_generator.cpp
	clang++ -o matrix_generator.o matrix_generator.cpp



row_static.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o row_static.o -D BY_ROW -D STATIC  -march=native 


row_guided.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o row_guided.o -D BY_ROW -D GUIDED  -march=native 


row_dynamic.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o row_dynamic.o -D BY_ROW -D DYNAMIC  -march=native 

block_static.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o block_static.o -D BY_BLOCK -D STATIC  -march=native 

block_dynamic.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o block_dynamic.o -D BY_BLOCK -D DYNAMIC  -march=native 

block_guided.o : gaussian_omp.cpp gaussian_omp.h
	clang++ gaussian_omp.cpp -o -fopenmp -std=c++14 -o block_guided.o -D BY_BLOCK -D GUIDED  -march=native 


gaussian_raw.o : gaussian_raw.cpp
	clang++ -std=c++14 -o gaussian_raw.o gaussian_raw.cpp