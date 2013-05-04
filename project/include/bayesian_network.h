#ifndef BSNW_H
#define BSNW_H

class BayesNetwork
{
public:
	BayesNetwork();
	~BayesNetwork();
	void readNetwork(char* networkFilePath);	//Create network from file
	void readDataset(char* datasetFilePath);	//Store dataset from file
	void learnML();					//Learn from complete data
	void learnEM();					//Learn from incomplete data
	void simulate(const char* simulateDatasetFilePath, int Ncases, double hideProbability, int seed);	//Simulate dataset from network
	void EM();
	void print();
protected:

// Network Info
	
	int m_Nnodes;		// Number of nodes
	int* m_cardinality;	// Cardinality for each node
	int* m_Nparents;	// Number of parents for each node
	int** m_parents;	// Array of parents for each node
	int* m_Ncpt;		// CPT Length for each node
	double** m_cpt;		// CPT table for each node
	char* m_filePath;

// Dataset Info
	bool m_incomplete;
	int** m_dataset;
	int m_Ncases;

// Helper functions
	int positionInCPT(int* data, int i);	
	void normalizeCPT(int i, int cardinality, int Ncpt, double* cpt);
};


#endif
