#ifndef IN_EN_H
#define IN_EN_H

#include "IL2.hxx"

using namespace inflib;

class InferenceEngine
{
public:
	InferenceEngine(char* filepath);
	~InferenceEngine();
	void updateCPTs(int Nnodes, double** newcpts,int* Ncpt);
	void loadEvidence(int** dataset,int datasetLength,  int Nnodes);
	void updateEvidence(int index);
	double* tableConditional(int x);
	double probability(int x, int u);
private:

	JavaVM* jvm;
	IL2* jEngine;

// Network Info

	char* m_filepath;

// private functions
	JavaVM* create_vm();


};

#endif
