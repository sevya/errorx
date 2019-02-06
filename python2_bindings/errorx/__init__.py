'''
    ErrorX bindings for Python. This file provides an interface
    for using the ErrorX dynamically linked library in a Python
    package
'''
import errorx_lib

from ErrorXOptions import ErrorXOptions
import os


def _get_base():
	'''
	Get the base path where ErrorX is located

	@return base path 
	'''
	return os.path.dirname(errorx_lib.__file__)


def correct_sequences(sequences, 
			   germline_sequences,
			   phred_scores,
			   options=None ):
	'''
    Error correct the provided sequences. All parameters
    are matched up elementwise, so they must have the same
    length.

    @param sequences List of NT sequences to correct
    @param germline_sequences List of inferred germline sequences
    corresponding to the entries in `sequences`
    @param phred_scores List of PHRED quality scores straight
    from the sequencer, corresponding to the entries in `sequences`
    @param options ErrorXOptions object to control options. If
    None, will set up default values internally

    @return List of corrected NT sequences corresponding
    to the input param `sequences`
	'''
	## if input is a string, wrap into a list
	if isinstance( sequences, str ):
		sequences = [sequences]
	if isinstance( germline_sequences, str ):
		germline_sequences = [germline_sequences]
	if isinstance( phred_scores, str ):
		phred_scores = [phred_scores]

	if not options:
		options = ErrorXOptions( "tmp", "tsv" )

	options.base_path( _get_base() )

	## the C++ code is expecting a Python list - wrapping
	## the args in list() allows it to accept numpy arrays,
	## Pandas series, etc.
	return errorx_lib.correct_sequences( 
		list(sequences),
		list(germline_sequences),
		list(phred_scores), options )


def get_predicted_errors(sequence, 
			   germline_sequence,
			   phred_score,
			   options=None ):
	'''
	Get the predicted likelihood that each base along a NT sequence
	is an error. 

	@param sequence A single NT sequence to correct
	@param germline_sequences Inferred germline sequence
	@param phred_scores PHRED quality scores
	@param options ErrorXOptions object to control options. If 
	None, will set up default values internally

	@return Python list of probability of error for each base, 
	where position i is prob error at position i
	'''
	if not options:
		options = ErrorXOptions( "tmp", "tsv" )

	options.base_path( _get_base() )
	return errorx_lib.get_predicted_errors( 
		str(sequence), str(germline_sequence), str(phred_score), options )


def run_protocol( options ):
	'''
	Run the ErrorX protocol and write the results to a file

	@param options ErrorXOptions object with the processing details
	'''
	## When the user doesn't set infile it defaults to 'tmp'
	## if they're reading from a file this is not good
	if options.infile == 'tmp':
		print('Error: infile was not set')
		return

	options.base_path( _get_base() )
	return errorx_lib.run_protocol( options )


