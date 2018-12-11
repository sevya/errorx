
# ErrorX API User Guide
The following is a guide to how to incorporate the ErrorX API into your application. ErrorX was written in C++ and has been ported to Java and Python, and can be run successfully from any of these languages.

# C++
## Quickstart
Below is a sample of code showing how you would use ErrorX in a C++ application. In this example, we are going to process a file called test.tsv, and output it to test\_out.tsv. ErrorX takes TSV files with four columns: the sequence ID or name, the nucleotide sequence itself, the inferred germline sequence, and the PHRED score from the sequencer.

First, we need to include the relevant ErrorX headers:

	#include "errorx.hh"
	#include "SequenceRecords.hh"
	#include "ErrorXOptions.hh"

Next, we will create an ErrorXOptions object that contains all the options related to your processing. We will set the input filename to test.tsv, and the file type to TSV. ErrorX currently accepts files in FASTQ or TSV format. We will also set the species as human - ErrorX currently supports human and mouse. Lastly, we need to set the path to the ErrorX folder. This will be used to locate different libraries needed for execution.

	errorx::ErrorXOptions options( "test.tsv", "tsv" );
	options.outfile( "test_out.tsv" );
	options.errorx_base( "/path/to/ErrorX/" );
	options.species( "human" );

Now, we're ready to run the protocol. If we have provided a FASTQ file, ErrorX will go through germline assignment and then error correction. If we use a TSV file with the germline sequence already specified, then it will only do error correction:

	errorx::run_protocol_write( options );

This will run the protocol and output the results to our previously defined outfile. Congrats! You just ran your first successful ErrorX error correction!

You can also run a query using a FASTQ file, and ErrorX will do the germline assignment for you. To do so, you only need to give a FASTQ file to ErrorXOptions, and set the format type to "fastq", as shown below:

	errorx::ErrorXOptions options( "test.fastq", "fastq" );
	options.outfile( "test_out.tsv" );
	options.species( "human" );
	options.errorx_base( "/path/to/ErrorX/" );
	errorx::run_protocol_write( options );
	
## Viewing and Analyzing SequenceRecords
The previous example only showed how to run the protocol and output the results to a file. But, you can also directly get the results of error correction to do further analysis within your C++ application. The central object in ErrorX is a SequenceRecord - this holds all the information for one nucleotide sequence, including its germline genes, level of somatic hypermutation, chain type, and eventually its corrected sequence. There is also a SequenceRecords object, which is a collection of separate SequenceRecord objects that have been processed together. Using the SequenceRecords object we can get access to the overall error rate in our sample, individual corrected sequences, and more. Using the ErrorXOptions object that we created before, we can get a pointer to a SequenceRecords object will all the information in the output file:

	errorx::SequenceRecords* records = errorx::run_protocol( options );

To work with the original and corrected nucleotide sequences of one of the entries, we can access them directly through the get() method. This will return a pointer to the SequenceRecord at the index that we provide:

	std::cout << "Original NT sequence: " << records->get( 0 ).full_nt_sequence() << std::endl;
	std::cout << "Corrected NT sequence: " << records->get( 0 ).full_nt_sequence_corrected() << std::endl;

We can also print out a summary of the data in our SequenceRecords object:

	records->print_summary();

A full working example is shown below:

	#include "errorx.hh"
	#include "SequenceRecords.hh"
	#include "ErrorXOptions.hh"
	#include <iostream>
	
	int main() {
		errorx::ErrorXOptions options( "test.tsv", "tsv" );
		options.outfile( "test_out.tsv" );
		options.species( "human" );
		options.errorx_base( "/path/to/ErrorX/" );
		errorx::run_protocol_write( options );
		errorx::SequenceRecords* records = errorx::run_protocol( options );
		std::cout << "Original NT sequence: " << records->get( 0 )->full_nt_sequence() << std::endl;
		std::cout << "Corrected NT sequence: " << records->get( 0 )->full_nt_sequence_corrected() << std::endl;
		records->print_summary();
	}


## Compilation
To use the ErrorX library in a C++ application, you only need to include two extra steps while you are compiling your code: 1. include the header files from ErrorX, and 2. dynamically link to the library. A successful clang command looks something like this:

	clang++ -std=c++11 -I/Path/to/ErrorX/include/ -L/Path/to/ErrorX/lib/ -lerrorx ExampleApp.cc

The `-I` flag will include all of the ErrorX headers during compilation, and the `-L` and `-l` flags will makes sure the correct library is linked. Note: make sure to replace /Path/to/ErrorX to the location of ErrorX on your computer.

**Note**: when you run the application linking to ErrorX, the ErrorX library **must** be in the path where your OS is going to look for dynamically linked libraries. In Linux this path is set by an environmental variable called LD\_LIBRARY\_PATH, or DYLD\_LIBRARY\_PATH on Mac. Make sure to run the following command to set the path correctly (or, even better, add this line to your ~/.bashrc and never worry about it again!):
	
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/Path/to/ErrorX/lib/

