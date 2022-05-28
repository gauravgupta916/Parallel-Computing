CC = g++
PY = python3

all:
	$(CC) matrix_generator.cpp -o matrix_generator.out
	$(CC) serial.cpp -o serial.out
	$(CC) -pthread pthread.cpp -o pthread.out
	$(CC) -fopenmp omp.cpp -o omp.out
	./looper.sh
	$(PY) graph_plot.py

clean:
	rm *.out
	rm *.txt
	





