gen_maqao: baseline opti opti_v2
	# maqao OV -R1 xp=report_baseline --replace -c=config/baseline.json
	# maqao OV -R1 xp=report_opti_v1  --replace -c=config/opti_v1.json
	maqao OV -R1 xp=report_opti_v2  --replace -c=config/opti_v2.json

	maqao OV -CR xp=compare_report  --replace --inputs=report_baseline,report_opti_v1,report_opti_v2
baseline:
	gcc -O2 -g -fno-omit-frame-pointer max_in_grid.c -o exe

opti:
	gcc -O3 -g -funroll-loops -march=native opti_v1.c -o exe_opti_v1

opti_v2:
	gcc -O3 -g -funroll-loops -march=native opti_v2.c -o exe_opti_v2

clean:
	rm exe exe_opti_v1 exe_opti_v2