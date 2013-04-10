#ifndef BSNW_M
#define BSNW_M

#include "bayesian_network_with_timer.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>

using namespace std;

void BayesNetworkWithTimer::Read(const char* networkFilePath)
{
	clock_t start = clock();
	BayesNetwork::Read(networkFilePath);
	clock_t end = clock();
	cout<<"Reading network file took "<<((double)(end-start))*1000/CLOCKS_PER_SEC<<" ms."<<endl<<endl;
}

void BayesNetworkWithTimer::Learn(const char* datasetFilePath)
{
	clock_t start = clock();
	BayesNetwork::Learn(datasetFilePath);
	clock_t end = clock();
	cout<<"Reading and learning dataset file took "<<((double)(end-start))*1000/CLOCKS_PER_SEC<<" ms."<<endl<<endl;
}

void BayesNetworkWithTimer::Print()
{
	BayesNetwork::Print();
}

#endif
