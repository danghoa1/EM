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
	void Learn();	//Learn parameters from dataset
	void Simulate(const char* simulateDatasetFilePath, int Ncases, bool incomplete, int seed);	//Simulate dataset from network
	void EM();
	virtual void Print();
protected:

// Network Info
	
	int** m_parents;	// Array of parents for each node
	int* m_Nparents;	// Number of parents for each node
	int* m_cardinality;	// Cardinality for each node
	double** m_cpt;		// CPT table for each node
	int* m_Ncpt;		// CPT Length for each node
	int m_Nnodes;		// Number of nodes

// Dataset Info
	int** m_dataset;
	int m_Ncases;

// Helper functions
	int positionInCPT(int* data, int i);	
	void normalizeCPT(int i);
};


#endif
