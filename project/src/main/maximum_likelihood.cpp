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
	int dflag = 0;			//dataset file
	char* dvalue = NULL;
	int c;
	opterr = 1;

	while ((c = getopt (argc, argv, "i:d:")) != -1)
	{
		switch (c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;
				break;
			case 'd':
				dflag = 1;
				dvalue = optarg;
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
	char* dfile = "output/simulate1.dat";	
	if (dflag == 1)
		dfile = dvalue;
	
	// Read network
	sw.on();
	BayesNetwork net;
	net.readNetwork(ifile);
	sw.off();
	sw.print("Read network:");

	// Read dataset
	sw.on();
	net.readDataset(dfile);
	sw.off();
	sw.print("Read dataset:");

	// Maximum Likelihood
	sw.on();
	net.learnML();
	sw.off();
	sw.print("Maximum Likelihood:");
	net.print();
}
