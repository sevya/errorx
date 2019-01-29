.DEFAULT_GOAL := binary

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
	WNO=-Wno-sign-compare -Wno-deprecated-register
	LIBFLAGS=-shared -fPIC
	FINAL=-ldl

	OS=linux
	DLLEXT=so
	PY_INC=-I/usr/include/python2.7
	PY3_INC=-I/usr/include/python3.6

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

CPPFLAGS=-pthread -std=c++11 -Wall

INC=-Iinclude/

SRCS=src/SequenceRecords.cc src/SequenceRecord.cc src/IGBlastParser.cc \
	 src/ErrorPredictor.cc src/SequenceFeatures.cc \
	 src/ErrorXOptions.cc src/keras_model.cc src/util.cc \
	 src/SequenceQuery.cc src/errorx.cc

BOOST=dependencies/boost_1_68_0/$(OS)/libboost_filesystem.a \
	  dependencies/boost_1_68_0/$(OS)/libboost_program_options.a \
	  dependencies/boost_1_68_0/$(OS)/libboost_system.a


all: binary binary_testing library python_install python3_install java

libraries: library python java


binary_testing: $(SRCS) src/testing.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) -Ofast -o bin/errorx_testing $(SRCS) src/testing.cc $(BOOST) $(FINAL)

binary: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) -Ofast -o bin/errorx $(SRCS) src/main.cc $(BOOST) $(FINAL)

debug: $(SRCS) src/main.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) -g -o bin/errorx_debug $(SRCS) src/main.cc $(BOOST) $(FINAL)

library: $(SRCS)
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) -Ofast $(LIBFLAGS) -o lib/liberrorx.$(DLLEXT) $(SRCS) $(BOOST) $(FINAL)

package: binary library python python3 java
	$(tar) cfz ErrorX-1.0_$(OS).tar.gz bin/errorx bin/igblastn_* build_test/ database/ documentation/ include/ internal_data/ lib/ optional_file/ test_case/ python2_bindings/ python3_bindings/ java_bindings/ --transform "s/^/ErrorX\//"

python: $(SRCS) src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(PY_INC) -Ofast $(LIBFLAGS) -o python2_bindings/errorx/errorx_lib.so $(SRCS) src/errorx_python.cc $(BOOST) $(FINAL)
	
python_install: python
	sudo $(PY_EXE) -m pip install -I python2_bindings/

python_package: python_install
	$(tar) cfz ErrorX-1.0_$(OS)_python2.7.tar.gz python2_bindings/ --transform "s/python2_bindings/ErrorX_python2.7/"

python3: $(SRCS) src/errorx_python.cc
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(PY3_INC) -Ofast $(LIBFLAGS) -o python3_bindings/errorx/errorx_lib.so $(SRCS) src/errorx_python.cc $(BOOST) $(FINAL)
	
python3_install: python3
	sudo $(PY3_EXE) -m pip install -I python3_bindings/

python3_package: python3_install
	$(tar) cfz ErrorX-1.0_$(OS)_python3.6.tar.gz python3_bindings/ --transform "s/python3_bindings/ErrorX_python3.6/"

java: $(SRCS)
	$(CXX) $(CPPFLAGS) $(WNO) $(INC) $(JAVA_INC) -Ofast $(LIBFLAGS) -o java_bindings/errorx/liberrorx.$(DLLEXT) $(SRCS) src/errorx_java.cc $(BOOST) $(FINAL)
	cd java_bindings; make

java_package: java
	$(tar) cfz ErrorX-1.0_$(OS)_java.tar.gz java_bindings/bin java_bindings/database java_bindings/errorx/liberrorx* java_bindings/ErrorX.jar java_bindings/internal_data java_bindings/optional_file --transform "s/java_bindings/ErrorX_java/"

clean: 
	rm -rf bin/errorx* lib/* python*_bindings/errorx/errorx_lib.so java_bindings/errorx/liberrorx.$(DLLEXT)
	
