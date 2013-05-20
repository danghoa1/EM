#ifndef BSNW_M
#define BSNW_M

#include "bayesian_network.h"
#include "random_generator.h"
#include "inference_engine.h"
#include "stopwatch.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

BayesNetwork::BayesNetwork(NetworkType type)
{
	m_Nnodes = 0;
	m_Ncases = 0;
	m_dataset = NULL;
	m_parents = NULL;
	m_Nparents = NULL;
	m_cardinality = NULL;
	m_Ncpt = NULL;
	m_cpt = NULL;
	m_type = type;
}

BayesNetwork::~BayesNetwork()
{
	if (m_dataset != NULL)
	{
		for(int i=0; i < m_Ncases; i++)
			delete [] m_dataset[i];
	}

	if (m_parents != NULL)
	{
		for(int i=0; i < m_Nnodes; i++)
		{
			delete [] m_parents[i];
			delete [] m_cpt[i];
		}
	}

	if (m_Nparents != NULL) delete [] m_Nparents;
	if (m_Ncpt != NULL) delete [] m_Ncpt;
	if (m_cardinality != NULL) delete [] m_cardinality;
}

// Main functions -----------------------------------------

void BayesNetwork::readNetwork(char* networkFilePath)
{
	//Initialize

	ifstream netFile;
	netFile.open(networkFilePath);
	m_filePath = networkFilePath;
	
	// Read network type

	string networkType;
	getline(netFile,networkType);	

	// Read number of nodes

	netFile>> m_Nnodes;

	// Intialize arrays

	m_parents = new int*[m_Nnodes];
	m_Nparents = new int[m_Nnodes];
	m_cardinality = new int[m_Nnodes];
	m_cpt = new double*[m_Nnodes];
	m_Ncpt = new int[m_Nnodes];


	// Read the number of states for each node

	for(int i=0; i < m_Nnodes; i++)
	{
		int cardinality;
		netFile>>cardinality;
		
		// Create new node
		m_cardinality[i] = cardinality;
		m_Nparents[i] = 0;
	}

	// Read the number of CPT tables

	int Nfunctions;
	netFile>>Nfunctions;

	// Read each clique

	for(int i=0; i < Nfunctions; i++)
	{
		int numVariables;
		netFile>>numVariables;
		
		// Initialize parent array
		m_Nparents[i] = numVariables - 1;

		if (m_Nparents[i]>0)
		{
			m_parents[i] = new int[m_Nparents[i]];
			
			// Insert parents
			for (int j=0; j < m_Nparents[i]; j++)
				netFile >> m_parents[i][j];
		}
		else m_parents[i] = NULL;	
		int dummy;
		netFile >> dummy;
	}
	
	// Read CPT Table
	
	for (int i=0; i < m_Nnodes; i++)
	{
		netFile >> m_Ncpt[i];
		// Initialize CPT table
		m_cpt[i] = new double[m_Ncpt[i]];

		if (m_type == IL1)		// IL1 Format
		{
			for (int j=0; j< m_Ncpt[i]; j++)
				netFile >> m_cpt[i][j];	
		}
		else if (m_type == IL2)	// IL2 Format
		{
			for (int j=0; j < m_Ncpt[i]; j++)
			{
				int il2 = IL1ToIL2(i,j);
				netFile >> m_cpt[i][il2];
			}	

		}
	}
	
	netFile.close();
}

void BayesNetwork::learnML()
{
	if (m_incomplete == false)
	{
		cout <<"Complete data"<<endl;
		for (int c=0; c < m_Ncases; c++)
		{

			for(int i=0; i < m_Nnodes; i++)
			{
				int CPTposition = positionInCPT(m_dataset[c],i);
				m_cpt[i][CPTposition] +=1;
			}	
		}
	
		//Normalize
		for (int i =0; i < m_Nnodes; i++)
		{
			normalizeCPT(i,m_cardinality[i],m_Ncpt[i],m_cpt[i]);
		}
	}
	else
	{
		cout<<endl<<"Incomplete data.  No learning is done."<<endl<<endl;
	}
}

