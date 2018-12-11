#! /bin/csh

echo "Running binary..."
./test_binary.sh

echo "Running Python..."
python test_python_bindings.py

echo "Running Java..."
make -s

echo "Checking output..."

@ failed = 0
foreach file ( binary_fastq_out.tsv  binary_tsv_out.tsv    java_fastq_out.tsv    java_tsv_out.tsv      python_fastq_out.tsv  python_tsv_out.tsv )
	set output = `diff new/$file old/$file`
	if ( "$output" != "") then
		echo $file" is different! Build test failed"
		# @ failed = $failed + 1
		@ failed +=  1
	endif
end


if ( $failed == 0 ) then
	echo "All tests passed!"
else
	echo $failed" of 6 tests failed"
endif 
