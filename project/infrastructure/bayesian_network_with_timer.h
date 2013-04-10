#ifndef BSNWWT_H
#define BSNWWT_H

#include <iostream>
#include "bayesian_network.h"


using namespace std;

class BayesNetworkWithTimer : BayesNetwork
{
public:
	virtual void Read(const char* networkFilePath);		//Create network from file
	virtual void Learn(const char* datasetFilePath);		//Learn parameters from file
	virtual void Print();
};


#endif
