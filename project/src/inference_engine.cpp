#include "inference_engine.h"
#include "JInferenceEngine.hxx"
#include "jni.h"

InferenceEngine(int Nnodes, int* cardinality, int* Nparents, int** parents, int* Ncpt, double** cpt, char* filename) {}
{
	jvm = create_jvm();
	jEngine = new JInferenceEngine(jvm);
	engine->loadBayesianNetwork(filename);
}

InferenceEngine::~InferenceEngine()
{
	delete jEngine;
}

JavaVM*  InferenceEngine::create_vm() {
	JavaVM* jvm; 
	JNIEnv* env;

    	JavaVMInitArgs args;
        JavaVMOption options[2];

	/* There is a new JNI_VERSION_1_4, but it doesn't add anything for the purposes of our example. */
	args.version = JNI_VERSION_1_4;

	args.nOptions = 2;
	options[0].optionString = "-Djava.class.path=.";
	options[1].optionString = "-Xcheck:jni";
	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;
	JNI_CreateJavaVM(&jvm, (void **)&env, &args);

	return jvm;
}

void InferenceEngine::updateEvidence(int* evidence)
{
	
}

double InferenceEngine::probability(int x, int u)
{
	return jEngine->probability(x,u);
}






