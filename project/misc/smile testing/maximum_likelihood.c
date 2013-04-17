#include <iostream>
#include "smile/smile.h"
#include "smile/smilearn.h"

using namespace std;

int main()
{
	
	DSL_dataset ds;
    
    	if (ds.ReadFile("simulatedData/em_data.dat") != DSL_OKAY) {
        	cout << "Cannot read data file."<<endl;
        	exit(1);
    	}

	DSL_network net;
  	if (net.ReadFile("simulatedData/em_bayes_network.net", DSL_HUGIN_FORMAT) != DSL_OKAY) {
		cout << "Cannot read network... exiting." << endl;
		exit(1);
  	}

	vector<DSL_datasetMatch> matches;
  	string err;
  	if (ds.MatchNetwork(net, matches, err) != DSL_OKAY) {
     	cout << "Cannot match network... exiting." << endl;
     	exit(1);
  	}

	int numberOfRecords = ds.GetNumberOfRecords();
	
	// Parameters for A

	int unNorm_P_A = 0.5;

	vector<int> recordA = ds.GetIntData(0);
	vector<string> stateNamesA = ds.GetStateNames(0);
	for(int i=0; i< numberOfRecords; i++)
	{
		if (recordA[i] == 1)
			unNorm_P_A ++;
	}
	float P_A = float(unNorm_P_A) / float(numberOfRecords+1);

	cout<<"Parameters for A:"<<endl;
	cout<<" - "<< stateNamesA[1] <<" : "<< P_A <<endl;
	cout<<" - "<< stateNamesA[0] <<" : "<< 1 - P_A <<endl;
	cout<<endl;
	
	// Parameters for B

	vector<int> recordB = ds.GetIntData(1);
	vector<string> stateNamesB = ds.GetStateNames(1);
	
	float unNorm_P_B_given_A = 0;
	float unNorm_P_B_given_notA = 0;
	for (int i=0; i< ds.GetNumberOfRecords();i++)
	if (recordB[i] == 1)
	{
		if (recordA[i] == 1)
			unNorm_P_B_given_A ++;
		if (recordA[i] == 0)
			unNorm_P_B_given_notA ++;
	}

	float P_B_given_A = unNorm_P_B_given_A / (P_A * numberOfRecords);
	float P_B_given_notA = unNorm_P_B_given_notA / ((1- P_A) * numberOfRecords);	

	cout<<"Parameters for B:"<<endl;
	cout<<" - "<< stateNamesB[1] <<" given A is "<< stateNamesA[1] << " : "<< P_B_given_A <<endl;
	cout<<" - "<< stateNamesB[0] <<" given A is "<< stateNamesA[1] << " : "<< 1 - P_B_given_A <<endl;
        cout<<" - "<< stateNamesB[1] <<" given A is "<< stateNamesA[0] << " : "<< P_B_given_notA <<endl;
        cout<<" - "<< stateNamesB[0] <<" given A is "<< stateNamesA[0] << " : "<< 1 - P_B_given_notA <<endl;
	cout<<endl;

	// Parameters for C

        vector<int> recordC = ds.GetIntData(2);
        vector<string> stateNamesC = ds.GetStateNames(2);

        float unNorm_P_C_given_A_B = 0;
        float unNorm_P_C_given_A_notB = 0;
	float unNorm_P_C_given_notA_B = 0;
        float unNorm_P_C_given_notA_notB = 0;

        for (int i=0; i< ds.GetNumberOfRecords();i++)
	if (recordC[i] == 1)
        {
                if (recordA[i] == 1 && recordB[i] == 1)
                        unNorm_P_C_given_A_B ++;
                if (recordA[i] == 1 && recordB[i] == 0)
                        unNorm_P_C_given_A_notB ++;
		if (recordA[i] == 0 && recordB[i] == 1)
			unNorm_P_C_given_notA_B ++;
		if (recordA[i] == 0 && recordB[i] == 0)
			unNorm_P_C_given_notA_notB ++;
        }

	
	float P_C_given_A_B 		= unNorm_P_C_given_A_B / (P_B_given_A * numberOfRecords);
	float P_C_given_A_notB 		= unNorm_P_C_given_A_notB / ((1-P_B_given_A) * numberOfRecords);
	float P_C_given_notA_B 		= unNorm_P_C_given_notA_B / (P_B_given_notA * numberOfRecords);
	float P_C_given_notA_notB 	= unNorm_P_C_given_notA_notB / ((1-P_B_given_A) * numberOfRecords);

        cout<<"Parameters for C: "<<endl;
        cout<<" - "<< stateNamesC[1] <<" given A and B are "<< stateNamesA[1] <<" "<< stateNamesB[1] << " : "<< P_C_given_A_B <<endl;
	cout<<" - "<< stateNamesC[1] <<" given A and B are "<< stateNamesA[1] <<" "<< stateNamesB[0] << " : "<< P_C_given_A_notB <<endl;
	cout<<" - "<< stateNamesC[1] <<" given A and B are "<< stateNamesA[0] <<" "<< stateNamesB[1] << " : "<< P_C_given_notA_B <<endl;
	cout<<" - "<< stateNamesC[1] <<" given A and B are "<< stateNamesA[0] <<" "<< stateNamesB[0] << " : "<< P_C_given_notA_notB <<endl;
	cout<<endl;

}
