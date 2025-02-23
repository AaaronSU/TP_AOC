gen_maqao: baseline opti
	# maqao OV -R1 xp=report_baseline --replace -- ./exe         10 2000 3000
	maqao OV -R1 xp=report_opti_v1  --replace -c template_lib.json
baseline:
	gcc -O2 -g -fno-omit-frame-pointer max_in_grid.c -o exe

opti:
	gcc -O3 -g -funroll-loops -march=native opti_v1.c -o exe_opti_v1

clean:
	rm exe exe_opti_v1 