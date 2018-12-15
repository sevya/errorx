#! /bin/csh

bin/TestErrorX
bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil
setenv LD_LIBRARY_PATH ../lib/
bin/TestLinking

python test_python_bindings.py

make TestJava
