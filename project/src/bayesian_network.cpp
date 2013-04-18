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
	Nnodes = 0;
	Ncases = 0;
}

BayesNetwork::~BayesNetwork()
{
	for(int i=0; i < Ncases; i++)
		delete [] dataset[i];
}
// Helper Functions ------------------------------------

void BayesNetwork::normalizeCPT(int i)
{
	int j=0;
	while (j < this->nodes[i].Ncpt)
	{
		double total = 0;
		for (int k=0; k < this->nodes[i].cardinality; k++)
			total += this->nodes[i].cpt[j+k];
		
		for (int k=0; k < this->nodes[i].cardinality; k++)
			this->nodes[i].cpt[j+k] /= total;
		
		j += this->nodes[i].cardinality;
	}
}


int BayesNetwork::positionInCPT(int* data, int i)
{
	int CPTposition = data[i];
	int multiplier = this->nodes[i].cardinality;
	for (int j = (this->nodes[i].Nparents - 1); j>=0; j--)
	{
		CPTposition += data[this->nodes[i].parents[j]] * multiplier;
		multiplier *= this->nodes[this->nodes[i].parents[j]].cardinality;
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

	netFile>> this->Nnodes;

	// Read the number of states for each node

	for(int i=0; i < this->Nnodes; i++)
	{
		int cardinality;
		netFile>>cardinality;
		
		// Create new node
		this->nodes[i].cardinality = cardinality;
		this->nodes[i].Ncpt = cardinality;
		this->nodes[i].Nparents = 0;
	}

	// Read the number of cliques

	int Ncliques;
	netFile>>Ncliques;

	// Read each clique

	for(int i=0; i < Ncliques; i++)
	{
		int numVariables;
		netFile>>numVariables;
		int previous = -1;
		
		// Insert parents
		for (int j=0; j < numVariables; j++)
		{
			int current;
			netFile >> current;
			
			if (previous!=-1)
			{
				this->nodes[current].parents[this->nodes[current].Nparents] = previous;
				this->nodes[current].Nparents++;
				
				this->nodes[current].Ncpt *= this->nodes[previous].cardinality;
			}
			previous = current;
		}
		
	}
	
	// Read CPT Table
	
	for (int i=0; i < this->Nnodes; i++)
	{
		int Nentries;
		netFile >> Nentries;
		for (int j=0; j< this->nodes[i].Ncpt; j++)
			netFile >> this->nodes[i].cpt[j];	
	}
	
	
	netFile.close();
}

void BayesNetwork::Learn()
{

	for (int c=0; c < Ncases; c++)
	{

		for(int i=0; i < this->Nnodes; i++)
		{
			int CPTposition = positionInCPT(dataset[c],i);
			this->nodes[i].cpt[CPTposition] +=1;
		}	
	}
	
	//Normalize
	for (int i =0; i < this->Nnodes; i++)
	{
		normalizeCPT(i);
	}
}

void BayesNetwork::ReadDataset(char* datasetFilePath)
{
	//Initialize & Set up unnormalized CPT Table 

	ifstream dsFile;
	dsFile.open(datasetFilePath);

	//Set up header ordering

	int header[this->Nnodes];

	for (int i=0; i < this->Nnodes; i++)
	{
		string str;
		if (i+1 < this->Nnodes)
			getline(dsFile,str,',');
		else
			getline(dsFile,str,'\n');
		str = str.substr(1);			//Remove the first letter (eg: x or s)	
		
		const char* cstr = str.c_str();
		int val = atoi(cstr);
		header[i] = val;
	}

	//Read line by line

	while (dsFile.eof()==false)
	{
		int* a = new int[this->Nnodes];
		bool legalCase= true;		
		for (int i=0; i< this->Nnodes; i++)
		{
			string str;
			if (i+1 < this->Nnodes)
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
			dataset.push_back(a);
			Ncases++;
		}
	}
	dsFile.close();
}

void BayesNetwork::Simulate(const char* simulateDatasetFilePath, int Ncases, bool incomplete, int seed)
{
	//Initialize & Set up unnormalized CPT Table 
	ofstream dsFile;
	dsFile.open(simulateDatasetFilePath);
	
	//Print header
	for (int i=0; i<this->Nnodes;i++)
	{
		dsFile<<"x"<<i;
		if ( i < (this->Nnodes -1))
			dsFile<<",";
		else
			dsFile<<endl;
	}

	
	//Generate dataset cases, assuming topological order
	
	RandomGenerator randomGen(seed);
	
	for (int c=1; c <= Ncases; c++)
	{
		int dataRow[this->Nnodes];
		for (int i=0; i < this->Nnodes; i++)
		{
			dataRow[i] = 0;	
			
			// Find starting position in CPT
			int CPTposition = positionInCPT(dataRow,i);
			
			do
			{
				double accumulate = 0;
				double rand = randomGen.Randomize();
			
			
				for (int j=0; j<this->nodes[i].cardinality; j++)
				{
					accumulate += this->nodes[i].cpt[CPTposition+j];
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
		for (int i=0; i<this->Nnodes;i++)
		{
			dsFile<<"s"<<dataRow[i];
			if ( i < (this->Nnodes -1))
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
	
	for(int i=0; i< this->Nnodes; i++)
	{
		cout<<"Node "<<i<<"'s CPT:"<<endl;		

		for (int j=0; j < this->nodes[i].Ncpt; j++)
		{
			cout<< this->nodes[i].cpt[j];
			if ((j+1) % this->nodes[i].cardinality == 0) cout<<endl;
			else cout<<"  ";
		}
		cout<<endl;
	}
}

#endif
