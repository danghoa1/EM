#include "inference_engine.h"
#include "IL2.hxx"
#include "jni.h"

using namespace inflib;

// CONSTRUCTOR **********************

InferenceEngine::InferenceEngine(char* filepath)
{
	jvm = create_vm();
	jEngine = new IL2(jvm);
	jEngine->loadBayesianNetwork(filepath);
	jEngine->startEngine();
}

// DESTRUCTOR ********************

InferenceEngine::~InferenceEngine()
{
	if (jEngine != NULL) delete jEngine;
	// if (jvm != NULL) delete jvm;
	jvm->DestroyJavaVM();
}

// MAIN MEMBER FUNCTIONS *****************

void InferenceEngine::updateCPTs(int Nnodes, double** newcpts, int* Ncpt)
{
	for (int i=0; i<Nnodes;i++)
	{
		jEngine->updateCPTTable(i,newcpts[i],Ncpt[i]);
	}

}

void InferenceEngine::loadEvidence(int** dataset,int datasetLength,  int Nnodes)
{
	jEngine->loadEvidence(dataset,datasetLength,Nnodes);	
}

void InferenceEngine::updateEvidence(int index)
{
	jEngine->updateEvidence(index);
}

double* InferenceEngine::tableConditional(int x)
{
	int len;
	return jEngine->tableConditional(x,&len);
}

double InferenceEngine::probability(int x, int u)
{
	return jEngine->probability(x,u);
}

// MISC ***************************

JavaVM*  InferenceEngine::create_vm() {
	JavaVM* jvm; 
	JNIEnv* env;

    	JavaVMInitArgs args;
        JavaVMOption options[1];

	/* There is a new JNI_VERSION_1_4, but it doesn't add anything for the purposes of our example. */
	args.version = JNI_VERSION_1_4;

	args.nOptions = 1;
	options[0].optionString = "-Djava.class.path=.:libs/inflib.jar";
	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;
	JNI_CreateJavaVM(&jvm, (void **)&env, &args);

	return jvm;
}

