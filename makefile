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
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register

	PYTHON_INC=-I/usr/include/python$(PYTHON_VERSION)
	PYTHON_LINK=-L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home/include/
	# JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register
		
	PYTHON_INC=-I/usr/include/python$(PYTHON_VERSION)
	PYTHON_LINK=-L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

	# PYTHON_INC=-I/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/
	# PYTHON_LINK=-L/Library/Frameworks/Python.framework/Versions/2.7/lib/ -lpython2.7

	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home/include/
	# JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif


INC=-Iinclude/ 

SRCS=src/SequenceRecords.cc src/SequenceRecord.cc src/IGBlastParser.cc \
	 src/ErrorPredictor.cc src/SequenceFeatures.cc src/DataScaler.cc \
	 src/ErrorXOptions.cc src/keras_model.cc  \
	 src/util.cc src/model.cc src/SequenceQuery.cc \
	 src/errorx.cc src/errorx_python.cc src/errorx_java.cc

BOOST_LIBS=dependencies/boost/mac/libboost_filesystem.a \
		   dependencies/boost/mac/libboost_program_options.a \
		   dependencies/boost/mac/libboost_system.a \
		   dependencies/boost/mac/libboost_python27.a


all: binary_testing library binary python java

libraries: library python java

binary_testing: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -o bin/errorx_testing $(SRCS) src/testing.cc

library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) $(PYTHON_INC) $(JAVA_INC) $(PYTHON_LINK) -shared -undefined dynamic_lookup -o lib/liberrorx.dylib $(SRCS) 

binary: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -o bin/errorx $(SRCS) src/main.cc

python: library
	cp lib/liberrorx.dylib python_bindings/mac/errorx/errorx_lib.so
	pip install python_bindings/mac

java: library
	cp lib/liberrorx.dylib java_bindings/mac/errorx/

debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -g $(BOOST_LIBS) -o bin/errorx_debug $(SRCS) src/main.cc
	
clean: 
	rm -rf bin/errorx bin/errorx_debug* bin/errorx_static bin/errorx_testing lib/* 
	
