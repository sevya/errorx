/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file errorx_java.cc
@brief Java bindings to expose ErrorX interface
@author Alex Sevy (alex@endeavorbio.com)
*/


#include <jni.h>        // JNI header provided by JDK
#include <stdio.h>
#include "errorx_java.hh"
#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "errorx.hh"
#include "SequenceQuery.hh"
#include "exceptions.hh"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

JNIEXPORT jobject JNICALL Java_errorx_ErrorX_correctSequences( JNIEnv *env, jobject thisObj,
					 jobjectArray sequence_list, // String[] 
					 jobjectArray germline_sequence_list, // String[]
					 jobjectArray phred_score_list, // String[]
					 jobject options
					 )
{
	errorx::SequenceRecords* records = get_corrected_records( env, 
									 sequence_list, 
									 germline_sequence_list, 
									 phred_score_list,
									 options );

	vector<string> corrected_sequences;

	for ( int ii = 0; ii < records->size(); ++ii ) {
		corrected_sequences.push_back( 
			records->get(ii)->full_nt_sequence_corrected() 
			);
	}

	delete records;
	return vector_to_array( env, corrected_sequences );
}

JNIEXPORT jdoubleArray JNICALL Java_errorx_ErrorX_getPredictedErrors( JNIEnv* env, jobject thisObj,
					 jstring sequence, // String
					 jstring germline_sequence, // String
					 jstring phred_score, // String
					 jobject options
					 )
{
	// get_corrected_records needs a vector<string>, not a single string
	// wrap all the strings up in a vector
	vector<string> sequence_list = { jstring_to_string( env, sequence ) };
	vector<string> germline_sequence_list = { jstring_to_string( env, germline_sequence ) };
	vector<string> phred_score_list = { jstring_to_string( env, phred_score ) };

	errorx::ErrorXOptions options_cpp = joptions_to_options( env, options );
	errorx::SequenceRecords* records = get_corrected_records( env,
									 sequence_list, 
									 germline_sequence_list, 
									 phred_score_list,
									 options_cpp
									 );


	vector<double> predicted_errors; 
	vector<pair<int,double>> errors = records->get(0)->get_predicted_errors();

	for ( int jj = 0; jj < errors.size(); ++jj ) {
		double probability = errors[jj].second;
		predicted_errors.push_back( probability );
	}

	delete records;
	
	return vector_to_array( env, predicted_errors );

}

JNIEXPORT void JNICALL Java_errorx_ErrorX_runProtocol( JNIEnv *env, jobject thisObj,
					 jobject options ) // ErrorXOptions
{
	errorx::ErrorXOptions options_cpp = joptions_to_options( env, options );
	try {
		errorx::run_protocol_write( options_cpp );
	} catch ( errorx::InvalidLicenseException & exc ) {
		cout << exc.what() << endl;
		exit(1);
	}
}	


errorx::SequenceRecords* get_corrected_records( JNIEnv* env,
					 jobjectArray & sequence_list, // String[] 
					 jobjectArray & germline_sequence_list, // String[]
					 jobjectArray & phred_score_list, // String[]
					 jobject & options
					 ) 
{
	jsize sequence_len = env->GetArrayLength( sequence_list );
	jsize gl_sequence_len = env->GetArrayLength( germline_sequence_list ); 
	jsize phred_len = env->GetArrayLength( phred_score_list );
		
	if ( sequence_len != gl_sequence_len || sequence_len != phred_len ) {
		throw invalid_argument("Error: the length of lists containing sequences, "
			"germline sequences, and phred scores are not uniform.");
	}

	vector<string> sequences = array_to_vector( env, sequence_list );
	vector<string> germline_sequences = array_to_vector( env, germline_sequence_list );
	vector<string> phred_scores = array_to_vector( env, phred_score_list );

	errorx::ErrorXOptions options_cpp = joptions_to_options( env, options );
	return get_corrected_records( env, sequences, germline_sequences, phred_scores, options_cpp );
}


errorx::SequenceRecords* get_corrected_records( JNIEnv* env,
					 vector<string> & sequence_list, // ArrayList 
					 vector<string> & germline_sequence_list, // ArrayList
					 vector<string> & phred_score_list, // ArrayList
					 errorx::ErrorXOptions & options
					 ) 
{

	if ( sequence_list.size() != germline_sequence_list.size() || 
		 sequence_list.size() != phred_score_list.size() ) {
		throw invalid_argument("Error: the length of lists containing sequences, "
			"germline sequences, and phred scores are not uniform.");
	}

	// convert the sequences element-wise to SequenceQuery objects
	vector<errorx::SequenceQuery> queries;
	for ( int ii = 0; ii < sequence_list.size(); ++ii ) {
		errorx::SequenceQuery query( to_string(ii),
							 sequence_list[ ii ],
							 germline_sequence_list[ ii ],
							 phred_score_list[ ii ]
							);
		queries.push_back( query );
	}

	errorx::ErrorPredictor predictor( options );

	try {
		errorx::SequenceRecords* records = run_protocol( queries, options );
		return records;	
	} catch ( errorx::InvalidLicenseException & exc ) {
		cout << exc.what() << endl;
		exit(1);
	}
	
}


