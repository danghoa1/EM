#ifndef BSNW_H
#define BSNW_H

#include <iostream>

const int MAX_NUMBER_OF_NODES = 50;
const int MAX_NUMBER_OF_PARENTS = 10;
const int MAX_CPT_LENGTH = 1024;

using namespace std;

class BayesNetwork
{
public:
	BayesNetwork();
	int positionInCPT(int* data, int i);
	virtual void Read(const char* networkFilePath);				//Create network from file
	virtual void Learn(const char* datasetFilePath);			//Learn parameters from file
	virtual void Simulate(const char* simulateDatasetFilePath, int Ncases);	//Simulate dataset from network
	virtual void Print();
protected:

	struct Node
	{
		int parents[MAX_NUMBER_OF_PARENTS];
		int Nparents;
		
		int cardinality;
		
		double cpt[MAX_CPT_LENGTH];
		int Ncpt;	
	};

	Node nodes[MAX_NUMBER_OF_NODES];
	int Nnodes;	
};


#endif