# C++ API

## Using the API
In addition to the ErrorX binary, you can integrate the functions into your own C++ application using the shared library API. The dynamic library is location in the `ErrorX/lib/` folder. In addition, all of the input files used in this tutorial can be found in the `ErrorX/documentation` folder.

## Walkthrough
Below is a sample of code showing how you would use ErrorX in a C++ application. In this example, we are going to process a file called `test.tsv`, and output it to `test_out.tsv`. ErrorX takes TSV files with four columns: the sequence ID or name, the nucleotide sequence itself, the inferred germline sequence, and the PHRED score from the sequencer.

First, we need to include the relevant ErrorX headers:

	#include "errorx.hh"
	#include "SequenceRecords.hh"
	#include "ErrorXOptions.hh"

Next, we will create an ErrorXOptions object that contains all the options related to your processing. We will set the input filename to test.tsv, and the file type to TSV. ErrorX currently accepts files in FASTQ, FASTA, TSV format. We will also set the species as human - ErrorX currently supports human and mouse. Lastly, we need to set the path to the ErrorX folder. This will be used to locate different libraries needed for execution.

	errorx::ErrorXOptions options( "test.tsv", "tsv" );
	options.outfile( "test_out.tsv" );
	options.errorx_base( "/path/to/ErrorX/" );
	options.species( "human" );

Now, we're ready to run the protocol. If we have provided a FASTQ or FASTA file, ErrorX will go through germline assignment and then error correction. If we use a TSV file with the germline sequence already specified, then it will only do error correction:

	errorx::run_protocol_write( options );

This will run the protocol and output the results to our previously defined outfile. 

You can also run a query using a FASTQ file, and ErrorX will do the germline assignment for you. To do so, you only need to give a FASTQ file to ErrorXOptions, and set the format type to `"fastq"`, as shown below:

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

	clang++ -std=c++11 -IErrorX/include/ -LErrorX/lib/ -lerrorx ExampleApp.cc

The `-I` flag will include all of the ErrorX headers during compilation, and the `-L` and `-l` flags will makes sure the correct library is linked. 

**Note:** when you run the application linking to ErrorX, the ErrorX library **must** be in the path where your OS is going to look for dynamically linked libraries. In Linux this path is set by an environmental variable called `LD_LIBRARY_PATH`, or `DYLD_LIBRARY_PATH` on Mac. Make sure to run the following command to set the path correctly (or, even better, add this line to your `~/.bashrc` and never worry about it again!):
	
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:ErrorX/lib/
