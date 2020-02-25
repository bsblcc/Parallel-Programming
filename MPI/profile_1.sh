
make
rm profile_1.out



for size in 100 200 500 1000 2000 3000
do
	for t in $(seq 1 3)
	do

		./obj/matrix_generator.o $size


		./gaussian_avx.o >> profile_1.out
		for p in 5 10 17
		do
			echo "running size: $size, time: $t, $p"
			mpirun -n $p ./obj/main.o >> profile_1.out
		done




	done



done