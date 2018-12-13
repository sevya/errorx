.DEFAULT_GOAL := binary

PY_VERSION=2.7

ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	# set Windows compiler, flags, and paths
endif
ifeq ($(uname_S), Linux)
	# set Linux compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-sign-compare
	LIBFLAGS=-shared -fPIC
	FINAL=-ldl

	OS=linux
	DLLEXT=so
	PY_INC=-I/usr/include/python$(PY_VERSION)
	PY_LINK=-L/usr/lib/python$(PY_VERSION)/config -lpython$(PY_VERSION)

	JAVA_HOME=/home/sevya/jdk1.8.0_45/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)linux/
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register
	LIBFLAGS=-shared -undefined dynamic_lookup
	FINAL=

	OS=mac
	DLLEXT=dylib
	PY_INC=-I/System/Library/Frameworks/Python.framework/Versions/$(PY_VERSION)/include/python$(PY_VERSION)/
	PY_LINK=-I/System/Library/Frameworks/Python.framework/Versions/$(PY_VERSION)/lib/python$(PY_VERSION)/ -lpython$(PY_VERSION)

	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif


INC=-Iinclude/

SRCS=src/SequenceRecords.cc src/SequenceRecord.cc src/IGBlastParser.cc \
	 src/ErrorPredictor.cc src/SequenceFeatures.cc src/DataScaler.cc \
	 src/ErrorXOptions.cc src/keras_model.cc  \
	 src/util.cc src/model.cc src/SequenceQuery.cc \
	 src/errorx.cc

BOOST=dependencies/boost/mac/libboost_filesystem.a \
	  dependencies/boost/mac/libboost_program_options.a \
	  dependencies/boost/mac/libboost_system.a
		   


all: binary_testing library binary python java

libraries: library python java


binary_testing: $(SRCS) src/testing.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast -o bin/errorx_testing $(SRCS) src/testing.cc $(BOOST) $(FINAL)


binary: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast -o bin/errorx $(SRCS) src/main.cc $(BOOST) $(FINAL)


library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(PY_INC) $(JAVA_INC) $(PY_LINK) $(LIBFLAGS) -o lib/liberrorx.$(DLLEXT) $(SRCS) src/errorx_java.cc src/errorx_python.cc $(BOOST) $(FINAL)


python: library
	cp lib/liberrorx.$(DLLEXT) python_bindings/$(OS)/errorx/errorx_lib.so
	/usr/bin/python2.7 -m pip install python_bindings/$(OS)/


java: library
	cp lib/liberrorx.$(DLLEXT) java_bindings/errorx/
	cd java_bindings; make all


debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -g -o bin/errorx_debug $(SRCS) src/main.cc $(BOOST) $(FINAL)
	

clean: 
	rm -rf bin/errorx bin/errorx_debug* bin/errorx_static bin/errorx_testing lib/* 
	
