# Java API

## Installation

The ErrorX Java library is also located in the gzipped archive, in the folder java_bindings. To install you don't need to do anything special - all the functionality is contained in the `ErrorX.jar` archive within the `java_bindings` folder. When you are compiling your Java application simply add the flag 

	-cp 'ErrorX/java_bindings/ErrorX.jar'

to make sure the JAR is linked, and you are all set! 

Note: the JAR relies on data files contained in the java_bindings folder. For best results, leave the directory structure of java_bindings intact and only link the JAR. Alternatively, if you have to move the JAR file, make sure to move all the other directories in `java_bindings` along with it.

## Walkthrough
Similar to the Python interface, the Java API consists of three main functions: 

1.	Runs the whole protocol on a FASTQ or TSV file and writes to another TSV file,
1.	Takes in an array of sequences, germline sequences, and PHRED scores and returns an array of corrected sequences,
1.	Takes in a single sequence, germline sequence, and PHRED score and returns an array where element i is the probability that nucleotide at position i is an error (positions are zero-indexed).

Below are examples of how to run each scenario. A full example is presented in `ErrorX/documentation/ExampleApp.java`. First import the errorx package. 

	import errorx.ErrorX;

Next, in the body of your class, you need to create an ErrorXOptions object that defines, at minimum, the name of the file to be processed and its format (either FASTQ or TSV). ErrorXOptions can also be used for more advanced control of the protocol.

	ErrorXOptions options = new ErrorXOptions( "example.tsv", "tsv" );
	options.outfile( "out.tsv" );
	options.species( "mouse" );
		
	ErrorX ex = new ErrorX();
	ex.runProtocol( options );

For scenario two, you need to make arrays of Strings representing the nucleotide sequence, germline sequence, and PHRED score of your query. The function `correctSequences` will return an array of Strings with the corrected nucleotide sequences corresponding to the input:

	String sequence = "TACTCCCG";
	String germline_sequence = "TACTCCCA";
	String phred_score = "HEIHHIII";
	
	String[] sequences = new String[1];
	sequences[0] = sequence;

	String[] germline_sequences = new String[1];
	germline_sequences[0] = germline_sequence;

	String[] phred_scores = new String[1];
	phred_scores[0] = phred_score;

	String[] correctedSequences = new ErrorX().correctSequences( sequences, germline_sequences, phred_scores );

For the final scenario, you need to input a single sequence, germline, and PHRED score. The function `getPredictedErrors` will return an array of doubles representing the probability of error at each position (zero-indexed):

	double[] errorPredictions = new ErrorX().getPredictedErrors(
	sequence, germline_sequence, phred_score );
	
	System.out.println( errorPredictions[0] ); // prints the probability of error at position 0


[Back to Home](index.md)