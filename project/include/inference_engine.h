#ifndef IN_EN_H
#define IN_EN_H

#include "JInferenceEngine.hxx"

class InferenceEngine
{
public:
	InferenceEngine(int Nnodes, int* cardinality, int* Nparents, int** parents, int* Ncpt, double** cpt, char* filename) {}
	~InferenceEngine();
	void updateEvidence(int* evidence) { return;}
	double probability(int x, int u) {return 0.5;}
private:

	JavaVM* jvm;
	JInferenceEngine* jEngine;

};

#endif
