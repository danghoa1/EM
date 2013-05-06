#include "inference_engine.h"
#include "IL2.hxx"
#include "jni.h"

using namespace inflib;

// CONSTRUCTOR **********************

InferenceEngine::InferenceEngine(int Nnodes, int* cardinality, int* Nparents, int** parents, int* Ncpt, double** cpt, char* filepath)
{
	jvm = create_vm();
	jEngine = new IL2(jvm);
	jEngine->loadBayesianNetwork(filepath);
	jEngine->startEngine();

	m_Nnodes = Nnodes;
	m_cardinality = cardinality;
	m_Nparents = Nparents;
	m_Ncpt = Ncpt;
	m_cpt = cpt;
	m_filepath = filepath;

}

// DESTRUCTOR ********************

InferenceEngine::~InferenceEngine()
{
	delete jEngine;
}

// MAIN MEMBER FUNCTIONS *****************

void InferenceEngine::updateCPTs(double** newcpts)
{
	for (int i=0; i<m_Nnodes;i++)
	{
		jEngine->updateCPTTable(i,m_cpt[i],m_Ncpt[i]);
	}

}

void InferenceEngine::updateEvidence(int* evidence)
{
	jEngine->updateEvidence(evidence,m_Nnodes);	
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

