#include "IL2.hxx"
/* Generated by GIWS (version 2.0.1) with command:
giws -f src/misc/InferenceEngine.giws.xml 
*/
/*

This is generated code.

This software is a computer program whose purpose is to hide the complexity
of accessing Java objects/methods from C++ code.

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
*/

namespace inflib {

                // Static declarations (if any)
                
// Returns the current env

JNIEnv * IL2::getCurrentEnv() {
JNIEnv * curEnv = NULL;
jint res=this->jvm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
if (res != JNI_OK) {
std::cerr << "Could not retrieve the current JVM." << std::endl;
exit(EXIT_FAILURE);

}
return curEnv;
}
// Destructor

IL2::~IL2() {
JNIEnv * curEnv = NULL;
this->jvm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

curEnv->DeleteGlobalRef(this->instance);
curEnv->DeleteGlobalRef(this->instanceClass);
}
// Constructors
IL2::IL2(JavaVM * jvm_) {
jmethodID constructObject = NULL ;
jobject localInstance ;
jclass localClass ;

const std::string construct="<init>";
const std::string param="()V";
jvm=jvm_;

JNIEnv * curEnv = getCurrentEnv();

localClass = curEnv->FindClass( this->className().c_str() ) ;
if (localClass == NULL) {
std::cerr << "Could not get the Class " << this->className() <<  std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
}

this->instanceClass = static_cast<jclass>(curEnv->NewGlobalRef(localClass));

/* localClass is not needed anymore */
curEnv->DeleteLocalRef(localClass);

if (this->instanceClass == NULL) {
std::cerr << "Could not create a Global Ref of " << this->className() <<  std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
}


constructObject = curEnv->GetMethodID( this->instanceClass, construct.c_str() , param.c_str() ) ;
if(constructObject == NULL){
std::cerr << "Could not retrieve the constructor of the class " << this->className() << " with the profile : " << construct << param << std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
}

localInstance = curEnv->NewObject( this->instanceClass, constructObject ) ;
if(localInstance == NULL){
std::cerr << "Could not instantiate the object " << this->className() << " with the constructor : " << construct << param << std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
}
 
this->instance = curEnv->NewGlobalRef(localInstance) ;
if(this->instance == NULL){
std::cerr << "Could not create a new global ref of " << this->className() << std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
}
/* localInstance not needed anymore */
curEnv->DeleteLocalRef(localInstance);

                /* Methods ID set to NULL */
voidloadBayesianNetworkjstringjava_lang_StringID=NULL;
voidupdateCPTTablejintintjdoubleArray_doubledoubleID=NULL;
voidstartEngineID=NULL;
voidloadEvidencejobjectArray__intintID=NULL;
voidupdateEvidencejintintID=NULL;
jdoubleprobabilityjintintjintintID=NULL;
jdoubleArray_tableConditionaljintintID=NULL;
voidprintID=NULL;


}

IL2::IL2(JavaVM * jvm_, jobject JObj) {
        jvm=jvm_;

        JNIEnv * curEnv = getCurrentEnv();

jclass localClass = curEnv->GetObjectClass(JObj);
        this->instanceClass = static_cast<jclass>(curEnv->NewGlobalRef(localClass));
        curEnv->DeleteLocalRef(localClass);

        if (this->instanceClass == NULL) {

std::cerr << "Could not create a Global Ref of " << this->className() <<  std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
        }

        this->instance = curEnv->NewGlobalRef(JObj) ;
        if(this->instance == NULL){

std::cerr << "Could not create a new global ref of " << this->className() << std::endl;
curEnv->ExceptionDescribe();
exit(EXIT_FAILURE);
        }
        /* Methods ID set to NULL */
        voidloadBayesianNetworkjstringjava_lang_StringID=NULL;
voidupdateCPTTablejintintjdoubleArray_doubledoubleID=NULL;
voidstartEngineID=NULL;
voidloadEvidencejobjectArray__intintID=NULL;
voidupdateEvidencejintintID=NULL;
jdoubleprobabilityjintintjintintID=NULL;
jdoubleArray_tableConditionaljintintID=NULL;
voidprintID=NULL;


}

// Generic methods

void IL2::synchronize() {
if (getCurrentEnv()->MonitorEnter(instance) != JNI_OK) {
std::cerr << "Fail to enter monitor." << std::endl;
exit(EXIT_FAILURE);

}
}

void IL2::endSynchronize() {
if ( getCurrentEnv()->MonitorExit(instance) != JNI_OK) {

std::cerr << "Fail to exit monitor." << std::endl;
exit(EXIT_FAILURE);
}
}
// Method(s)

void IL2::loadBayesianNetwork (char const* filename){

JNIEnv * curEnv = getCurrentEnv();

if (voidloadBayesianNetworkjstringjava_lang_StringID==NULL) { /* Use the cache */
 voidloadBayesianNetworkjstringjava_lang_StringID = curEnv->GetMethodID(this->instanceClass, "loadBayesianNetwork", "(Ljava/lang/String;)V" ) ;
if (voidloadBayesianNetworkjstringjava_lang_StringID == NULL) {
std::cerr << "Could not access to the method " << "loadBayesianNetwork" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
jstring filename_ = curEnv->NewStringUTF( filename );
if (filename != NULL && filename_ == NULL)
{
std::cerr << "Could not convert C string to Java UTF string, memory full." << std::endl;
exit(EXIT_FAILURE);
}


                         curEnv->CallVoidMethod( this->instance, voidloadBayesianNetworkjstringjava_lang_StringID ,filename_);
                        curEnv->DeleteLocalRef(filename_);

}

void IL2::updateCPTTable (int node, double const* cpt, int cptSize){

JNIEnv * curEnv = getCurrentEnv();

if (voidupdateCPTTablejintintjdoubleArray_doubledoubleID==NULL) { /* Use the cache */
 voidupdateCPTTablejintintjdoubleArray_doubledoubleID = curEnv->GetMethodID(this->instanceClass, "updateCPTTable", "(I[D)V" ) ;
if (voidupdateCPTTablejintintjdoubleArray_doubledoubleID == NULL) {
std::cerr << "Could not access to the method " << "updateCPTTable" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
jdoubleArray cpt_ = curEnv->NewDoubleArray( cptSize ) ;

curEnv->SetDoubleArrayRegion( cpt_, 0, cptSize, (jdouble*)(cpt) ) ;


                         curEnv->CallVoidMethod( this->instance, voidupdateCPTTablejintintjdoubleArray_doubledoubleID ,node, cpt_);
                        curEnv->DeleteLocalRef(cpt_);

}

void IL2::startEngine (){

JNIEnv * curEnv = getCurrentEnv();

if (voidstartEngineID==NULL) { /* Use the cache */
 voidstartEngineID = curEnv->GetMethodID(this->instanceClass, "startEngine", "()V" ) ;
if (voidstartEngineID == NULL) {
std::cerr << "Could not access to the method " << "startEngine" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
                         curEnv->CallVoidMethod( this->instance, voidstartEngineID );
                        
}

void IL2::loadEvidence (int const* const* dataset, int datasetSize, int datasetSizeCol){

JNIEnv * curEnv = getCurrentEnv();

if (voidloadEvidencejobjectArray__intintID==NULL) { /* Use the cache */
 voidloadEvidencejobjectArray__intintID = curEnv->GetMethodID(this->instanceClass, "loadEvidence", "([[I)V" ) ;
if (voidloadEvidencejobjectArray__intintID == NULL) {
std::cerr << "Could not access to the method " << "loadEvidence" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
 jobjectArray dataset_ = curEnv->NewObjectArray(datasetSize, curEnv->FindClass("[I"),NULL);

 for (int i=0; i<datasetSize; i++){

jintArray datasetLocal = curEnv->NewIntArray( datasetSizeCol ) ;

curEnv->SetIntArrayRegion( datasetLocal, 0, datasetSizeCol, (jint*)(dataset[i]) ) ;
curEnv->SetObjectArrayElement(dataset_, i, datasetLocal);
curEnv->DeleteLocalRef(datasetLocal);
}

                         curEnv->CallVoidMethod( this->instance, voidloadEvidencejobjectArray__intintID ,dataset_);
                        curEnv->DeleteLocalRef(dataset_);

}

void IL2::updateEvidence (int index){

JNIEnv * curEnv = getCurrentEnv();

if (voidupdateEvidencejintintID==NULL) { /* Use the cache */
 voidupdateEvidencejintintID = curEnv->GetMethodID(this->instanceClass, "updateEvidence", "(I)V" ) ;
if (voidupdateEvidencejintintID == NULL) {
std::cerr << "Could not access to the method " << "updateEvidence" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
                         curEnv->CallVoidMethod( this->instance, voidupdateEvidencejintintID ,index);
                        
}

double IL2::probability (int node, int cptPos){

JNIEnv * curEnv = getCurrentEnv();

if (jdoubleprobabilityjintintjintintID==NULL) { /* Use the cache */
 jdoubleprobabilityjintintjintintID = curEnv->GetMethodID(this->instanceClass, "probability", "(II)D" ) ;
if (jdoubleprobabilityjintintjintintID == NULL) {
std::cerr << "Could not access to the method " << "probability" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
                        jdouble res =  static_cast<jdouble>( curEnv->CallDoubleMethod( this->instance, jdoubleprobabilityjintintjintintID ,node, cptPos));
                        
return res;

}

double* IL2::tableConditional (int node, int *lenRow){

JNIEnv * curEnv = getCurrentEnv();

if (jdoubleArray_tableConditionaljintintID==NULL) { /* Use the cache */
 jdoubleArray_tableConditionaljintintID = curEnv->GetMethodID(this->instanceClass, "tableConditional", "(I)[D" ) ;
if (jdoubleArray_tableConditionaljintintID == NULL) {
std::cerr << "Could not access to the method " << "tableConditional" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
                        jdoubleArray res =  static_cast<jdoubleArray>( curEnv->CallObjectMethod( this->instance, jdoubleArray_tableConditionaljintintID ,node));
                        if (res == NULL) { return NULL; }
                        if (curEnv->ExceptionCheck()) {
curEnv->ExceptionDescribe() ;
}

* lenRow = curEnv->GetArrayLength(res);
jboolean isCopy = JNI_FALSE;

/* GetPrimitiveArrayCritical is faster than getXXXArrayElements */
jdouble *resultsArray = static_cast<jdouble *>(curEnv->GetPrimitiveArrayCritical(res, &isCopy));
double* myArray= new double[* lenRow];

for (jsize i = 0; i < * lenRow; i++){
myArray[i]=resultsArray[i];
}
curEnv->ReleasePrimitiveArrayCritical(res, resultsArray, JNI_ABORT);

                        curEnv->DeleteLocalRef(res);

return myArray;

}

void IL2::print (){

JNIEnv * curEnv = getCurrentEnv();

if (voidprintID==NULL) { /* Use the cache */
 voidprintID = curEnv->GetMethodID(this->instanceClass, "print", "()V" ) ;
if (voidprintID == NULL) {
std::cerr << "Could not access to the method " << "print" << std::endl;
curEnv->ExceptionDescribe();

exit(EXIT_FAILURE);
}
}
                         curEnv->CallVoidMethod( this->instance, voidprintID );
                        
}

}
