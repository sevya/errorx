#! /bin/csh

bin/TestErrorX
bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil

/usr/bin/python test_python_bindings.py

make TestJava
