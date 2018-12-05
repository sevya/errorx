#! /bin/csh

bin/TestIGBlastParser
bin/TestSequenceFeatures
bin/TestUtil

python test_python_bindings.py

java -cp .:../java_bindings/ErrorX.jar:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore TestErrorX
