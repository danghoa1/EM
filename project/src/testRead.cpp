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
	int tflag = 0;
	int tvalue = 0;
	int c;
	opterr = 1;

	while ((c = getopt (argc, argv, "i:t:")) != -1)
	{
		switch (c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;
				break;
			case 't':
				tflag = 1;
				tvalue = atoi(optarg);
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
	
	BayesNetwork::NetworkType type = BayesNetwork::IL2;
	if (tflag == 1 && tvalue == 1)
		type = BayesNetwork::IL1;
	
	// Read network
	sw.Start();
	BayesNetwork net(type);
	net.readNetwork(ifile);
	sw.End();
	sw.Print("Read network:");

	// Print
	net.print();
}
