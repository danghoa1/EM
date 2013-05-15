#ifndef STOPWATCH
#define STOPWATCH

#include <string>
#include <time.h>

using namespace std;

class Stopwatch
{
public:
	Stopwatch();		// Constructor
	void on();
	void off();
	void print(string message);	// Print elapse time with message

private:
	bool running;
	clock_t start;
	clock_t end;	
	clock_t duration;
};

#endif
