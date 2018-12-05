#! /bin/bash

../bin/errorx -s human -n 1 --out new/binary_test_tsv.tsv --format tsv input_files/test.tsv
../bin/errorx -s human -n 1 --out new/binary_test_fastq.tsv --format fastq input_files/test.fastq
../bin/errorx -s human -n -1 --out new/binary_test_fastq.tsv --format fastq input_files/test.fastq
