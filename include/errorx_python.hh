/**
@file errorx_python.hh
@brief Python bindings to expose ErrorX interface
@author Alex Sevy (alex.sevy@gmail.com)
*/

#ifndef INCLUDE_ERRORX_PYTHON_HH_
#define INCLUDE_ERRORX_PYTHON_HH_

#include "SequenceRecords.hh"
#include <boost/python.hpp>

namespace py = boost::python;
using namespace std;

namespace errorx {

/**
	Converts a std::vector to Python list

	@param input Templated std::vector
	@return Python list consisted of same elements
*/
template <typename T>
py::list vector_to_python_list( vector<T> const & input ) {
	py::list output;

	for ( int ii = 0; ii < input.size(); ++ii ) {
		output.append( input[ii] );
	}
	return output;
}

/**
	Runs the ErrorX prediction method on a set of sequences
	and returns the corrected sequences in a Python list.
	Note: the sequences in sequence_list, germline_sequence_list,
	and phred_score_list are matched up element-wise, and must
	be the same length

	Ex: sequence_list = ['ACTG','GTCA']
	germline_sequence_list = ['ATTG','CTCA']
	phred_score_list = ['GGFF','FFFG']

	@param sequence_list List of nucleotide sequences to be corrected
	@param germline_sequence_list List of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list List of PHRED scores as strings to be corrected
	@param errorx_base The root directory where the errorx code is located.
	This is set automatically by the Python code

	@throws invalid_argument if lists are not the same length
	
	@return Python list of corrected nucleotide sequences, corresponding 
	element-wise to the input sequence list
*/
py::list correct_sequences( py::list & sequence_list, 
					 py::list & germline_sequence_list,
					 py::list & phred_score_list,
					 py::str & errorx_base );

/**
	Runs the ErrorX prediction method on a single sequence
	and returns the probability that each base is an error.

	Ex: sequence = 'ACTG'
	germline_sequence = 'ATTG'
	phred_score = 'GGFF'

	@param sequence Nucleotide sequence to be corrected
	@param germline_sequence Germline sequence corresponding to sequence to be corrected
	@param phred_score PHRED score as string to be corrected
	@param errorx_base The root directory where the errorx code is located.
	This is set automatically by the Python code

	@throws invalid_argument if lists are not the same length

	@return Python list of tuples showing the probability of error for each base, 
	each tuple representing (position, probability)
*/
py::list get_predicted_errors( py::str & sequence, 
					 py::str & germline_sequence,
					 py::str & phred_score,
					 py::str & errorx_base );

/**
	Runs the ErrorX prediction method on a set of sequences. Both 
	correct_sequences and get_predicted_errors are wrappers for this
	subroutine.

	@param sequence_list List of nucleotide sequences to be corrected
	@param germline_sequence_list List of germline sequences corresponding 
	to each sequence to be corrected
	@param phred_score_list List of PHRED scores as strings to be corrected
	@param errorx_base The root directory where the errorx code is located.
	This is set automatically by the Python code

	@throws invalid_argument if lists are not the same length
	
	@return SequenceRecords* with error correction consisting of all input
	sequences
*/
SequenceRecords* get_corrected_records( py::list & sequence_list, 
					 py::list & germline_sequence_list,
					 py::list & phred_score_list,
					 py::str & errorx_base );


/**
	Runs the ErrorX prediction method on a set of sequences and outputs
	to a file. Wrapper for the run_protocol_write method in errorx.hh

	@param options ErrorXOptions Python object to control parameters of
	ErrorX. Will be converted internally to ErrorXOptions C++ object
*/
void run_py_protocol( py::object const & options );


} // namespace errorx

#endif /* INCLUDE_ERRORX_PYTHON_HH_ */