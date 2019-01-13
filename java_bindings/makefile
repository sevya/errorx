.DEFAULT_GOAL := all


JAVAC=javac
JAR=jar
CP='.:../java_bindings/ErrorX.jar'

ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	LIB=errorx/liberrorx.dll
	JAVAC="C:/Program Files/Java/jdk1.8.0_45/bin/javac.exe"
	JAR="C:/Program Files/Java/jdk1.8.0_45/bin/jar.exe"
	CP='.;../java_bindings/ErrorX.jar'
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
	$(JAVAC) $(SRCS)

jar: 
	$(JAR) cf ErrorX.jar $(CLASS) $(LIB)


clean: 
	rm -rf ErrorX.jar $(CLASS)
	
