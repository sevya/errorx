#! /bin/csh

bin/TestErrorX
bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil
if ( `uname -s` == "DARWIN" ) then
	setenv DYLD_LIBRARY_PATH ../lib/
else
	setenv LD_LIBRARY_PATH ../lib/
endif
bin/TestLinking

/usr/bin/python test_python_bindings.py

make TestJava
