
SRC_DIR = src
LIB_DIR = lib
CP   	= ${LIB_DIR}:${LIB_DIR}/inflib.jar
OPTS 	= -Xlint:unchecked

all:
	javac -d ${LIB_DIR} -sourcepath ${SRC_DIR} -cp ${CP} ${OPTS} ${SRC_DIR}/*.java
