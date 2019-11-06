#! /bin/csh

echo "Running binary..."
../bin/errorx -s mouse -n 1 --out new/binary_tsv_out.tsv --format tsv --verbose 0 input_files/test.tsv
../bin/errorx -s mouse -n 1 --out new/binary_fastq_out.tsv --format fastq --verbose 0 input_files/test.fastq

echo "Running Python2..."
python test_python_bindings.py

echo "Running Java..."
make -s TestJava

foreach build ( binary java python )
	foreach format ( fastq tsv )
		set output = `diff new/$build"_"$format"_out.tsv" old/$format"_out.tsv"`
		if ( "$output" != "") then
			echo $build"_"$format" is different! Build test failed"
			# @ failed = $failed + 1
			@ failed +=  1
		endif
	end
end

echo "Finished!"