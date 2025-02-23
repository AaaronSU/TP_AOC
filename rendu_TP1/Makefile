CC=gcc
CFLAGS=-O2 -g -Wall
OPTFLAGS=-O3 -g -Wall -march=native -funroll-loops -ftree-vectorize -ffast-math
OPTPARFLAGS= $(OPTFLAGS) -fopenmp
OBJS_COMMON=rdtsc.o

all: prep build/measure build/measure_opti build/measure_opti_parallel
	# maqao OV -R1 xp=baseline_report              --replace -- build/measure               200 1000 20000
	maqao OV -R1 xp=optimisation_report          --replace -- build/measure_opti          200 1000 20000
	maqao OV -R1 xp=optimisation_parallel_report --replace -- build/measure_opti_parallel 200 1000 20000

	# maqao OV -R1 xp=baseline_report              --replace -- build/measure               7000 10 50
	# maqao OV -R1 xp=optimisation_parallel_report --replace -- build/measure_opti_parallel 7000 10 50
	# maqao OV -R1 xp=optimisation_report          --replace -- build/measure_opti          7000 10 50


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
