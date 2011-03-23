all:
	g++ cpuMandel_omp.cpp -o cpuMandel_omp -O4 -fopenmp
	g++ -I/usr/local/include -L/usr/local/lib -ltbb cpuMandel_tbb.cpp -o cpuMandel_tbb -O4
