.DEFAULT_GOAL := all


ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	LIB=errorx/liberrorx.dll
endif
ifeq ($(uname_S), Linux)
	LIB=errorx/liberrorx.so
endif
ifeq ($(uname_S), Darwin)
	LIB=errorx/liberrorx.dylib
endif


SRCS=errorx/NativeUtils.java errorx/ErrorX.java errorx/ErrorXOptions.java
CLASS=errorx/NativeUtils.class errorx/ErrorX.class errorx/ErrorXOptions.class


all: class jar

class: $(SRCS)
	javac $(SRCS)

jar: 
	jar cf ErrorX.jar $(CLASS) $(LIB)


clean: 
	rm -rf ErrorX.jar $(CLASS)
	
