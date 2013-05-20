#include "inference_engine.h"
#include "stopwatch.h"

using namespace std;

int main()
{
	Stopwatch sw;
	sw.on();
	
	InferenceEngine engine("network/nets-uai/easy/alarm.uai");
	for (int i=1; i<= 3000; i++)
	{
		int dataset[1];
		dataset[1] = i % 2;
		engine.updateEvidence(dataset,1);

	}

	sw.off();
	sw.print("Time:");
}
