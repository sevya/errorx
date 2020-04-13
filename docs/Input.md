# Input

## FASTQ
The most common way to run ErrorX is with a FASTQ file. ErrorX will run germline assignment on the sequences and use sequence data and quality information to feed into the error prediction model.

## FASTA
Alternatively, ErrorX can be run on a FASTA file. However, please note that error correction cannot be performed with FASTA input, since error correction requires the quality information that is missing in FASTA format. Running ErrorX on FASTA input will simply run germline assignment and annotate the input sequences.

## TSV
Alternatively you can provide sequences in TSV format. If you've already run germline assignment with another software, you can save time and only run error prediction in ErrorX. TSV files should have four columns, separated by tabs, with no header: 

1.	Sequence ID
1.	Nucleotide sequence
1.	Inferred germline sequence
1.	PHRED score.

## Output
The output of ErrorX is a TSV file summarizing the input sequences along with a corrected nucleotide sequence, where the predicted errors are replaced by 'N'. If you input a FASTQ sequence, then the TSV will have information on the V, D, and J genes, as well as the level of somatic mutation and CDR3 sequence.

## Full options list
Below is a list of all options that can be given to the application:

  
	-h [ --help ]					Produce help message
		
	-f [ --format ] arg				Input file format. Valid entries are fastq, fasta, or tsv.
		
	-o [ --out ] arg (=out.tsv)		Output file (Default=out.tsv)
		
	-s [ --species ] arg (=human)	Species for IGBLAST search. Valid entries are human or mouse (Default=human)
		
	--igtype arg (=Ig)				Receptor type for IGBLAST search. Valid entries are Ig or TCR. (Default=Ig)
		
	-n [ --nthreads ] arg (=-1)		Number of threads to use during execution. Enter -1 to use all available (Default=-1)
		
	-e [ --error-threshold ] arg (=0.730736)	Probability cutoff for a base to be considered an error. Higher=more stringent in calling errors. Don't change this value unless you know what you are doing.
		
	--infile arg					Input file name
		
	--version						Print ErrorX version information and exit
		
	-v [ --verbose ] arg (=1)		Verbosity level: should ErrorX output extra warnings and messages? 0: don't output any message at all. 1: output progress during processing. 2: output progress and debugging messages. (default=1)
		
	--allow-nonproductive			Allow nonproductive and out-of-frame sequences to be included? (default=No)
		
	 --license arg					License key to activate full version of ErrorX
