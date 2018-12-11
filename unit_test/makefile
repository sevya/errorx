.DEFAULT_GOAL := all

CXX=clang++
CPPFLAGS=-pthread -std=c++11 -Wall # -Werror
INC=-I../include/ -Icxxtest/

SRCS=../src/SequenceRecords.cc ../src/SequenceRecord.cc ../src/IGBlastParser.cc \
	 ../src/ErrorPredictor.cc ../src/SequenceFeatures.cc ../src/DataScaler.cc \
	 ../src/ErrorXOptions.cc ../src/keras_model.cc ../src/errorx.cc \
	 ../src/util.cc ../src/model.cc ../src/SequenceQuery.cc

BOOST_LIBS=../dependencies/boost_1_68_0/stage/lib/libboost_filesystem.a \
		   ../dependencies/boost_1_68_0/stage/lib/libboost_program_options.a \
		   ../dependencies/boost_1_68_0/stage/lib/libboost_system.a

CXXGEN=cxxtest/bin/cxxtestgen

all: TestSequenceFeatures TestIGBlastParser TestUtil TestErrorX

TestSequenceFeatures: $(SRCS)
	$(CXXGEN) --error-printer -o TestSequenceFeatures.cc TestSequenceFeatures.hh
	$(CXX) $(CPPFLAGS) $(INC) $(BOOST_LIBS) -o bin/TestSequenceFeatures $(SRCS) TestSequenceFeatures.cc
	
TestIGBlastParser: $(SRCS)
	$(CXXGEN) --error-printer -o TestIGBlastParser.cc TestIGBlastParser.hh
	$(CXX) $(CPPFLAGS) $(INC) $(BOOST_LIBS) -o bin/TestIGBlastParser $(SRCS) TestIGBlastParser.cc

TestErrorX: $(SRCS)
	$(CXXGEN) --error-printer -o TestErrorX.cc TestErrorX.hh
	$(CXX) $(CPPFLAGS) $(INC) $(BOOST_LIBS) -o bin/TestErrorX $(SRCS) TestErrorX.cc

TestUtil: ../src/util.cc
	$(CXXGEN) --error-printer -o TestUtil.cc TestUtil.hh
	$(CXX) $(CPPFLAGS) $(INC) $(BOOST_LIBS) -o bin/TestUtil ../src/util.cc TestUtil.cc	

TestLinking: 
	$(CXXGEN) --error-printer -o TestLinking.cc TestLinking.hh
	$(CXX) -std=c++11 $(INC) -L../lib/ -lerrorx -o bin/TestLinking TestLinking.cc

TestJava: TestErrorX.java 
	javac -cp '.:../java_bindings/ErrorX.jar:hamcrest-core-1.3.jar:junit-4.12.jar' TestErrorX.java
	java -cp .:../java_bindings/ErrorX.jar:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore TestErrorX

clean: 
	rm -rf bin/* 
	
