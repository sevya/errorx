.DEFAULT_GOAL := all


ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	LIB=win64/errorx/liberrorx.dll
endif
ifeq ($(uname_S), Linux)
	LIB=linux/errorx/liberrorx.so
endif
ifeq ($(uname_S), Darwin)
	LIB=mac/errorx/liberrorx.dylib
endif


SRCS=mac/errorx/NativeUtils.java mac/errorx/ErrorX.java mac/errorx/ErrorXOptions.java
CLASS=errorx/NativeUtils.class errorx/ErrorX.class errorx/ErrorXOptions.class


all: class jar

class: $(SRCS)
	javac $(SRCS)

jar: 
	cd mac/; jar cf ../ErrorX.jar $(CLASS) errorx/liberrorx.dylib


clean: 
	rm -rf ErrorX.jar $(CLASS)
	
