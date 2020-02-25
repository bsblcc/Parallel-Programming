
make
#./obj/matrix_generator.o 22

clang++ gaussian_raw.cpp -o gaussian_raw.o

./gaussian_raw.o
./gaussian_avx.o
mpirun -n 10 ./obj/main.o