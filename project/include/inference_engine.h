#ifndef IN_EN_H
#define IN_EN_H

#include "IL2.hxx"

using namespace inflib;

class InferenceEngine
{
public:
	InferenceEngine(int Nnodes, int* cardinality, int* Nparents, int** parents, int* Ncpt, double** cpt, char* filepath);
	~InferenceEngine();
	void updateCPTs(double** newcpts);
	void updateEvidence(int* evidence);
	double probability(int x, int u);
private:

	JavaVM* jvm;
	IL2* jEngine;

// Network Info

	int m_Nnodes;           // Number of nodes
	int* m_cardinality;     // Cardinality for each node
	int* m_Nparents;        // Number of parents for each node
	int** m_parents;        // Array of parents for each node
	int* m_Ncpt;            // CPT Length for each node
	double** m_cpt;         // CPT table for each node
	char* m_filepath;

// private functions
	JavaVM* create_vm();


};

#endif
