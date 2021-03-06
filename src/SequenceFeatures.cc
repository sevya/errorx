/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceFeatures.cc
@brief The features extracted from a nucleotide sequence to be 
used for neural network processing and error correction
@details This class calculates, from an NT sequence, the features
that are used in the NN implementation. It is then given to 
ErrorPredictor to do the actual transformation and prediction.
@author Alex Sevy (alex@endeavorbio.com)
*/


#include <iostream>
#include <vector>
#include <numeric>

#include "SequenceFeatures.hh"
#include "SequenceRecord.hh"
#include "util.hh"
#include "constants.hh"

using namespace std;

namespace errorx {

SequenceFeatures::SequenceFeatures( SequenceRecord const & record, int position ) {

	int window = constants::WINDOW;
	
	string full_nt_sequence = record.full_nt_sequence();
	string full_gl_nt_sequence = record.full_gl_nt_sequence();
	if ( position >= full_nt_sequence.size() ) {
		throw invalid_argument(
			"Error: position "+to_string(position)+" is out of bounds."
			);
	}


	// Get an array of PHRED scores as int, not char
	string phred_string = record.sequence().quality_string_trimmed();
	vector<int> phred_array = vector<int>( phred_string.size() );

	if ( full_nt_sequence.size() != phred_array.size() ) {
		throw invalid_argument(
			"Internal error: NT sequence is not the same length as phred array "
			"for sequence ID " + record.sequenceID()
			);
	}

	// decode quality string into an array of integer values
	for ( int ii = 0; ii < phred_string.size(); ++ii ) {
		phred_array[ii] = decode( phred_string[ ii ], 33 );
	}
	// get a window of quality scores around the position of interest
	quality_window_     = get_window( phred_array, position, window );
	// calculate average phred scores in global and local scope
	global_quality_avg_ = util::phred_avg_realspace( phred_array );
	local_quality_avg_  = util::phred_avg_realspace( quality_window_ );


	// get sequence window around position of interest and convert to binary
	sequence_window_    = get_window( full_nt_sequence, position, window );
	gl_sequence_window_ = get_window( full_gl_nt_sequence, position, window );

	sequence_window_binary_    = encode_sequence( sequence_window_ );
	gl_sequence_window_binary_ = encode_sequence( gl_sequence_window_ );


	// Calculate SHM and GC pct over the full sequence
	pair<double,double> global_metrics =
			calculate_metrics( full_nt_sequence, full_gl_nt_sequence );
	global_GC_pct_ = global_metrics.first;
	global_SHM_    = global_metrics.second;	

	// Calculate SHM and GC pct over the local sequence window
	pair<double,double> local_metrics =
			calculate_metrics( sequence_window_, gl_sequence_window_ );

	local_GC_pct_ = local_metrics.first;
	local_SHM_    = local_metrics.second;	

	is_germline_ = ( full_nt_sequence[position] == full_gl_nt_sequence[position] );

}

SequenceFeatures::~SequenceFeatures() {}

SequenceFeatures::SequenceFeatures( SequenceFeatures const & other ) :
		sequence_window_(other.sequence_window_),
		gl_sequence_window_(other.gl_sequence_window_),
		quality_window_(other.quality_window_),
		global_GC_pct_(other.global_GC_pct_),
		local_GC_pct_(other.local_GC_pct_),
		global_SHM_(other.global_SHM_),
		local_SHM_(other.local_SHM_),
		global_quality_avg_(other.global_quality_avg_),
		local_quality_avg_(other.local_quality_avg_),
		is_germline_(other.is_germline_),
		sequence_window_binary_( other.sequence_window_binary_ ),
		gl_sequence_window_binary_( other.gl_sequence_window_binary_ )
{}

string SequenceFeatures::get_window( string const & sequence, int position, int window ) const {
	string sequence_from_window;

	if ( sequence.length() < window ) {
		throw invalid_argument("Error: sequence is too short for the window you requested");
	}

	sequence_from_window = string(window, 'X') + sequence + string(window, 'X');
	
	position += window;

	return sequence_from_window.substr( position-window, (window*2)+1 );
}

vector<int> SequenceFeatures::get_window( vector<int> const & array, int position, int window ) const {

	if ( array.size() < window ) {
		throw invalid_argument("Error: sequence is too short for the window you requested");
	}

	vector<int> new_array;
	for ( int ii = 0; ii < window; ++ii )       new_array.push_back( -1 );
	for ( int ii = 0; ii < array.size(); ++ii ) new_array.push_back( array[ii] );
	for ( int ii = 0; ii < window; ++ii )       new_array.push_back( -1 );

	position += window;

	int start = position-window;
	int end = position+window;
	return vector<int>( new_array.begin()+start, new_array.begin()+end+1 );
}

vector<int> SequenceFeatures::nt_to_binary( char nt ) const {
	vector<int> array = {0,0,0,0,0,0};
	vector<char> nts = {'A','T','C','G','N','-'};

	// X is used internally to denote when a window goes past the end
	// or before the beginning of a sequence
	if ( nt == 'X' ) return array;
	if ( find( nts.begin(), nts.end(), nt )==nts.end() ) {
		throw invalid_argument("Error: "+string(1, nt)+" is not a valid nucleotide. "
			"Please make sure you are inputting DNA sequences for correction.");
	};

	int index = distance( nts.begin(),
				find( nts.begin(), nts.end(), nt ));

	array[ index ] = 1;
	return array;
}

// Calculate binary encoding of nt sequences
vector<int> SequenceFeatures::encode_sequence( string const & sequence ) const {
	vector<int> bin_array;
	vector<int> local_bin_array;

	for ( int ii = 0; ii < sequence.length(); ++ii ) {
		local_bin_array = nt_to_binary( sequence[ii] );
		bin_array.insert( bin_array.end(), local_bin_array.begin(), local_bin_array.end() );
	}
	return bin_array;
}

vector<double> SequenceFeatures::get_feature_vector() const {

	/// 11.28.18 AMS changed to remove three features that
	/// are count-based and not normalized
	/// These three are abs_position, global_gc, and local_gc

	/* ['global_gc_pct', 'local_gc_pct', 'global_avg_phred', 'local_avg_phred', 'nt_-41', 'nt_-42', 'nt_-43', 'nt_-44', 'nt_-45', 'nt_-46', 'nt_-31', 'nt_-32', 'nt_-33', 'nt_-34', 'nt_-35', 'nt_-36', 'nt_-21', 'nt_-22', 'nt_-23', 'nt_-24', 'nt_-25', 'nt_-26', 'nt_-11', 'nt_-12', 'nt_-13', 'nt_-14', 'nt_-15', 'nt_-16', 'nt_01', 'nt_02', 'nt_03', 'nt_04', 'nt_05', 'nt_06', 'nt_11', 'nt_12', 'nt_13', 'nt_14', 'nt_15', 'nt_16', 'nt_21', 'nt_22', 'nt_23', 'nt_24', 'nt_25', 'nt_26', 'nt_31', 'nt_32', 'nt_33', 'nt_34', 'nt_35', 'nt_36', 'nt_41', 'nt_42', 'nt_43', 'nt_44', 'nt_45', 'nt_46', 'phred_-8', 'phred_-7', 'phred_-6', 'phred_-5', 'phred_-4', 'phred_-3', 'phred_-2', 'phred_-1', 'phred_0', 'phred_1', 'phred_2', 'phred_3', 'phred_4', 'phred_5', 'phred_6', 'phred_7', 'phred_8', 'glnt_-41', 'glnt_-42', 'glnt_-43', 'glnt_-44', 'glnt_-45', 'glnt_-46', 'glnt_-31', 'glnt_-32', 'glnt_-33', 'glnt_-34', 'glnt_-35', 'glnt_-36', 'glnt_-21', 'glnt_-22', 'glnt_-23', 'glnt_-24', 'glnt_-25', 'glnt_-26', 'glnt_-11', 'glnt_-12', 'glnt_-13', 'glnt_-14', 'glnt_-15', 'glnt_-16', 'glnt_01', 'glnt_02', 'glnt_03', 'glnt_04', 'glnt_05', 'glnt_06', 'glnt_11', 'glnt_12', 'glnt_13', 'glnt_14', 'glnt_15', 'glnt_16', 'glnt_21', 'glnt_22', 'glnt_23', 'glnt_24', 'glnt_25', 'glnt_26', 'glnt_31', 'glnt_32', 'glnt_33', 'glnt_34', 'glnt_35', 'glnt_36', 'glnt_41', 'glnt_42', 'glnt_43', 'glnt_44', 'glnt_45', 'glnt_46', 'is_germline', 'local_SHM', 'global_SHM', 't0']
	 *
	 */


	vector<double> feature_vector = {
			global_GC_pct_,
			local_GC_pct_,
			global_quality_avg_ / 40.0, 
			local_quality_avg_ / 40.0 
		};

	feature_vector.insert( feature_vector.end(),
			sequence_window_binary_.begin() + 24,
			sequence_window_binary_.end()   - 24
			);

	for ( int ii = 4; ii < quality_window_.size() - 4; ++ii ) {
		// if the window extends before the beginning or after the end
		// of the sequence, its placeholder value is -1
		if ( quality_window_[ ii ] >= 0 ) {
			feature_vector.push_back( quality_window_[ ii ] / 40.0 );
		} else {
			feature_vector.push_back( quality_window_[ ii ] );
		}
	}

	feature_vector.insert( feature_vector.end(),
			gl_sequence_window_binary_.begin() + 24,
			gl_sequence_window_binary_.end()   - 24
			);

	feature_vector.push_back( is_germline_ );
	feature_vector.push_back( local_SHM_ );
	feature_vector.push_back( global_SHM_ );
	return feature_vector;
}

// Returns pair consisting of (GC_pct, SHM)
pair<double,double> SequenceFeatures::calculate_metrics( string const & sequence, string const & gl_sequence ) {
	int mutations = 0;
	int gc_count = 0;
	for ( int ii = 0; ii < sequence.length(); ++ii ) {
		if ( sequence[ii] != gl_sequence[ii] && gl_sequence[ii] != '-' ) mutations++;
		if ( sequence[ii] == 'G' || sequence[ii] == 'C' ) gc_count++;
	}
	double gc_pct  = (double)gc_count/(double)sequence.length();
	double shm_pct = (double)mutations/(double)sequence.length();
	return pair<double,double> ( gc_pct, shm_pct );
}

int SequenceFeatures::decode( char qual, int base ) {
	return (int)qual - base;
}

bool SequenceFeatures::is_germline() const { return is_germline_; }
vector<int> SequenceFeatures::quality_window() const { return quality_window_; }


} // namespace errorx
