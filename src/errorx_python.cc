/**
@file errorx_python.cc
@brief Python bindings to expose ErrorX interface
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "errorx_python.hh" 

#include "SequenceQuery.hh"
#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "DataScaler.hh"
#include "util.hh"
#include "errorx.hh"

#include <boost/python.hpp>
#include <boost/filesystem.hpp>

namespace py = boost::python;
using namespace std;

namespace errorx {

SequenceRecords* get_corrected_records( py::list & sequence_list, 
					 py::list & germline_sequence_list,
					 py::list & phred_score_list,
					 ErrorXOptions & options ) {

	if ( py::len(sequence_list) != py::len(germline_sequence_list) or 
		 py::len(sequence_list) != py::len(phred_score_list)) {
		throw invalid_argument("Error: the length of lists containing sequences, "
			"germline sequences, and phred scores are not uniform.");
	}

	vector<SequenceQuery> queries;

	for ( int ii = 0; ii < py::len(sequence_list); ++ii ) {
		SequenceQuery query ( "Sequence"+to_string(ii),
							  py::extract<string>( sequence_list[ii] ),
							  py::extract<string>( germline_sequence_list[ii] ),
							  py::extract<string>( phred_score_list[ii] )
							  );
		queries.push_back( query );
	}

	DataScaler scaler;
	ErrorPredictor predictor( options.verbose() );

	SequenceRecords* records = run_protocol( queries, options );
	return records;
}

py::list correct_sequences( py::list & sequence_list, 
					 py::list & germline_sequence_list,
					 py::list & phred_score_list,
					 py::object const & options ) 
{
	ErrorXOptions options_cpp = options_from_pyoptions( options );

	SequenceRecords* records = get_corrected_records( sequence_list, 
		germline_sequence_list, phred_score_list, options_cpp );

	py::list corrected_sequences;
	for ( int ii = 0; ii < records->size(); ++ii ) {
		corrected_sequences.append( records->get(ii)->full_nt_sequence_corrected() );
	}

	return corrected_sequences;
}

py::list get_predicted_errors( py::str & sequence, 
					 py::str & germline_sequence,
					 py::str & phred_score,
					 py::object const & options ) 
{

	py::list sequence_list;
	py::list germline_sequence_list;
	py::list phred_score_list;

	sequence_list.append( sequence );
	germline_sequence_list.append( germline_sequence );
	phred_score_list.append( phred_score );

	ErrorXOptions options_cpp = options_from_pyoptions( options );

	SequenceRecords* records = get_corrected_records( sequence_list,
		germline_sequence_list, phred_score_list, options_cpp );

	py::list predicted_errors;

	vector<pair<int,double>> errors = records->get( 0 )->get_predicted_errors();

	for ( int jj = 0; jj < errors.size(); ++jj ) {
		int position = errors[jj].first;
		double probability = errors[jj].second;
		predicted_errors.append( py::make_tuple(position,probability) );
	}

	return predicted_errors;
}

void run_py_protocol( py::object const & options ) {
	ErrorXOptions options_cpp = options_from_pyoptions( options );

	run_protocol_write( options_cpp );
}

ErrorXOptions options_from_pyoptions( py::object const & options ) {
	string infile = py::extract<string>( options.attr( "infile_" )); 
	string format = py::extract<string>( options.attr( "format_" )); 
	string outfile = py::extract<string>( options.attr( "outfile_" )); 
	string species = py::extract<string>( options.attr( "species_" )); 
	int verbose = py::extract<int>( options.attr( "verbose_" )); 
	double error_threshold = py::extract<double>( options.attr( "error_threshold_" ));
	bool allow_nonproductive = py::extract<bool>( options.attr( "allow_nonproductive_" )); 
	char correction = py::extract<char>( options.attr( "correction_" )); 
	string base_path = py::extract<string>( options.attr( "base_path_" )); 
	int nthreads = py::extract<int>( options.attr("nthreads_"));

	ErrorXOptions options_cpp( infile, format );
	options_cpp.outfile( outfile );
	options_cpp.species( species );
	options_cpp.verbose( verbose );
	options_cpp.error_threshold( error_threshold );
	options_cpp.allow_nonproductive( allow_nonproductive );
	options_cpp.correction( correction );
	options_cpp.errorx_base( base_path );
	options_cpp.nthreads( nthreads );

	return options_cpp;
}

BOOST_PYTHON_MODULE(errorx_lib)
{
	py::def("correct_sequences", &errorx::correct_sequences );
	py::def("get_predicted_errors", &errorx::get_predicted_errors );
	py::def("run_protocol", &errorx::run_py_protocol );
}

} // namespace errorx
