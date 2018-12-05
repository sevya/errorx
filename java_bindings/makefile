.DEFAULT_GOAL := all

SRCS=errorx/NativeUtils.java errorx/ErrorX.java errorx/ErrorXOptions.java
CLASS=errorx/NativeUtils.class errorx/ErrorX.class errorx/ErrorXOptions.class
LIB=errorx/liberrorx_java.dylib

all: class jar

class: $(SRCS)
	javac $(SRCS)

jar: $(CLASS)
	jar cf ErrorX.jar $(CLASS) $(LIB)


clean: 
	rm -rf ErrorX.jar $(CLASS)
	
