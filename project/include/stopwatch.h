#ifndef STOPWATCH
#define STOPWATCH

#include <string>
#include <time.h>

using namespace std;

class Stopwatch
{
public:
	Stopwatch();		// Constructor
	void Start();		// Start stopwatch
	void End();		// End stopwatch
	void Print(string message);	// Print elapse time with message

private:
	clock_t start;
	clock_t end;	
};

#endif