void BayesNetwork::learnEM()
{

	Stopwatch memorySW;
	Stopwatch inferenceSW;
	Stopwatch normalizeSW;


	memorySW.on();

	const int MAX_NUMBER_OF_ITERATION = 100;

	double*** theta = new double**[MAX_NUMBER_OF_ITERATION];
	
	//Initialize theta(0)  TODO
	double** zero = new double*[m_Nnodes];
	for (int i=0; i<m_Nnodes;i++)
	{
		double* cpt = new double[m_Ncpt[i]];
		double initValue = 1.0 / m_cardinality[i];
		
		for (int j=0; j<m_Ncpt[i]; j++)
			cpt[j] = initValue;

		zero[i] = cpt;
	}
	theta[0] = zero;

	memorySW.off();
	
	// Initialize Inference Engine
	inferenceSW.on();
	InferenceEngine engine(m_filePath);
	inferenceSW.off();
	

	//Iterate
	int k = 0;
	while ((k+1)<MAX_NUMBER_OF_ITERATION)
	{
		k++;
		// Update CPTs
		inferenceSW.on();
		engine.updateCPTs(m_Nnodes,theta[k-1],m_Ncpt);

		inferenceSW.off();

		// Initialize New Array
		memorySW.on();
		double** cpts = new double*[m_Nnodes];
		for (int i=0; i<m_Nnodes;i++)
		{
			double* cpt = new double[m_Ncpt[i]];
			
			for (int j=0; j< m_Ncpt[i]; j++)
				cpt[j]  =0;

			cpts[i] = cpt;
		}
		memorySW.off();

		// Iterate over datasets
		inferenceSW.on();

		for (int i=0; i < m_Ncases;i++)
		{
			// Update evidence
			engine.updateEvidence(m_dataset[i],m_Nnodes);

			for (int x=0; x < m_Nnodes; x++)
			{
				for (int u=0; u < m_Ncpt[x]; u++)
				{
					cpts[x][u]  += engine.probability(x,u);
				}
			}
		}
		inferenceSW.off();

		// Update theta
		normalizeSW.on();
		for (int i=0; i < m_Nnodes; i++)
		{
			normalizeCPT(i,m_cardinality[i],m_Ncpt[i], cpts[i]);		
		}
		normalizeSW.off();

		theta[k] = cpts;
	}

	// Copy cpts
	normalizeSW.on();
	for (int i=0; i < m_Nnodes; i++)
		for (int j=0; j < m_Ncpt[i]; j++)
			m_cpt[i][j] = theta[k][i][j];
	normalizeSW.off();

	memorySW.on();
	// Dealloc
	for (int i=0; i <= k; i++)
	{
		for (int j=0; j < m_Nnodes; j++)
		{
			delete [] theta[i][j];
		}
		delete [] theta[i];
	}
	delete [] theta;
	
	memorySW.off();
	
	inferenceSW.print("Inference:");
	memorySW.print("Memory:");
	normalizeSW.print("Normalize:");

}

void BayesNetwork::readDataset(char* datasetFilePath)
{
	//Initialize & Set up unnormalized CPT Table 
	
	m_incomplete = false;

	ifstream dsFile;
	dsFile.open(datasetFilePath);
	int Nnodes;
	dsFile >> Nnodes;
	dsFile >> m_Ncases;
	m_dataset = new int*[m_Ncases];


	//Set up header ordering

	int header[m_Nnodes];

	for (int i=0; i < m_Nnodes; i++)
	{
		string str;
		if (i+1 < m_Nnodes)
			getline(dsFile,str,',');
		else
			getline(dsFile,str,'\n');
		str = str.substr(1);			//Remove the first letter (eg: x or s)	
		
		const char* cstr = str.c_str();
		int val = atoi(cstr);
		header[i] = val;
	}

	//Read line by line

	int readCount = 0;
	
	while (dsFile.eof()==false)
	{
		int* a = new int[m_Nnodes];
		bool legalCase= true;		
		for (int i=0; i< m_Nnodes; i++)
		{
			string str;
			if (i+1 < m_Nnodes)
				getline(dsFile,str,',');
			else
				getline(dsFile,str,'\n');
			
			if (str != "")
			{
				int val;
				if (str.length()==1) 		// Incomplete data
				{
					val = -1;
					m_incomplete = true;
				}
				else
				{
					str = str.substr(1);
					const char* cstr = str.c_str();
					val = atoi(cstr);
				}
				a[header[i]] = val;		//Order according to header
			}
			else 
			{
				legalCase = false;
				break;
			}
		}
		
		// Insert data row
		if (legalCase)
		{
			m_dataset[readCount] = a;
			readCount++;
		}
	}
	dsFile.close();
}

