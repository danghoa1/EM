#include "MyComplexClass.hxx"
#include "Factorial.hxx"
#include "jni.h"


JavaVM* create_vm() {
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

using namespace basic_example;
using namespace std;

int main()
{
	JavaVM* jvm = create_vm();

	MyComplexClass *testOfMyClass = new MyComplexClass(jvm);
	cout << "My Computation : "  << testOfMyClass->myVeryComplexComputation(1.2,80) << endl;

	Factorial *factorialClass = new Factorial(jvm);
	cout << "Factorial of 5 : " << factorialClass->calculate(5) << endl;

	return 0;   
}


