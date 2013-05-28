simulator:

-i <path/file> : path to bayesian network input file

-o <path/file> : path to simulated dataset output file

-s <int> : seed value

-n <int> : number of cases in dataset

-h : toogle to simulate incomplete data


ml:

-i <path/file> : path to bayesian network input file

-d <path/file> : path to simulated dataset input file


valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes debug/em -i network/alarm-seed.uai -d output/alarm.dat

