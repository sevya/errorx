#! /bin/csh

bin/TestErrorX
bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil
setenv DYLD_LIBRARY_PATH ../lib/
bin/TestLinking

/usr/bin/python test_python_bindings.py

make TestJava
