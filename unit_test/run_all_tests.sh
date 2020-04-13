#! /bin/csh

if ( `uname -s` == "DARWIN" || `uname -s` == "Darwin" ) then
    setenv DYLD_LIBRARY_PATH lib/
else
    setenv LD_LIBRARY_PATH lib/
endif
bin/TestUtil && \
bin/TestErrorX && \
bin/TestIGBlastParser && \
bin/TestSequenceFeatures && \
bin/TestSequenceRecords && \
bin/TestLinking && \
bin/TestKerasModel && \
bin/TestErrorXOptions && \
\
/usr/bin/python test_python_bindings.py && \
\
make TestJava
