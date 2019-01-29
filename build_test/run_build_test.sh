#! /bin/csh

echo "Running binary..."
./test_binary.sh

echo "Running C++ library..."
make -s TestLinking
if ( `uname -s` == "DARWIN" || `uname -s` == "Darwin" ) then
    setenv DYLD_LIBRARY_PATH ../lib/
else
    setenv LD_LIBRARY_PATH ../lib/
endif
./a.out
 
echo "Running Python2..."
/usr/bin/python test_python_bindings.py

echo "Running Java..."
make -s TestJava

echo "Checking output..."

@ failed = 0
foreach build ( binary java python linking )
	foreach format ( fastq tsv )
		set output = `diff new/$build"_"$format"_out.tsv" old/$format"_out.tsv"`
		if ( "$output" != "") then
			echo $build"_"$format" is different! Build test failed"
			# @ failed = $failed + 1
			@ failed +=  1
		endif
	end
end


if ( $failed == 0 ) then
	echo "All tests passed!"
else
	echo $failed" of 8 tests failed"
endif 
