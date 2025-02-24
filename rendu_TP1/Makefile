CC=gcc
CFLAGS=-O2 -g -Wall
OPTFLAGS=-O3 -g -Wall -march=native -funroll-loops -ftree-vectorize -ffast-math
OPTPARFLAGS= $(OPTFLAGS) -fopenmp
OBJS_COMMON=rdtsc.o

all: prep build/measure build/measure_opti build/measure_opti_parallel
	export OMP_PLACES=cores && export OMP_PROC_BIND=close

	# maqao OV -R1 xp=baseline_report              --replace -- build/measure               200 1000 20000
	# maqao OV -R1 xp=optimisation_report          --replace -- build/measure_opti          200 1000 20000
	# maqao OV -R1 xp=optimisation_parallel_report --replace -- build/measure_opti_parallel 200 1000 20000
	# maqao OV -CR xp=compare_report --inputs=baseline_report,optimisation_report,optimisation_parallel_report

	# maqao OV -R1 xp=baseline_report              --replace -- build/measure               7000 10 50
	maqao OV -R1 xp=optimisation_parallel_report_2 --replace -- build/measure_opti_parallel 6000 10 150
	maqao OV -R1 xp=optimisation_report_2          --replace -- build/measure_opti          6000 10 150
	# maqao OV -CR xp=compare_report_2 --inputs=optimisation_report_2,optimisation_parallel_report_2

	# maqao oneview -R1 --with-scalability=strong -xp=compare_report_strong_scal --replace -c=strong_scalability.json
	# maqao oneview -R1 --with-scalability=strong -xp=compare_report_strong_scal_6000 --replace -c=strong_scalability_6000.json



prep:
	mkdir -p build

build/measure: $(OBJS_COMMON) driver.c
	$(CC) -o $@ $^ $(CFLAGS)

build/measure_opti: $(OBJS_COMMON) driver_opti.c
	$(CC) -o $@ $^ $(OPTFLAGS)

build/measure_opti_parallel: $(OBJS_COMMON) driver_opti.c
	$(CC) -o $@ $^ $(OPTPARFLAGS)

clean:
	rm -rf $(OBJS_COMMON) build
