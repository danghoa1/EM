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

	DSL_em em;
  	if (em.Learn(ds, net, matches) != DSL_OKAY) {
     	cout << "Cannot learn parameters... exiting." << endl;
     	exit(1);
  	}
  	net.WriteFile("output/em_learned_parameters.net", DSL_HUGIN_FORMAT);
}
