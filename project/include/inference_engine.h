#ifndef IN_EN_H
#define IN_EN_H

// PD: TODO

class InferenceEngine
{
public:
	InferenceEngine(int Nnodes, int* m_cardinality, int* m_Nparents, int** m_parents, int* m_Ncpt, double** m_cpt) {};
	void updateEvidence(int* evidence) { return;}
	double probability(int x, int u) {return 0.5;}
private:
};

#endif
