#include <iostream>
#include <string>
#include "bayesian_network.h"
#include "stopwatch.h"

using namespace std;

int main()
{
	cout<<endl;

	Stopwatch sw;

	// Read network
	sw.Start();
	BayesNetwork net;
	net.Read("network/network1.uai");
	sw.End();
	sw.Print("Read network:");

	// Simulate
	sw.Start();
	net.Simulate("output/simulate1.dat",2000000);
	sw.End();
	sw.Print("Simulate dataset:");	

	// Maximum Likelihood
	sw.Start();
	net.Learn("output/simulate1.dat");
	sw.End();
	sw.Print("Maximum Likelihood:");
	net.Print();
}
