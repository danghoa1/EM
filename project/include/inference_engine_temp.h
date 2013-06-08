#ifndef IN_EN_H
#define IN_EN_H

class InferenceEngine
{
public:
	InferenceEngine(char* filepath){ return;}
	~InferenceEngine() {return;}
	void updateCPTs(int Nnodes, double** newcpts,int* Ncpt) { m_Ncpt = Ncpt;}
	void loadEvidence(int** dataset,int datasetLength,  int Nnodes) { return; }
	void updateEvidence(int index) {return;}
	double* tableConditional(int x)
	{
		double* a = new double[m_Ncpt[x]];
		return a;
	};
	double probability(int x, int u) { return 0.5;}
private:
	int * m_Ncpt;
};

#endif
