#ifndef BSNW_H
#define BSNW_H

#include <iostream>
#include <vector>

const int MAX_NUMBER_OF_NODES = 50;
const int MAX_NUMBER_OF_PARENTS = 10;
const int MAX_CPT_LENGTH = 1024;
const int MEMORY_ALLOCATED = 100000;

using namespace std;

class BayesNetwork
{
public:
	BayesNetwork();
	~BayesNetwork();
	void ReadNetwork(char* networkFilePath);	//Create network from file
	void ReadDataset(char* datasetFilePath);	//Store dataset from file
	void Learn(const char* datasetFilePath);	//Learn parameters from dataset
	void Simulate(const char* simulateDatasetFilePath, int Ncases, bool incomplete, int seed);	//Simulate dataset from network
	void EM(const char* datasetFilePath);
	virtual void Print();
protected:

// Network Info
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

// Dataset Info
	vector< int *> dataset;
	int Ncases;

// Helper functions
	int positionInCPT(int* data, int i);	
	void normalizeCPT(int i);
};


#endif
