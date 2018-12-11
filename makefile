.DEFAULT_GOAL := binary

PYTHON_VERSION=2.7

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
	CXX=g++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-sign-compare
	LIBFLAGS=-shared -fPIC -static
	FINAL=-ldl

	OS=linux
	DLLEXT=so
	PYTHON_INC=-I/usr/include/python$(PYTHON_VERSION)
	PYTHON_LINK=-L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register
	LIBFLAGS=-shared -undefined dynamic_lookup
	FINAL=""		
	OS=mac
	DLLEXT=dylib
	# PYTHON_INC=-I/usr/include/python$(PYTHON_VERSION)
	# PYTHON_LINK=-L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

	PYTHON_INC=-I/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/
	PYTHON_LINK=-L/Library/Frameworks/Python.framework/Versions/2.7/lib/ -lpython2.7

#	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home/include/
	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif


INC=-Iinclude/

SRCS=src/SequenceRecords.cc src/SequenceRecord.cc src/IGBlastParser.cc \
	 src/ErrorPredictor.cc src/SequenceFeatures.cc src/DataScaler.cc \
	 src/ErrorXOptions.cc src/keras_model.cc src/errorx.cc \
	 src/util.cc src/model.cc src/SequenceQuery.cc


OBJ=lib/SequenceRecords.o lib/SequenceRecord.o lib/IGBlastParser.o \
	 lib/ErrorPredictor.o lib/SequenceFeatures.o lib/errorx.o \
	 lib/DataScaler.o lib/ErrorXOptions.o lib/keras_model.o \
	 lib/util.o lib/model.o lib/SequenceQuery.o lib/errorx_python.o

BOOST_LIBS=dependencies/boost/$(OS)/libboost_filesystem.a \
		   dependencies/boost/$(OS)/libboost_program_options.a \
		   dependencies/boost/$(OS)/libboost_system.a


all: binary_testing library binary python java

libraries: library python java

binary_testing: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast -o bin/errorx_testing.$(OS) $(SRCS) src/testing.cc $(BOOST_LIBS) $(FINAL)

library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(LIBFLAGS) -o lib/liberrorx.$(DLLEXT) $(SRCS) $(BOOST_LIBS) $(FINAL)

binary: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -o bin/errorx.$(OS) $(SRCS) src/main.cc $(BOOST_LIBS) $(FINAL)


python: $(SRCS) src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(INC) $(PYTHON_INC) -Ofast -fPIC -c $(SRCS) src/errorx_python.cc $(BOOST_LIBS) $(FINAL)
	mv *.o lib/
	$(CXX) $(CPPFLAGS) -shared $(OBJ) $(PYTHON_LINK) -undefined dynamic_lookup -o lib/errorx_lib.so $(BOOST_LIBS) dependencies/boost/$(OS)/libboost_python27.a
	rm $(OBJ)
	cp lib/errorx_lib.so python_bindings/$(OS)/errorx
	pip install python_bindings/$(OS)

java: $(SRCS) src/errorx_java.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(JAVA_INC) -shared -undefined dynamic_lookup -o lib/liberrorx_java.dylib $(SRCS) src/errorx_java.cc $(BOOST_LIBS) $(FINAL)
	cp lib/liberrorx_java.dylib java_bindings/errorx/


debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -g -o bin/errorx_debug $(SRCS) src/main.cc $(BOOST_LIBS) $(FINAL)
	
clean: 
	rm -rf bin/errorx bin/errorx_debug* bin/errorx_static bin/errorx_testing lib/* 
	
