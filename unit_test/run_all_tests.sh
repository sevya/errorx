#! /bin/csh

bin/TestErrorX
bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil

python test_python_bindings.py

make TestJava
