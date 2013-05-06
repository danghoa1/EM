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
	int c;
	opterr = 1;

	while ((c = getopt (argc, argv, "i:")) != -1)
	{
		switch (c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;
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
	
	// Read network
	sw.Start();
	BayesNetwork net(BayesNetwork::IL2);
	net.readNetwork(ifile);
	sw.End();
	sw.Print("Read network:");

	// Print
	net.print();
}
