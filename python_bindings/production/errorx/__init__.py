import errorx_lib

from ErrorXOptions import ErrorXOptions
import os

'''
	ErrorX bindings for Python. This file provides an interface
	for using the ErrorX dynamically linked library in a Python
	package
'''

'''
	Get the base path where ErrorX is located

	@return base path 
'''
def _get_base():
	return os.path.dirname(errorx_lib.__file__)

'''
	Error correct the provided sequences. All parameters
	are matched up elementwise, so they must have the same
	length.

	@param sequences List of NT sequences to correct
	@param germline_sequences List of inferred germline sequences
	corresponding to the entries in `sequences`
	@param phred_scores List of PHRED quality scores straight
	from the sequencer, corresponding to the entries in `sequences`

	@return List of corrected NT sequences corresponding
	to the input param `sequences`
'''
def correct_sequences(sequences, 
			   germline_sequences,
			   phred_scores ):

	if not isinstance(sequences, list):
		sequences = [sequences]
	if not isinstance(germline_sequences, list):
		germline_sequences = [germline_sequences]
	if not isinstance(phred_scores, list):
		phred_scores = [phred_scores]

	return errorx_lib.correct_sequences( sequences,germline_sequences,phred_scores, _get_base() )

'''
	Get the predicted likelihood that each base along a NT sequence
	is an error. 

	@param sequence A single NT sequence to correct
	@param germline_sequences Inferred germline sequence
	@param phred_scores PHRED quality scores

	@return Python list of tuples showing the probability of error for each base, 
	each tuple representing (position, probability)
'''
def get_predicted_errors(sequence, 
			   germline_sequence,
			   phred_score ):

	return errorx_lib.get_predicted_errors( sequence,germline_sequence,phred_score, _get_base() )

'''
	Run the ErrorX protocol and write the results to a file

	@param options ErrorXOptions object with the processing details
'''
def run_protocol( options ):
	options.base_path( _get_base() )
	return errorx_lib.run_protocol( options )


