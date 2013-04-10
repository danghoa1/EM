#include <iostream>
#include "smile/smile.h"
#include "smile/smilearn.h"

using namespace std;

int main()
{
	
	DSL_dataset d;
    
    if (d.ReadFile("simulatedData/cancerTest.dat") != DSL_OKAY) {
        cout << "Cannot read data file."<<endl;
        exit(1);
    }
	
    DSL_network result;
    DSL_naiveBayes naive;
    naive.classVariableId = "cancer";
    if (naive.Learn(d,result)!=DSL_OKAY)
    {
        cout << "Learning failed." <<endl;
        return 1;
    }

    result.WriteFile("naivebayes.xdsl");
    return 0;
}
