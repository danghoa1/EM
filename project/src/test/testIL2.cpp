#include "inference_engine.h"
#include "stopwatch.h"

using namespace std;

int main(int argc, char **argv)
{
	int loop = 3000;
	char* filename = "network/nets-uai/easy/alarm.uai";
	int c;
	opterr = 1;

	while ((c = getopt (argc, argv, "l:f:")) != -1)
	{
		switch (c)
		{
			case 'l':
				loop = atoi(optarg);
				break;
			case 'f':
				filename = optarg;
				break;
			default:
				abort();
		}
	}

	Stopwatch sw;
	sw.on();

	int dataset[37];
	
	InferenceEngine engine(filename);
	for (int i=1; i<= loop; i++)
	{
		for (int j=0; j < 37; j++)
			dataset[j] = j % 2;
	//	engine.updateEvidence(dataset,37);
	//	double prob = engine.probability(0,0);

	}

	sw.off();
	sw.print("Time:");
}