jdoubleArray vector_to_array( JNIEnv* env, vector<double> & in_vector ) {
	jdoubleArray out_array;
	const int size = in_vector.size();
	out_array = env->NewDoubleArray( size );
	if ( out_array == NULL ) {
		return NULL; // out of memory
	}

	// fill a temp structure to use to populate the java int array
	jdouble* fill = new jdouble [size];
	for ( int ii = 0; ii < size; ++ii ) {
		fill[ ii ] = in_vector[ ii ];
	}
	// move from the temp structure to the java structure
	env->SetDoubleArrayRegion( out_array, 0, size, fill );
	delete[] fill;
	return out_array;
}


jobjectArray vector_to_array( JNIEnv* env, vector<string> & in_vector ) {
	jobjectArray out_array;
	int size = in_vector.size();

	jobject initialElement = NULL;
	jclass elementClass = env->FindClass("java/lang/String");
	
	out_array = env->NewObjectArray( size, elementClass, initialElement );
	if ( out_array == NULL ) {
		return NULL; // out of memory
	}

	for ( int ii = 0; ii < in_vector.size(); ++ii ) {
		jstring element = env->NewStringUTF( in_vector[ ii ].c_str() );
		env->SetObjectArrayElement( out_array, ii, element );
		// Release local reference so Java GC can clean it up
		env->DeleteLocalRef( element );
	}
	return out_array;
}

vector<string> array_to_vector( JNIEnv* env, jobjectArray & array ) {
	jsize len = env->GetArrayLength( array );
	vector<string> out_vector( len );

	for ( int ii = 0; ii < len; ++ii ) {
		jobject obj = env->GetObjectArrayElement( array, ii );
		jstring element = static_cast<jstring>( obj );

		const char* pchars = env->GetStringUTFChars(element, nullptr);
		string sequence( pchars );
		out_vector[ ii ] = sequence;
		env->ReleaseStringUTFChars(element, pchars);
		// Release local references so Java GC can clean them up
		env->DeleteLocalRef(obj);
		env->DeleteLocalRef(element);
	}

	return out_vector;
}

string jstring_to_string( JNIEnv* env, jstring & jstr ) {
	const char* cstr = env->GetStringUTFChars( jstr, nullptr );
	string sequence( cstr );
	env->ReleaseStringUTFChars( jstr, cstr );
	return sequence;
}

errorx::ErrorXOptions joptions_to_options( JNIEnv* env, jobject & options ) {
	jclass cls = env->GetObjectClass( options );
	jfieldID fid = env->GetFieldID( cls, "infile_", "Ljava/lang/String;" );
	jstring infile = (jstring)env->GetObjectField( options, fid );

	fid = env->GetFieldID( cls, "format_", "Ljava/lang/String;" );
	jstring format = (jstring)env->GetObjectField( options, fid );

	fid = env->GetFieldID( cls, "outfile_", "Ljava/lang/String;" );
	jstring outfile = (jstring)env->GetObjectField( options, fid );

	fid = env->GetFieldID( cls, "species_", "Ljava/lang/String;" );
	jstring species = (jstring)env->GetObjectField( options, fid );

	fid = env->GetFieldID( cls, "base_path_", "Ljava/lang/String;" );
	jstring base_path = (jstring)env->GetObjectField( options, fid );

	fid = env->GetFieldID( cls, "verbose_", "I" ); // int type
	jint verbose = env->GetIntField( options, fid );

	fid = env->GetFieldID( cls, "allow_nonproductive_", "Z" ); // boolean type
	jboolean allow_nonproductive = env->GetBooleanField( options, fid );

	fid = env->GetFieldID( cls, "error_threshold_", "D" ); // double type
	jdouble error_threshold = env->GetDoubleField( options, fid );

	fid = env->GetFieldID( cls, "nthreads_", "I" ); // int type
	jint nthreads = env->GetIntField( options, fid );

	fid = env->GetFieldID( cls, "correction_", "C" ); // int type
	jchar correction = env->GetCharField( options, fid );

	errorx::ErrorXOptions options_cpp( 
				jstring_to_string( env, infile ), 
				jstring_to_string( env, format )
				);
	options_cpp.outfile( jstring_to_string( env, outfile ));
	options_cpp.species( jstring_to_string( env, species ));
	options_cpp.errorx_base( jstring_to_string( env, base_path ));
	
	options_cpp.verbose( (int)verbose );
	options_cpp.allow_nonproductive( (bool)allow_nonproductive );
	
	options_cpp.error_threshold( (double)error_threshold );
	options_cpp.nthreads( (int)nthreads );
	options_cpp.correction( (char)correction );

	return options_cpp;
}


