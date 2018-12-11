#! /bin/csh

echo "Running binary..."
./test_binary.sh

echo "Running c++ library..."
make -s TestLinking
setenv DYLD_LIBRARY_PATH ../lib/
./a.out
 
echo "Running Python..."
python test_python_bindings.py

echo "Running Java..."
make -s TestJava

echo "Checking output..."

@ failed = 0
foreach build ( binary java python linking )
	foreach format ( fastq tsv )
		set output = `diff new/$build"_"$format"_out.tsv" old/$format"_out.tsv"`
		if ( "$output" != "") then
			echo $file" is different! Build test failed"
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
