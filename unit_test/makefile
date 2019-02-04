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
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-sign-compare
	FINAL=-ldl
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	CPPFLAGS=-pthread -std=c++11 -Wall -Wno-deprecated-register
	FINAL=
endif

INC=-I../include/ -Icxxtest/

SRCS=../src/SequenceRecords.cc ../src/SequenceRecord.cc ../src/IGBlastParser.cc \
	 ../src/ErrorPredictor.cc ../src/SequenceFeatures.cc \
	 ../src/ErrorXOptions.cc ../src/keras_model.cc ../src/errorx.cc \
	 ../src/util.cc ../src/SequenceQuery.cc

BOOST=../dependencies/boost/mac/libboost_filesystem.a \
	  ../dependencies/boost/mac/libboost_program_options.a \
	  ../dependencies/boost/mac/libboost_system.a
		   
CXXGEN=cxxtest/bin/cxxtestgen

all: TestSequenceFeatures TestIGBlastParser TestUtil TestErrorX TestLinking

CopyLibrary: 
	cp ../lib/liberrorx.dylib lib/

TestSequenceFeatures: CopyLibrary
	$(CXXGEN) --error-printer -o TestSequenceFeatures.cc TestSequenceFeatures.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestSequenceFeatures TestSequenceFeatures.cc $(BOOST) $(FINAL)
	
TestIGBlastParser: CopyLibrary
	$(CXXGEN) --error-printer -o TestIGBlastParser.cc TestIGBlastParser.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestIGBlastParser TestIGBlastParser.cc $(BOOST) $(FINAL)

TestErrorX: CopyLibrary
	$(CXXGEN) --error-printer -o TestErrorX.cc TestErrorX.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestErrorX TestErrorX.cc $(BOOST) $(FINAL)

TestUtil: CopyLibrary
	$(CXXGEN) --error-printer -o TestUtil.cc TestUtil.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestUtil TestUtil.cc $(BOOST) $(FINAL)

TestLinking: CopyLibrary
	$(CXXGEN) --error-printer -o TestLinking.cc TestLinking.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestLinking TestLinking.cc $(FINAL)

TestJava: TestErrorX.java 
	javac -cp '.:../java_bindings/ErrorX.jar:hamcrest-core-1.3.jar:junit-4.12.jar' TestErrorX.java
	java -cp .:../java_bindings/ErrorX.jar:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore TestErrorX

clean: 
	rm -rf bin/* 
	
