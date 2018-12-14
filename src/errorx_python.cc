/**
@file errorx_python.cc
@brief Python bindings to expose ErrorX interface
@detail These bindings do not rely on boost/python at all,
hopefully they can be more portable
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <Python.h>
#include <bytesobject.h>

#include <iostream>
#include <vector>

#include "errorx_python.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "SequenceQuery.hh"

#include "errorx.hh"


using namespace std;

namespace errorx {
namespace python {

static PyObject* correct_sequences( PyObject* self, PyObject* args, PyObject* kwargs ) {

	static char *kwlist[] = {
		(char*)"sequence_list", 
		(char*)"germline_sequence_list", 
		(char*)"phred_score_list", 
		(char*)"options", 
		NULL
	};

	PyObject* sequenceArg;
	PyObject* glArg;
	PyObject* phredArg;
	PyObject* optionsArg;

	// Options were not passed correctly - set exception and exit
	if ( !PyArg_ParseTupleAndKeywords(
		args, kwargs, "OOOO", kwlist,
		&sequenceArg, &glArg, &phredArg, &optionsArg) ) 
	{
		PyErr_SetString(PyExc_TypeError, "Error: badly formed input arguments. Function accepts three keyword arguments: sequence_list, germline_sequence_list, phred_score_list, and (optionally) options");
		return NULL;

	} 

	// check arguments one by one to see if they are lists
	// this may not be necessary since we check in the Python
	// module, but what the hell
	if ( !PyList_Check( sequenceArg ) ) {
		PyErr_SetString( PyExc_TypeError, "Error: sequence_list is an invalid format. Must be a list" );
		return NULL;
	}

	if ( !PyList_Check( glArg ) ) {
		PyErr_SetString( PyExc_TypeError, "Error: germline_sequence_list is an invalid format. Must be a list" );
		return NULL;
	}

	if ( !PyList_Check( phredArg ) ) {
		PyErr_SetString( PyExc_TypeError, "Error: phred_score_list is an invalid format. Must be a list" );
		return NULL;
	}

	// sanity check - make sure length of all lists is uniform.
	// if not set exception and return null
	if ( PyList_Size( sequenceArg ) != PyList_Size( glArg ) or
		 PyList_Size( sequenceArg ) != PyList_Size( phredArg ) ) {
		PyErr_SetString( PyExc_TypeError, "Error: the length of sequence_list, "
		"germline_sequence_list, and phred_score_list are not uniform." );
		return NULL;
	}

	try {
		// if the attributes we extract are not present, then the object 
		// must be the wrong type, this throws an invalid_argument exception
		ErrorXOptions options = options_from_pyoptions( optionsArg );

		vector<string> sequences = pylist_to_vector( sequenceArg );
		vector<string> gl_sequences = pylist_to_vector( glArg );
		vector<string> phred_scores = pylist_to_vector( phredArg );
	
	
		// now I'm ready to run a query - use the function submit_query to 
		// run error correction and get the resulting SequenceRecords
		SequenceRecords* records = submit_query( 
			sequences, gl_sequences, phred_scores, options 
			);

		// Convert the records to a list of corrected sequences
		PyObject* output = PyList_New( records->size() );

		for ( int ii = 0; ii < records->size(); ++ii ) {
			string correctedSeq = records->get( ii )->full_nt_sequence_corrected();
			PyList_SetItem( output, ii, _PyString_FromString( correctedSeq.c_str() ));
		}

		delete records;

		// return list of corrected sequences
		return output;

	} catch ( invalid_argument & exc ) {
		PyErr_SetString( PyExc_TypeError, exc.what() );
		return NULL;
	}
}


static PyObject* get_predicted_errors( PyObject* self, PyObject* args, PyObject* kwargs ) {
	static char *kwlist[] = {
		(char*)"sequence", 
		(char*)"germline_sequence", 
		(char*)"phred_score", 
		(char*)"options", 
		NULL
	};

	PyObject* sequenceArg;
	PyObject* glArg;
	PyObject* phredArg;
	PyObject* optionsArg;

	// Options were not passed correctly - set exception and exit
	if ( !PyArg_ParseTupleAndKeywords(
		args, kwargs, "OOOO", kwlist,
		&sequenceArg, &glArg, &phredArg, &optionsArg) ) 
	{
		PyErr_SetString(PyExc_TypeError, "Error: badly formed input arguments. Function accepts three keyword arguments: sequence, germline_sequence, phred_score, and (optionally) options");
		return NULL;

	} 

	try {
		// if the attributes we extract are not present, then the object 
		// must be the wrong type, this throws an invalid_argument exception
		ErrorXOptions options = options_from_pyoptions( optionsArg );
		vector<string> sequences = { _PyString_AsString( sequenceArg ) };
		vector<string> gl_sequences = { _PyString_AsString( glArg ) };
		vector<string> phred_scores = { _PyString_AsString( phredArg ) };

		// now I'm ready to run a query - use the function submit_query to 
		// run error correction and get the resulting SequenceRecords
		SequenceRecords* records = submit_query( 
			sequences, gl_sequences, phred_scores, options 
			);

		// Convert the records to a list of error predictions
		vector<pair<int,double>> predictions = records->get(0)->get_predicted_errors();
		PyObject* output = PyList_New( predictions.size() );

		for ( int ii = 0; ii < predictions.size(); ++ii ) {
			double probability = predictions[ ii ].second;
			PyList_SetItem( output, ii, PyFloat_FromDouble( probability ));
		}

		delete records;

		// return list of corrected sequences
		return output;

	} catch ( invalid_argument & exc ) {
		PyErr_SetString( PyExc_TypeError, exc.what() );
		return NULL;
	}
}

static PyObject* run_protocol( PyObject* self, PyObject* args, PyObject* kwargs ) {
	static char *kwlist[] = {
		(char*)"options", 
		NULL
	};

	PyObject* optionsArg;

	// Options were not passed correctly - set exception and exit
	if ( !PyArg_ParseTupleAndKeywords(
		args, kwargs, "O", kwlist,
		&optionsArg) ) 
	{
		PyErr_SetString(PyExc_TypeError, "Error: badly formed input arguments. Function accepts one keyword argument: options");
		return NULL;

	}

	// if the attributes we extract are not present, then the object 
	// must be the wrong type, return an exception
	ErrorXOptions options_cpp;
	try {
		options_cpp = options_from_pyoptions( optionsArg );

		run_protocol_write( options_cpp );

		// return an empty string - will be interpreted as NULL
		return Py_BuildValue( "s", NULL );
	} catch ( invalid_argument & exc ) {
		PyErr_SetString( PyExc_TypeError, exc.what() );
		return NULL;
	}
	
	
}

SequenceRecords* submit_query( vector<string> & sequences, 
				   vector<string> & gl_sequences, 
				   vector<string> & phred_scores, 
				   ErrorXOptions & options ) 
{
	vector<SequenceQuery> queries;
	for ( int ii = 0; ii < sequences.size(); ++ii ) {
		SequenceQuery query ( "Sequence"+to_string(ii),
							  sequences[ ii ],
							  gl_sequences[ ii ],
							  phred_scores[ ii ] );

		queries.push_back( query );
	}

	DataScaler scaler;
	ErrorPredictor predictor( options.verbose() );

	SequenceRecords* records = run_protocol( queries, options );
	return records;
}

errorx::ErrorXOptions options_from_pyoptions( PyObject* const options ) {
	string infile = extract_string_attr( options, "infile_" );
	string format = extract_string_attr( options, "format_" );
	string outfile = extract_string_attr( options, "outfile_" );
	string species = extract_string_attr( options, "species_" );
	string base_path = extract_string_attr( options, "base_path_" );

	int verbose = extract_int_attr( options, "verbose_" );
	bool allow_nonproductive = (bool)extract_int_attr( options, "allow_nonproductive_" );
	double error_threshold = extract_double_attr( options, "error_threshold_" );
	string correction_str = extract_string_attr( options, "correction_" );
	char correction = correction_str[0];
	int nthreads = extract_int_attr( options, "nthreads_" );

	errorx::ErrorXOptions options_cpp( infile, format );
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


/// Helper functions
string extract_string_attr( PyObject* const options, string const & attr ) {

	if ( !PyObject_HasAttrString( options, attr.c_str() )) {
		throw invalid_argument( "Error: attribute "+attr+" not present. Make sure you are passing an ErrorX object" );
	}
	PyObject* attrObj = PyObject_GetAttrString( options, attr.c_str() );
	string attrStr = _PyString_AsString( attrObj );

	Py_XDECREF( attrObj );
	return attrStr;
}

int extract_int_attr( PyObject* const options, string const & attr ) {
	if ( !PyObject_HasAttrString( options, attr.c_str() )) {
		throw invalid_argument( "Error: attribute "+attr+" not present." );
	}
	PyObject* attrObj = PyObject_GetAttrString( options, attr.c_str() );
	int attrInt = (int)PyLong_AsLong( attrObj );
	Py_XDECREF( attrObj );
	return attrInt;
}

double extract_double_attr( PyObject* const options, string const & attr ) {
	if ( !PyObject_HasAttrString( options, attr.c_str() )) {
		throw invalid_argument( "Error: attribute "+attr+" not present." );
	}
	PyObject* attrObj = PyObject_GetAttrString( options, attr.c_str() );
	double attrDouble = PyFloat_AsDouble( attrObj );
	Py_XDECREF( attrObj );
	return attrDouble;
}

vector<string> pylist_to_vector( PyObject* const list ) {
	int length = PyList_Size( list );

	PyObject *item;
	vector<string> out_vect( length );

	for ( int ii = 0; ii < length; ++ii ) {
		item = PyList_GetItem( list, ii );
		// we already know these will be strings, since
		// the Python interface translates them before
		// passing it to C++ api
		string element = _PyString_AsString( item );
		out_vect[ ii ] = element;
	}

	return out_vect;
}

PyObject* _PyString_FromString( const char* v ) {
	#if PY_MAJOR_VERSION >= 3
		return PyBytes_FromString( v );
	#else
		return PyString_FromString( v );
	#endif
}

string _PyString_AsString( PyObject* obj ) {
	#if PY_MAJOR_VERSION >= 3

		PyObject* repr = PyObject_Repr( obj );
		PyObject* str = PyUnicode_AsEncodedString( repr, "utf-8", "~E~" );
		const char *bytes = PyBytes_AsString( str );

		Py_XDECREF(repr);
		Py_XDECREF(str);

		string objString( bytes );
		
		// for some reason this encoding surrounds the string with single quotes,
		// i.e. 'tsv' instead of tsv. Trim them off here
		return objString.substr( 1, objString.size()-2 );

	#else
		return string( PyString_AsString( obj ));
	#endif
}

} // namespace python
} // namespace errorx

/// Expose API methods to Python
static PyMethodDef ExposedMethods[] = {
	{ 
		"correct_sequences", 
		(PyCFunction)errorx::python::correct_sequences, 
		METH_VARARGS | METH_KEYWORDS, 
		"Correct sequences using ErrorX" 
	},
	{ 
		"get_predicted_errors", 
		(PyCFunction)errorx::python::get_predicted_errors, 
		METH_VARARGS | METH_KEYWORDS, 
		"Get probability of error per position using ErrorX" 
	},
	{ 
		"run_protocol", 
		(PyCFunction)errorx::python::run_protocol, 
		METH_VARARGS | METH_KEYWORDS, 
		"Run ErrorX protocol from a file and write results to a file" 
	},
	{ NULL, NULL, 0, NULL }
};


/// different init modules for Python 2 vs 3
#if PY_MAJOR_VERSION >= 3
	static struct PyModuleDef errorx_lib = {
		PyModuleDef_HEAD_INIT,
		"errorx_lib", // name of module
		NULL, // module documentation, may be NULL
		-1, //size of per-interpreter state of the module, 
			// or -1 if the module keeps state in global variables.
		ExposedMethods
	};

	PyMODINIT_FUNC
	PyInit_errorx_lib( void )
	{
		return PyModule_Create( &errorx_lib );
	}
#else
	PyMODINIT_FUNC
	initerrorx_lib( void )
	{
		(void) Py_InitModule("errorx_lib", ExposedMethods);
	}
#endif