void BayesNetwork::simulate(const char* simulateDatasetFilePath, int Ncases, double hideProbability, int seed)
{
	
	//Initialize & Set up unnormalized CPT Table 
	ofstream dsFile;
	dsFile.open(simulateDatasetFilePath);
	

	// Print number of cases & number of nodes
	dsFile<<m_Nnodes<<" "<<Ncases<<endl;



	//Print header
	for (int i=0; i<m_Nnodes;i++)
	{
		dsFile<<"x"<<i;
		if ( i < (m_Nnodes -1))
			dsFile<<",";
		else
			dsFile<<endl;
	}

	
	//Generate m_dataset cases, assuming topological order
	
	RandomGenerator randomGen(seed);
	
	for (int c=1; c <= Ncases; c++)
	{
		int dataRow[m_Nnodes];
		for (int i=0; i < m_Nnodes; i++)
		{
			dataRow[i] = 0;	
			
			// Find starting position in CPT
			int CPTposition = positionInCPT(dataRow,i);
			
			do
			{
				double accumulate = 0;
				double rand = randomGen.Randomize();
			
			
				for (int j=0; j<m_cardinality[i]; j++)
				{
					accumulate += m_cpt[i][CPTposition+j];
					if (rand <= accumulate)
					{
						dataRow[i] = j;
						break;
					}
				}
				if (rand <= accumulate) break;
			} while (false);
		}
		
		//Print row
		for (int i=0; i<m_Nnodes;i++)
		{
			double rand = randomGen.Randomize();

			// Hide data
			if (rand<hideProbability)		
				dsFile<<"?";
			else
				dsFile<<"s"<<dataRow[i];

			// Add "," or new line
			if ( i < (m_Nnodes -1))
				dsFile<<",";
			else
				dsFile<<endl;
		}
	}
	dsFile.close();	
}

void BayesNetwork::print()
{
	cout.setf(ios::fixed); 
    	cout.precision(5);
	
	cout<<endl;

	for(int i=0; i< m_Nnodes; i++)
	{
		cout<<"Node "<<i<<"'s CPT:"<<endl;		

		for (int j=0; j < m_Ncpt[i]; j++)
		{
			if (m_type == IL1)		// IL1 format
				cout<< m_cpt[i][j];
			else if (m_type == IL2)		// IL2 format
				cout << m_cpt[i][IL1ToIL2(i,j)];

			if ((j+1) % m_cardinality[i] == 0) cout<<endl;
			else cout<<"  ";
		}
		cout<<endl;
	}

	cout<<endl;
}

// Helper Functions ------------------------------------

void BayesNetwork::normalizeCPT(int i, int cardinality, int Ncpt, double* cpt)
{
	if (m_type == IL1)		// IL1 format
	{
		int j=0;
		while (j < Ncpt)
		{
			double total = 0;
			for (int k=0; k < cardinality; k++)
				total += cpt[j+k];

			if (total > 0)
			{
				for (int k=0; k < cardinality; k++)
					cpt[j+k] /= total;
			}
			else
			{
				for (int k=0; k < cardinality; k++)
					cpt[j+k] = 1.0 / cardinality;
			}

			j += cardinality;
		}
	}
	else if (m_type == IL2)	// IL2 format
	{
		int multiplier = Ncpt / cardinality;
		for (int j=0; j < multiplier; j++)
		{
			double total = 0;
			for (int k=0; k < cardinality; k++)
				total += cpt[j + k*multiplier];
			if (total > 0)
			{
				for (int k=0; k < cardinality; k++)
					cpt[j + k*multiplier] /= total;
			}
			else
			{
				for (int k=0; k < cardinality; k++)
					cpt[j + k*multiplier] = 1.0 / cardinality;
			}
		}
	}
}


int BayesNetwork::positionInCPT(int* data, int i)
{
	int CPTposition = 0;
	if (m_type == IL1)		// IL1 format
	{
		CPTposition = data[i];
		int multiplier = m_cardinality[i];
		for (int j = (m_Nparents[i] - 1); j>=0; j--)
		{
			CPTposition += data[m_parents[i][j]] * multiplier;
			multiplier *= m_cardinality[m_parents[i][j]];
		}
	}
	else if (m_type == IL2)	// IL2 format
	{	
		CPTposition = 0;
		int multiplier = 1;
		for (int j = 0; j < m_Nparents[i]; j++)
		{
			CPTposition += data[m_parents[i][j]] * multiplier;
			multiplier *= m_cardinality[m_parents[i][j]];
		}
		CPTposition += data[i] * multiplier;
	}
	return CPTposition;
}

int BayesNetwork::IL1ToIL2(int node, int il1)
{
	int il2=0;
	int multiplier = m_Ncpt[node];
	int multiplier2 = 1;
	for (int i=0;  i < m_Nparents[node]; i++)
	{
		multiplier /= m_cardinality[m_parents[node][i]];
		int val = il1 / multiplier;
		il1 %= multiplier;

		il2 += val * multiplier2;
		multiplier2 *= m_cardinality[m_parents[node][i]];
	}

	il2 += il1 * multiplier2;
	return il2;
}


#endif
