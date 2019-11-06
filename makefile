

version=1.1.0
ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif


CPPFLAGS=-pthread -std=c++11 -Wall

ifeq ($(uname_S), Windows)
	# set Windows compiler, flags, and paths
	tar="/cygdrive/c/cygwin64/bin/tar.exe"
	OS=win64
endif
ifeq ($(uname_S), Linux)
	# set Linux compiler, flags, and paths
	CXX=clang++
	WNO=-Wno-sign-compare -Wno-deprecated-register
	LIBFLAGS=-shared -fPIC
	CPPFLAGS+=-fPIC
	FINAL=-ldl

	OS=linux
	DLLEXT=so
#	PY_INC=-I/usr/include/python2.7
#	PY3_INC=-I/usr/include/python3.6m

	PY_INC=-Idependencies/python2.7
	PY3_INC=-Idependencies/python3.6

	PY_EXE=/usr/bin/python
	PY3_EXE=/usr/bin/python3

	JAVA_INC=-Idependencies/java1.8.0_45/

	tar=tar
endif
ifeq ($(uname_S), Darwin)
	# set Mac compiler, flags, and paths
	CXX=clang++
	WNO=-Wno-deprecated-register
	LIBFLAGS=-shared -undefined dynamic_lookup
	FINAL=

	OS=mac
	DLLEXT=dylib
	PY_EXE=/usr/bin/python
	PY3_EXE=/Library/Frameworks/Python.framework/Versions/3.6/bin/python3
	
	PY_INC=-Idependencies/python2.7/
	PY3_INC=-Idependencies/python3.6/

	JAVA_INC=-Idependencies/java1.8.0_45/
	tar=/usr/local/bin/tar
endif


INC=-Iinclude/

SRCS=src/ProgressBar.cc src/SequenceRecords.cc src/SequenceRecord.cc src/IGBlastParser.cc \
	 src/ErrorPredictor.cc src/SequenceFeatures.cc \
	 src/ErrorXOptions.cc src/util.cc \
	 src/SequenceQuery.cc src/errorx.cc src/AbSequence.cc src/ClonotypeGroup.cc \
	 src/main.cc src/testing.cc src/errorx_java.cc

SRCS+=src/keras/DataChunkFlat.cc src/keras/LayerDense.cc \
		   src/keras/KerasModel.cc src/keras/LayerActivation.cc 


OBJ=obj/ProgressBar.o obj/SequenceRecords.o obj/SequenceRecord.o obj/IGBlastParser.o \
	 obj/ErrorPredictor.o obj/SequenceFeatures.o \
	 obj/ErrorXOptions.o obj/util.o \
	 obj/SequenceQuery.o obj/errorx.o obj/AbSequence.o obj/ClonotypeGroup.o

OBJ+=obj/keras/DataChunkFlat.o obj/keras/LayerDense.o \
		   obj/keras/KerasModel.o obj/keras/LayerActivation.o 



BOOST=dependencies/boost_1_68_0/$(OS)/libboost_filesystem.a \
	  dependencies/boost_1_68_0/$(OS)/libboost_program_options.a \
	  dependencies/boost_1_68_0/$(OS)/libboost_system.a


all: binary library

libraries: library python java

debug: CPPFLAGS += -g
debug: library

debug_bin: CPPFLAGS += -g
debug_bin: binary

obj/%.o: src/%.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) -c -Ofast -o "$@" "$^"
# $@ - refers to the obj/*.o name
# $< - refers to the src name

# objects: $(SRCS)
# 	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(PY_INC) $(PY3_INC) $(JAVA_INC) -c -Ofast $(SRCS) $(FINAL)
# 	mv *o obj/

obj/errorx_python2.o: src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(PY_INC) -o obj/errorx_python2.o -c src/errorx_python.cc -Ofast

obj/errorx_python3.o: src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(PY3_INC) -o obj/errorx_python3.o -c src/errorx_python.cc -Ofast

obj/errorx_java.o: src/errorx_java.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(JAVA_INC) -o obj/errorx_java.o -c src/errorx_java.cc -Ofast

binary: $(OBJ) obj/main.o
	$(CXX) $(CPPFLAGS) obj/main.o $(OBJ) $(BOOST) -o bin/errorx $(FINAL)


binary_testing: $(OBJ) obj/testing.o
	$(CXX) $(CPPFLAGS) -o bin/errorx_testing $(OBJ) obj/testing.o $(BOOST) $(FINAL)


library: $(OBJ)
	$(CXX) $(CPPFLAGS) $(LIBFLAGS) -o lib/liberrorx.$(DLLEXT) $(OBJ) $(BOOST) $(FINAL)


python:  $(OBJ) obj/errorx_python2.o
	$(CXX) $(CPPFLAGS) $(LIBFLAGS) -o python2_bindings/errorx/errorx_lib.so $(OBJ) obj/errorx_python2.o $(BOOST) $(FINAL)


python3: $(OBJ) obj/errorx_python3.o
	$(CXX) $(CPPFLAGS) $(LIBFLAGS) -o python3_bindings/errorx/errorx_lib.so $(OBJ) obj/errorx_python3.o $(BOOST) $(FINAL)


java: $(OBJ) obj/errorx_java.o
	$(CXX) $(CPPFLAGS) $(LIBFLAGS) -o java_bindings/errorx/liberrorx.$(DLLEXT) $(OBJ) obj/errorx_java.o $(BOOST) $(FINAL)
	cd java_bindings; make


python_install: $(OBJ) obj/errorx_python2.o python
	$(PY_EXE) -m pip install --user -I python2_bindings/

python2_install: python_install
	
python3_install: $(OBJ) obj/errorx_python3.o python3
	$(PY3_EXE) -m pip install --user -I python3_bindings/



package: binary library python python3 java
	$(tar) cfz ErrorX-$(version)_$(OS).tar.gz model.nnet bin/errorx bin/igblastn_* build_test/new/ build_test/test_binary.sh build_test/TestErrorX.java build_test/input_files/ build_test/old/ build_test/test_python_bindings.py build_test/TestLinking.cc build_test/makefile build_test/run_build_test.sh database/ documentation/ErrorX_out.tsv documentation/ErrorX_user_guide.docx documentation/ErrorX_user_guide.pdf documentation/ExampleApp.cc documentation/ExampleApp.java documentation/ExampleApp.py documentation/ExampleSequences.fastq documentation/ExampleSequences.tsv include/ internal_data/ lib/ optional_file/ python2_bindings/ python3_bindings/ java_bindings/ --transform "s/^/ErrorX\//"

clean: 
	rm -rf obj/*o obj/keras/*o bin/errorx* lib/* python*_bindings/errorx/errorx_lib.so java_bindings/errorx/liberrorx.$(DLLEXT)

uninstall: clean
	$(PY_EXE) -m pip uninstall errorx
	$(PY3_EXE) -m pip uninstall errorx	
