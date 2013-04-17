#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "bayesian_network.h"
#include "stopwatch.h"

using namespace std;

int main(int argc, char **argv)
{
	int iflag = 0;			//input file
	char* ivalue = NULL;
	int oflag = 0;			//output file
	char* ovalue = NULL;
	int sflag = 0;			//seed
	int svalue = 0;
	int nflag = 0;			//dataset count
	int nvalue = 0;
	int c;
	opterr = 1;

	while ((c = getopt (argc, argv, "i:o:s:n:")) != -1)
	{
		switch (c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;
				break;
			case 'o':
				oflag = 1;
				ovalue = optarg;
				break;
			case 's':
				sflag = 1;
				svalue = atoi(optarg);
				break;
			case 'n':
				nflag = 1;
				nvalue = atoi(optarg);
				break;
			default:
				abort();
		}
	}
	
	//Initialize
	Stopwatch sw;

	char* ifile = "network/network1.uai";
	if (iflag == 1)
		ifile = ivalue;
	char* ofile = "output/simulate1.dat";	
	if (oflag == 1)
		ofile = ovalue;
	
	int seed = time(NULL);
	if (sflag == 1)
		seed = svalue;

	int ncases = 2000000;
	if (nflag == 1)
		ncases = nvalue;

	// Read network
	sw.Start();
	BayesNetwork net;
	net.Read(ifile);
	sw.End();
	sw.Print("Read network:");

	// Simulate
	sw.Start();
	net.Simulate(ofile,ncases,incomplete,seed);
	sw.End();
	sw.Print("Simulate dataset:");	

	// Maximum Likelihood
	sw.Start();
	net.Learn("output/simulate1.dat");
	sw.End();
	sw.Print("Maximum Likelihood:");
	net.Print();
}
