#ifndef BSNW_H
#define BSNW_H

class BayesNetwork
{
public:

	enum NetworkType {
		IL1 = 1,
		IL2 = 2
	};

	BayesNetwork(NetworkType type = IL2);
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
	enum NetworkType m_type;	// IL1 or IL2


// Dataset Info
	bool m_incomplete;
	int** m_dataset;
	int m_Ncases;

// Private functions

void normalizeCPT(int i, int cardinality, int Ncpt, double* cpt);

int positionInCPT(int* data, int i);

int IL1ToIL2(int node, int il1);

};

#endif
