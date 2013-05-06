#include "IL2.hxx"
#include "jni.h"


JavaVM* create_vm() {
	JavaVM* jvm; 
	JNIEnv* env;

    	JavaVMInitArgs args;
        JavaVMOption options[2];

	/* There is a new JNI_VERSION_1_4, but it doesn't add anything for the purposes of our example. */
	args.version = JNI_VERSION_1_4;

	args.nOptions = 1;
	options[0].optionString = "-Djava.class.path=.:libs/inflib.jar";
	//options[1].optionString = "-Xcheck:jni";
	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;
	JNI_CreateJavaVM(&jvm, (void **)&env, &args);

	return jvm;
}

using namespace inflib;
using namespace std;

int main()
{
	JavaVM* jvm = create_vm();

	IL2 *engine = new IL2(jvm);
	engine->loadBayesianNetwork("network.uai");
	engine->startEngine();
	int dataset[1] = {0};
	engine->updateEvidence(dataset,1);
	for (int i=0; i<3; i++)
		for (int j=0;j<2; j++)
		cout << "Prob : " << engine->probability(i,j)<<endl;
	engine->print();
	return 0;   
}


