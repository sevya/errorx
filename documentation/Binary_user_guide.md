
# ErrorX User Guide
The following is a guide to how to run the ErrorX command line application. ErrorX supports error correction for B-cell receptor sequences from human or mouse samples. 


### Input format
ErrorX accepts two file formats: FASTQ files, where ErrorX will do the germline assignment and error correction, or TSV files, where the germline sequence is already specified and ErrorX only does the error correction portion. TSV files should have four columns: 1. Sequence ID, 2. Nucleotide sequence, 3. Inferred germline sequence, and 4. PHRED score

### Example command line

	/path/to/ErrorX/bin/errorx --format fastq --out test_out.tsv --species human --nthreads -1 --error-threshold 0.88 test.fastq
	
### Command line options

Below are the acceptable command line options for running ErrorX. You can also see this list by running `/path/to/ErrorX/bin/errorx -h`

	-h [ --help ]                         produce help message
	-f [ --format ] arg                   input file format. Valid entries are
	                                      fastq or tsv.
	-o [ --out ] arg (=out.tsv)           output file (Default=out.tsv)
	-s [ --species ] arg (=human)         Species for IGBLAST search. Valid
	                                      entries are human or mouse.
	                                      (Default=human)
	-n [ --nthreads ] arg (=-1)           Number of threads to use during
	                                      execution. Enter -1 to use all
	                                      available (Default=-1)
	-e [ --error-threshold ] arg (=0.884311)
	                                      Probability cutoff for a base to be
	                                      considered an error. Higher=more
	                                      stringent in calling errors. Don't
	                                      change this value unless you know what
	                                      you are doing.
	--infile arg                          input file
	--version                             Print ErrorX version information and
	                                      exit
	-v [ --verbose ] arg (=1)             Verbosity level: should ErrorX output
	                                      extra warnings and messages?
	                                      0: don't output any message at all
	                                      1: output progress during processing
	                                      2: output progress and debugging
	                                      messages
	                                      (default=1)
	--allow-nonproductive                 Allow nonproductive and out-of-frame
	                                      sequences to be included? (default=No)
	--license arg                         License key to activate full version of
	                                      ErrorX
	