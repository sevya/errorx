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

#	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_45.jdk/Contents/Home/include/
	JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/include/
	JAVA_INC=-I$(JAVA_HOME) -I$(JAVA_HOME)darwin/
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register
		
	# PYTHON_INC=-I/usr/include/python$(PYTHON_VERSION)
	# PYTHON_LINK=-L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

	PYTHON_INC=-I/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/
	PYTHON_LINK=-L/Library/Frameworks/Python.framework/Versions/2.7/lib/ -lpython2.7
	# PYTHON_INC=-I/System/Library/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/include/python$(PYTHON_VERSION)
	# PYTHON_LINK=-L/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION)

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

BOOST_LIBS=dependencies/boost_1_68_0/stage/lib/libboost_filesystem.a \
		   dependencies/boost_1_68_0/stage/lib/libboost_program_options.a \
		   dependencies/boost_1_68_0/stage/lib/libboost_system.a


all: binary_testing library binary python java

libraries: library python java

binary_testing: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -o bin/errorx_testing $(SRCS) src/testing.cc

library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -shared -undefined dynamic_lookup -o lib/liberrorx.dylib $(SRCS)

binary: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) -o bin/errorx $(SRCS) src/main.cc


python: $(SRCS) src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(INC) $(PYTHON_INC) -Ofast -fPIC -c $(SRCS) src/errorx_python.cc
	mv *.o lib/
	$(CXX) $(CPPFLAGS) -shared $(OBJ) $(PYTHON_LINK) -undefined dynamic_lookup -o lib/errorx_lib.so $(BOOST_LIBS) dependencies/boost_1_68_0/stage/lib/libboost_python27.a
	rm $(OBJ)
	cp lib/errorx_lib.so python_bindings/production/errorx
	pip install python_bindings/production

java: $(SRCS) src/errorx_java.cc
	$(CXX) $(CPPFLAGS) $(INC) -Ofast $(BOOST_LIBS) $(JAVA_INC) -shared -undefined dynamic_lookup -o lib/liberrorx_java.dylib $(SRCS) src/errorx_java.cc
	cp lib/liberrorx_java.dylib java_bindings/errorx/


debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(INC) -g $(BOOST_LIBS) -o bin/errorx_debug $(SRCS) src/main.cc
	
clean: 
	rm -rf bin/errorx bin/errorx_debug* bin/errorx_static bin/errorx_testing lib/* 
	
