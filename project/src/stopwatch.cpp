#include <iostream>
#include "stopwatch.h"
#include "time.h"

using namespace std;

Stopwatch::Stopwatch()
{
}

void Stopwatch::Start()
{
	start = clock();
}

void Stopwatch::End()
{
	end = clock();
}

void Stopwatch::Print(string message)
{
	cout<<message<<" "<<((double)(end-start))*1000/CLOCKS_PER_SEC<<" ms"<<endl;
}

