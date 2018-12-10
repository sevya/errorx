#! /bin/bash

../bin/errorx -s mouse -n 1 --out new/binary_tsv_out.tsv --format tsv --verbose 0 input_files/test.tsv
../bin/errorx -s mouse -n 1 --out new/binary_fastq_out.tsv --format fastq --verbose 0 input_files/test.fastq
