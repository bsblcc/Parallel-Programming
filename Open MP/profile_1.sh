
rm profile_1.out



for size in 50 100 200 500 1000 2000 3000
do
	for t in $(seq 1 3)
	do

		./matrix_generator.o $size

		for p in row_static.o row_guided.o row_dynamic.o block_static.o block_dynamic.o block_guided.o gaussian_avx.o
		do
			echo "running size: $size, time: $t, $p"
			./$p >> profile_1.out
		done




	done



done