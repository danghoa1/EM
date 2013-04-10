#include <iostream>
#include <string>
#include <time.h>
#include "bayesian_network.h"
#include "bayesian_network_with_timer.h"

using namespace std;

int main()
{
	cout<<endl;

	BayesNetworkWithTimer net;
	
	net.Read("network/network2.uai");
	net.Learn("dataset/dataset2_500000.dat");
	net.Print();
}
