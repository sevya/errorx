# ErrorX DNA correction software
## Overview

ErrorX is a revolutionary software for correction of DNA sequencing errors. In next-generation sequencing, it's common that many of the output sequences are actually errors, different from the input DNA sequence. This can be caused by the way the DNA was prepared, amplification before sequencing, and errors introduced by the sequencing machine itself. Unfortunately, these errors can be very costly to a next-generation sequencing pipeline, as you can never be sure which sequences are true and which are mistakes.

ErrorX solves this problem by using deep neural networks to predict sites where an error has been introduced. Trained on a dataset of tens of millions of sequences, ErrorX can pinpoint with extremely high accuracy positions where an error was likely introduced, saving valuable time and energy by removing these bases from further analysis. In benchmark studies, ErrorX was able to identify errors with an accuracy of 99.9% and a false positive rate of only 0.1%. 

ErrorX currently supports processing of antibody and T-cell repertoire sequencing data from human and mice, gathered on Illumina HiSeq and MiSeq instruments.

To install ErrorX, simply extract the `tar.gz` archive, which contains binaries for running the application and libraries for building the ErrorX platform into a C++, Python, or Java application. For more information see the Installation page.

## Quickstart

After installing ErrorX you are ready to run your first sequences. Use the following command to run ErrorX prediction:

	/path/to/ErrorX/bin/errorx --species human --format fastq --out ExampleSequences.tsv --allow-nonproductive ExampleSequences.fastq
	
This will create a file called `ExampleSequences.tsv` that contains a summary of your input sequences, plus the nucleotide sequences with ErrorX correction applied.

## Input
### FASTQ
The most common way to run ErrorX is with a FASTQ file. ErrorX will run germline assignment on the sequences and use sequence data and quality information to feed into the error prediction model.
### TSV
Alternatively you can provide sequences in TSV format. If you've already run germline assignment with another software, you can save time and only run error prediction in ErrorX. TSV files should have four columns, separated by tabs, with no header: 1. Sequence ID, 2. Nucleotide sequence, 3. Inferred germline sequence, and 4. PHRED score.

## Output
The output of ErrorX is a TSV file summarizing the input sequences along with a corrected nucleotide sequence, where the predicted errors are replaced by 'N'. If you input a FASTQ sequence, then the TSV will have information on the V, D, and J genes, as well as the level of somatic mutation and CDR3 sequence.

