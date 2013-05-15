#include <iostream>
#include "stopwatch.h"
#include "time.h"

using namespace std;

Stopwatch::Stopwatch()
{
	duration = 0;
	running = false;
}

void Stopwatch::on()
{
	if (running == false)
	{
		running = true;
		start = clock();
	}
}

void Stopwatch::off()
{
	if (running)
	{
		running = false;
		end = clock();
		duration += end - start;
	}
}

void Stopwatch::print(string message)
{
	off();
	cout<<message<<" "<<((double)duration)*1000/CLOCKS_PER_SEC<<" ms"<<endl;
}

