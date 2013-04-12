#include <iostream>
#include <string>
#include <time.h>
#include "bayesian_network.h"

using namespace std;

int main()
{
	cout<<endl;

	BayesNetwork net;
	
	net.Read("network/network1.uai");
	
	//net.Simulate("output/simulate1.dat",500000);
	net.Learn("output/simulate1.dat");
	net.Print();
}