# Python
## Installation
The ErrorX library is distributed as a Python package that you can install using pip. Once you have downloaded the archived Python package, unpack and install it with the following commands:

	tar xvfz ErrorX-1.0.tar.gz
	pip install ErrorX-1.0/
		

## Quickstart
You can run ErrorX through the Python API to make the analysis process quick and seamless. Below is an example of how to run a basic ErrorX query:

	import errorx as ex
	
	sequences = ['AGGGGCCACAGTCAAGTTGTCCTGCACAGCT']
				           
	germline_sequences = ['AGGGGCCACAGTCATGTTGTCCTGTACAGCT']

	phred_scores = ['GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG']

	results = ex.correct_sequences(sequences, 
			   germline_sequences,
			   phred_scores )
			   
	print results[0]

ErrorX takes in a list of nucleotide sequences, a list of the inferred germline sequences, and the PHRED quality scores corresponding to each sequence. It will run the error correction protocol on each sequence in the list (matched up element-wise with the germline sequences and PHRED scores) and return a list of corrected sequences. You can also see the predicted probability that each base is an error, using the `get_predicted_errors` method. Note, that `get_predicted_errors` only takes a sequence, germline sequence, and PHRED score, and not a list. This method returns a list of tuples, where each tuple is made up of `(position,probability_of_error)`:

	sequence = 'AGGGGCCACAGTCAAGTTGTCCTGCACAGCT'
				           
	germline_sequence = 'AGGGGCCACAGTCATGTTGTCCTGTACAGCT'

	phred_score = 'GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG'

	result = ex.get_predicted_errors(sequence, 
			   germline_sequence,
			   phred_score )
			   
	print result
	
In addition, you can run the ErrorX protocol using input from a file and outputting the results to a file, in the same way you would use the application. To do this you only need to create an ErrorXOptions object that contains all of the options around data processing. You can input files either in FASTQ or TSV format. TSV files should have four columns: the sequence ID or name, the nucleotide sequence itself, the inferred germline sequence, and the PHRED score from the sequencer. An example is shown below:

	import errorx as ex
	
	options = ex.ErrorXOptions('example.fastq','fastq')
	options.outfile( 'out.tsv' )
	ex.run_protocol( options )

This will process the sequences in `example.fastq`, and output the results to `out.tsv`.


# Java

## Quickstart
You can also run ErrorX through the Java API. Below is an example of how to run a basic ErrorX query:

	import errorx.ErrorX;
	
	public static void main( String[] args ) { 
		String[] sequences = new String[1];
		sequences[0] = "AGGGGCCACAGTCAAGTTGTCCTGCACAGCT";

		String[] germline_sequences = new String[1];
		germline_sequences[0] = "AGGGGCCACAGTCATGTTGTCCTGTACAGCT";

		String[] phred_scores = new String[1];
		phred_scores[0] = "GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG";

		String[] correctedSequences = new ErrorX().correctSequences( sequences, germline_sequences, phred_scores );
	}

ErrorX takes in as parameters three String arrays: an array of nucleotide sequences, an array of the inferred germline sequences, and an array of the PHRED quality scores corresponding to each sequence. It will run the error correction protocol on each sequence in the array (matched up element-wise with the germline sequences and PHRED scores) and return an array of corrected sequences. You can also see the predicted probability that each base is an error, using the `get_predicted_errors` method. Note, that `get_predicted_errors` only takes a single sequence, germline sequence, and PHRED score, and not an array. This method returns an array of doubles, where element `i` is the probability of error at position `i`:

	String sequence = "AGGGGCCACAGTCAAGTTGTCCTGCACAGCT";

	String germline_sequence = "AGGGGCCACAGTCATGTTGTCCTGTACAGCT";

	String phred_score = "GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG";

	double[] errorPredictions = new ErrorX().getPredictedErrors( 
			sequence, 
			germline_sequence, 
			phred_score );
	
In addition, you can run the ErrorX protocol using input from a file and outputting the results to a file, in the same way you would use the application. To do this you only need to create an ErrorXOptions object that contains all of the options around data processing. You can input files either in FASTQ or TSV format. TSV files should have four columns: the sequence ID or name, the nucleotide sequence itself, the inferred germline sequence, and the PHRED score from the sequencer. An example is shown below:

	import errorx.ErrorXOptions;
	
	ErrorXOptions options = new ErrorXOptions( "test.tsv", "tsv" );
	options.outfile( "out.tsv" );
		
	ErrorX ex = new ErrorX();
	ex.runProtocol( options );


This will process the sequences in `example.fastq`, and output the results to `out.tsv`.

## Compilation
The ErrorX library is distributed as a JAR archive, that can be linked to your existing Java code. **Note:** the JAR file comes in a folder containing other dependencies, which will be properly read if the JAR is left in its original location. To link the JAR library while compiling your own Java code, add the following commands:

	javac -cp '.:/Path/to/ErrorX/java_bindings/ErrorX.jar' ExampleApp.java
	java -cp '.:/Path/to/ErrorX/java_bindings/ErrorX.jar' ExampleApp