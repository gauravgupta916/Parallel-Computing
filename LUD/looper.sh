#!/usr/bin/bash



./matrix_generator.out 1000 > matrix_gen.txt
echo "n thread = Serial pthread OpenMP" > final_output.txt

#for loop
for (( n=100;n<1001;n=n+100 ))
do
	#input file
	./serial.out $n matrix_gen.txt > serial_n.txt
	for(( threads=1;threads<7;threads=threads+1  ))
	do
		echo -n $n $threads =' '>> final_output.txt
		#serial LU execution
		cat serial_n.txt >> final_output.txt
		#pthread LU execution
		./pthread.out $n matrix_gen.txt $threads >> final_output.txt
		#omp LU execution
		export OMP_NUM_THREADS=$threads
		./omp.out $n matrix_gen.txt >> final_output.txt
	done
done






