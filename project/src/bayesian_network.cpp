#ifndef BSNW_M
#define BSNW_M

#include "bayesian_network.h"
#include "random_generator.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

BayesNetwork::BayesNetwork()
{
	m_Nnodes = 0;
	m_Ncases = 0;
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
// Helper Functions ------------------------------------

void BayesNetwork::normalizeCPT(int i)
{
	int j=0;
	while (j < m_Ncpt[i])
	{
		double total = 0;
		for (int k=0; k < m_cardinality[i]; k++)
			total += m_cpt[i][j+k];
		
		for (int k=0; k < m_cardinality[i]; k++)
			m_cpt[i][j+k] /= total;
		
		j += m_cardinality[i];
	}
}


int BayesNetwork::positionInCPT(int* data, int i)
{
	int CPTposition = data[i];
	int multiplier = m_cardinality[i];
	for (int j = (m_Nparents[i] - 1); j>=0; j--)
	{
		CPTposition += data[m_parents[i][j]] * multiplier;
		multiplier *= m_cardinality[m_parents[i][j]];
	}
	return CPTposition;
}

// Main functions -----------------------------------------

void BayesNetwork::ReadNetwork(char* networkFilePath)
{
	//Initialize

	ifstream netFile;
	netFile.open(networkFilePath);
	
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
		m_parents[i] = new int[m_Nparents[i]];
		
		// Insert parents
		for (int j=0; j < numVariables; j++)
		{
			int parent;
			netFile >> parent;

			if (j < numVariables)
				m_parents[i][j] = parent;
		}
		
	}
	
	// Read CPT Table
	
	for (int i=0; i < m_Nnodes; i++)
	{
		netFile >> m_Ncpt[i];

		// Initialize CPT table
		m_cpt[i] = new double[m_Ncpt[i]];

		for (int j=0; j< m_Ncpt[i]; j++)
			netFile >> m_cpt[i][j];	
	}
	
	
	netFile.close();
}

void BayesNetwork::Learn()
{

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
		normalizeCPT(i);
	}
}

void BayesNetwork::ReadDataset(char* datasetFilePath)
{
	//Initialize & Set up unnormalized CPT Table 

	ifstream dsFile;
	dsFile.open(datasetFilePath);
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
				if (str == "?") 		// Incomplete data
				{
					val = -1;
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

void BayesNetwork::Simulate(const char* simulateDatasetFilePath, int Ncases, bool incomplete, int seed)
{
	
	//Initialize & Set up unnormalized CPT Table 
	ofstream dsFile;
	dsFile.open(simulateDatasetFilePath);
	

	// Print number of cases
	dsFile<<Ncases<<endl;

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
			dsFile<<"s"<<dataRow[i];
			if ( i < (m_Nnodes -1))
				dsFile<<",";
			else
				dsFile<<endl;
		}
	}
	dsFile.close();	
}

void BayesNetwork::Print()
{
	cout.setf(ios::fixed); 
    	cout.precision(5);
	
	for(int i=0; i< m_Nnodes; i++)
	{
		cout<<"Node "<<i<<"'s CPT:"<<endl;		

		for (int j=0; j < m_Ncpt[i]; j++)
		{
			cout<< m_cpt[i][j];
			if ((j+1) % m_cardinality[i] == 0) cout<<endl;
			else cout<<"  ";
		}
		cout<<endl;
	}
}

#endif
