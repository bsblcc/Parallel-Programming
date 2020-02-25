
obj_path = ./obj
SOURCE = buffer.cpp matrix.cpp worker.cpp master.cpp gaussian_mpi.cpp

#build : main.o


build : $(obj_path)/main.o $(obj_path)/matrix_generator.o



#$(obj_path)/main.o : $(SOURCE)
#	mpic++ $(SOURCE) -o $(obj_path)/main.o -std=c++11 -fopenmp -march=native


$(obj_path)/main.o : $(obj_path)/buffer.o $(obj_path)/matrix.o $(obj_path)/worker.o $(obj_path)/master.o $(obj_path)/gaussian_mpi.o $(obj_path)/common.o
	mpic++ -fopenmp -march=native $(obj_path)/common.o $(obj_path)/buffer.o $(obj_path)/matrix.o $(obj_path)/worker.o $(obj_path)/master.o $(obj_path)/gaussian_mpi.o -o $(obj_path)/main.o

#main.o : main.cpp main.h
#	mpic++ main.cpp -o $(obj_path)/main.o



$(obj_path)/buffer.o : buffer.cpp
	mpic++ buffer.cpp -o $(obj_path)/buffer.o -c -std=c++11



$(obj_path)/matrix.o : matrix.cpp
	mpic++ matrix.cpp -o $(obj_path)/matrix.o -c -std=c++11


$(obj_path)/worker.o : worker.cpp
	mpic++ worker.cpp -o $(obj_path)/worker.o -c -std=c++11 -fopenmp -march=native


$(obj_path)/common.o : common.cpp
	mpic++ common.cpp -o $(obj_path)/common.o -c -std=c++11

$(obj_path)/master.o : master.cpp
	mpic++ master.cpp -o $(obj_path)/master.o -c -std=c++11



$(obj_path)/gaussian_mpi.o : gaussian_mpi.cpp
	mpic++ gaussian_mpi.cpp -o $(obj_path)/gaussian_mpi.o -c -std=c++11



$(obj_path)/matrix_generator.o : matrix_generator.cpp
	clang++ -std=c++14 -o $(obj_path)/matrix_generator.o matrix_generator.cpp