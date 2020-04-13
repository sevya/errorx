# Python API

## Installation

The Python packages are also located in the folder you just extracted. ErrorX supports Python versions 2.7 and 3.6. To install the Python 2.7 package simply run

	pip install python2_bindings/
	
and for version 3.6 run

	pip3 install python3_bindings/
	
You are now ready to run ErrorX through the Python interface!

## Walkthrough

To run the Python interface, you first need to import the errorx package to have access to all the functions to do data correction. You can also import the ErrorXOptions class if you want to have more fine-tuned control over the settings. To run the entire protocol, you need to provide an input file, as well as its format. A full example is provided in `ErrorX/documentation/ExampleApp.py`.

	import errorx as ex
	from errorx import ErrorXOptions
	
	options = ErrorXOptions('example.fastq','fastq')
	options.outfile( 'example_out.tsv' )
	options.species( 'mouse' )
	options.igtype( ‘Ig’ )
	ex.run_protocol( options )

You can also get corrected sequences from within the Python script that you can then operate on, shown below. This function takes in lists of nucleotide sequences, their inferred germline sequences, and the PHRED quality scores, and returns a list of corrected sequences corresponding to the input sequences:
	
	sequences = ['TACTCCCG'] 
	germline_sequences = ['TACTCCCA']
	phred_scores = ['HEIHHIII']
	
	results = correct_sequences(sequences, germline_sequences, phred_scores, options )

You can also input a single sequence, and get the predicted probability of error for each position along that sequence (positions are zero-indexed):
	
	sequence = 'TACTCCCG'
	germline_sequence = 'TACTCCCA'
	phred_scores = 'HEIHHIII'
	
	probabilities = get_predicted_errors(sequences, germline_sequences, phred_scores, options )
	
	# returns probability of error at position 0
	print probabilities[0] 
