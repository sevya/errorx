.DEFAULT_GOAL := all

ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	JAVAC="C:/Program Files/Java/jdk1.8.0_45/bin/javac.exe"
	JAVA="C:/Program Files/Java/jdk1.8.0_45/bin/java.exe"
	# set Windows compiler, flags, and paths
	CP='.;../java_bindings/ErrorX.jar;hamcrest-core-1.3.jar;junit-4.12.jar'
endif

CXX=clang++
# set up blank CPPFLAGS
CPPFLAGS=
#enables us to add cppflags on command line
override CPPFLAGS+=-pthread -std=c++11 -Wall

JAVAC=javac
JAVA=java
CP='.:../java_bindings/ErrorX.jar:hamcrest-core-1.3.jar:junit-4.12.jar'

ifeq ($(uname_S), Linux)
	override CPPFLAGS+=-Wno-sign-compare
	FINAL=-ldl
	DLLEXT=so
endif
ifeq ($(uname_S), Darwin)
	override CPPFLAGS+=-Wno-deprecated-register
	FINAL=
	DLLEXT=dylib
endif

INC=-I../include/ -Icxxtest/

CXXGEN=cxxtest/bin/cxxtestgen

all: TestSequenceFeatures TestSequenceRecords TestIGBlastParser TestUtil TestErrorX TestErrorXOptions TestKerasModel TestLinking

CopyLibrary: 
	cp ../lib/liberrorx.$(DLLEXT) lib/

TestSequenceFeatures: CopyLibrary
	$(CXXGEN) --error-printer -o TestSequenceFeatures.cc TestSequenceFeatures.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestSequenceFeatures TestSequenceFeatures.cc $(FINAL)
	
TestIGBlastParser: CopyLibrary
	$(CXXGEN) --error-printer -o TestIGBlastParser.cc TestIGBlastParser.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestIGBlastParser TestIGBlastParser.cc $(FINAL)

TestErrorX: CopyLibrary
	$(CXXGEN) --error-printer -o TestErrorX.cc TestErrorX.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestErrorX TestErrorX.cc $(FINAL)

TestErrorXOptions: CopyLibrary
	$(CXXGEN) --error-printer -o TestErrorXOptions.cc TestErrorXOptions.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestErrorXOptions TestErrorXOptions.cc $(FINAL)

TestUtil: CopyLibrary
	$(CXXGEN) --error-printer -o TestUtil.cc TestUtil.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestUtil TestUtil.cc $(FINAL)

TestSequenceRecords: CopyLibrary
	$(CXXGEN) --error-printer -o TestSequenceRecords.cc TestSequenceRecords.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestSequenceRecords TestSequenceRecords.cc $(FINAL)

TestKerasModel: CopyLibrary
	$(CXXGEN) --error-printer -o TestKerasModel.cc TestKerasModel.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestKerasModel TestKerasModel.cc $(FINAL)

TestLinking: CopyLibrary
	$(CXXGEN) --error-printer -o TestLinking.cc TestLinking.hh
	$(CXX) $(CPPFLAGS) $(INC) -Llib/ -lerrorx -o bin/TestLinking TestLinking.cc $(FINAL)

TestJava: TestErrorX.java 
	$(JAVAC) -cp $(CP) TestErrorX.java
	$(JAVA) -cp $(CP) org.junit.runner.JUnitCore TestErrorX

clean: 
	rm -rf bin/* 
	
