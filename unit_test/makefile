.DEFAULT_GOAL := all


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

CXX=clang++
CPPFLAGS=-pthread -std=c++11 -Wall # -Werror
INC=-I../include/ -Icxxtest/

SRCS=../src/SequenceRecords.cc ../src/SequenceRecord.cc ../src/IGBlastParser.cc \
	 ../src/ErrorPredictor.cc ../src/SequenceFeatures.cc ../src/DataScaler.cc \
	 ../src/ErrorXOptions.cc ../src/keras_model.cc ../src/errorx.cc \
	 ../src/util.cc ../src/model.cc ../src/SequenceQuery.cc

BOOST_LIBS=../dependencies/boost/linux/libboost_filesystem.a \
		   ../dependencies/boost/linux/libboost_program_options.a \
		   ../dependencies/boost/linux/libboost_system.a

CXXGEN=cxxtest/bin/cxxtestgen

all: TestSequenceFeatures TestIGBlastParser TestUtil TestErrorX TestLinking

TestSequenceFeatures: $(SRCS)
	$(CXXGEN) --error-printer -o TestSequenceFeatures.cc TestSequenceFeatures.hh
	$(CXX) $(CPPFLAGS) $(INC) -o bin/TestSequenceFeatures $(SRCS) TestSequenceFeatures.cc $(BOOST_LIBS) $(FINAL)
	
TestIGBlastParser: $(SRCS)
	$(CXXGEN) --error-printer -o TestIGBlastParser.cc TestIGBlastParser.hh
	$(CXX) $(CPPFLAGS) $(INC) -o bin/TestIGBlastParser $(SRCS) TestIGBlastParser.cc $(BOOST_LIBS) $(FINAL)

TestErrorX: $(SRCS)
	$(CXXGEN) --error-printer -o TestErrorX.cc TestErrorX.hh
	$(CXX) $(CPPFLAGS) $(INC) -o bin/TestErrorX $(SRCS) TestErrorX.cc $(BOOST_LIBS) $(FINAL)

TestUtil: ../src/util.cc
	$(CXXGEN) --error-printer -o TestUtil.cc TestUtil.hh
	$(CXX) $(CPPFLAGS) $(INC) -o bin/TestUtil ../src/util.cc TestUtil.cc $(BOOST_LIBS) $(FINAL)

TestLinking: 
	$(CXXGEN) --error-printer -o TestLinking.cc TestLinking.hh
	$(CXX) -std=c++11 $(INC) -L../lib/ -lerrorx -o bin/TestLinking TestLinking.cc

TestJava: TestErrorX.java 
	javac -cp '.:../java_bindings/ErrorX.jar:hamcrest-core-1.3.jar:junit-4.12.jar' TestErrorX.java
	java -cp .:../java_bindings/ErrorX.jar:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore TestErrorX

clean: 
	rm -rf bin/* 
	
