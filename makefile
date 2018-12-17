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
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-sign-compare -Wno-deprecated-register
	LIBFLAGS=-shared -fPIC
	FINAL=-ldl

	OS=linux
	DLLEXT=so
	PY_INC=-I/usr/include/python$(PY_VERSION)
	PY_LINK=-L/usr/lib/python$(PY_VERSION)/config -lpython$(PY_VERSION)
	PY_EXE=/usr/bin/python

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
	PY_EXE=/usr/bin/python
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

debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -g -o bin/errorx_debug $(SRCS) src/main.cc $(BOOST) $(FINAL)

library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(LIBFLAGS) -o lib/liberrorx.$(DLLEXT) $(SRCS) $(BOOST) $(FINAL)

python: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) $(PY_INC) -Ofast $(LIBFLAGS) -o python_bindings/errorx/errorx_lib.so $(SRCS) src/errorx_python.cc $(BOOST) $(FINAL)
	sudo $(PY_EXE) -m pip install -I python_bindings/

python_package: python
	tar cfz ErrorX-1.0_linux_python2.7.tar.gz python_bindings/ --transform "s/python_bindings/ErrorX-1.0_$(OS)_python2.7/"

java: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) $(JAVA_INC) -Ofast $(LIBFLAGS) -o java_bindings/errorx/liberrorx.$(DLLEXT) $(SRCS) src/errorx_java.cc $(BOOST) $(FINAL)
	cd java_bindings; make

java_package: java
	tar cvfz ErrorX-1.0_linux_java.tar.gz java_bindings/bin java_bindings/database java_bindings/errorx/liberrorx* java_bindings/ErrorX.jar java_bindings/internal_data java_bindings/optional_file --transform "s/java_bindings/ErrorX-1.0_linux_java/"

clean: 
	rm -rf bin/errorx* lib/* python_bindings/$(OS)/errorx/errorx_lib.so java_bindings/errorx/liberrorx.$(DLLEXT)
	
