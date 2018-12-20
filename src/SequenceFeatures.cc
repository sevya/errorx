/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceFeatures.cc
@brief The features extracted from a nucleotide sequence to be 
used for neural network processing and error correction
@details This class calculates, from an NT sequence, the features
that are used in the NN implementation. It is then given to 
ErrorPredictor to do the actual transformation and prediction.
@author Alex Sevy (alex.sevy@gmail.com)
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

SequenceFeatures::SequenceFeatures( SequenceRecord* const record, int position ) {

	int window = constants::WINDOW;

	string full_nt_sequence = record->full_nt_sequence();
	string full_gl_nt_sequence = record->full_gl_nt_sequence();
	vector<int> quality_array = record->quality_array();

	if ( position >= full_nt_sequence.size() ) {
		throw invalid_argument(
			"Error: position "+to_string(position)+" is out of bounds."
			);
	}

	sequence_window_ = get_window( full_nt_sequence, position, window );
	gl_sequence_window_ = get_window( full_gl_nt_sequence, position, window );

	quality_window_ = get_window( quality_array, position, window );

	// abs_position_ = position;
	rel_position_ = (double)position/(double)full_nt_sequence.length();

	// global_GC_count_ = record->gc_count();
	global_GC_pct_ = record->gc_pct();

	pair<int,double> local_metrics =
			util::calculate_metrics( sequence_window_, gl_sequence_window_ );

	// local_GC_count_ = local_metrics.first;
	local_GC_pct_ = (double)local_metrics.first/(double)sequence_window_.length();
	
	local_SHM_ = local_metrics.second;
	global_SHM_ = record->shm();

	global_quality_avg_ = util::phred_avg_realspace( quality_array );
	local_quality_avg_ = util::phred_avg_realspace( quality_window_ );

	is_germline_ = full_nt_sequence[position] == full_gl_nt_sequence[position];

	initialize();
}

SequenceFeatures::SequenceFeatures( SequenceFeatures const & other ) :
		sequence_window_(other.sequence_window_),
		gl_sequence_window_(other.gl_sequence_window_),
		quality_window_(other.quality_window_),
		// abs_position_(other.abs_position_),
		rel_position_(other.rel_position_),
		// global_GC_count_(other.global_GC_count_),
		global_GC_pct_(other.global_GC_pct_),
		// local_GC_count_(other.local_GC_count_),
		local_GC_pct_(other.local_GC_pct_),
		global_SHM_(other.global_SHM_),
		local_SHM_(other.local_SHM_),
		global_quality_avg_(other.global_quality_avg_),
		local_quality_avg_(other.local_quality_avg_),
		is_germline_(other.is_germline_)
{
	initialize();
}

string SequenceFeatures::get_window( string sequence, int position, int window ) const {
	string sequence_from_window;

	if ( sequence.length() < window ) {
		throw invalid_argument("Error: sequence is too short for the window you requested");
	}

	sequence = string(window, 'X') + sequence + string(window, 'X');
	
	position += window;

	return sequence.substr( position-window, (window*2)+1 );
}

vector<int> SequenceFeatures::get_window( vector<int> const & array, int position, int window ) const {

	if ( array.size() < window ) {
		throw invalid_argument("Error: sequence is too short for the window you requested");
	}

	vector<int> new_array;
	for ( int ii = 0; ii < window; ++ii ) new_array.push_back( -1 );
	for ( int ii = 0; ii < array.size(); ++ii ) new_array.push_back( array[ii] );
	for ( int ii = 0; ii < window; ++ii ) new_array.push_back( -1 );

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

void SequenceFeatures::initialize() {
	// Calculate binary encoding of nt sequences
	sequence_window_binary_ = encode_sequence( sequence_window_ );
	gl_sequence_window_binary_ = encode_sequence( gl_sequence_window_ );
}

vector<double> SequenceFeatures::get_feature_vector() const {

	/// 11.28.18 AMS changed to remove three features that
	/// are count-based and not normalized
	/// These three are abs_position, global_gc, and local_gc

	/* rel_position global_gc_pct
	 * local_gc_pct global_avg_phred
	 * local_avg_phred nt_-81 nt_-82 nt_-83 nt_-84
	 * nt_-85 nt_-86 nt_-71 nt_-72 nt_-73 nt_-74
	 * nt_-75 nt_-76 nt_-61 nt_-62 nt_-63 nt_-64
	 * nt_-65 nt_-66 nt_-51 nt_-52 nt_-53 nt_-54
	 * nt_-55 nt_-56 nt_-41 nt_-42 nt_-43 nt_-44
	 * nt_-45 nt_-46 nt_-31 nt_-32 nt_-33 nt_-34
	 * nt_-35 nt_-36 nt_-21 nt_-22 nt_-23 nt_-24
	 * nt_-25 nt_-26 nt_-11 nt_-12 nt_-13 nt_-14
	 * nt_-15 nt_-16 nt_01 nt_02 nt_03 nt_04
	 * nt_05 nt_06 nt_11 nt_12 nt_13 nt_14 nt_15
	 * nt_16 nt_21 nt_22 nt_23 nt_24 nt_25 nt_26
	 * nt_31 nt_32 nt_33 nt_34 nt_35 nt_36 nt_41
	 * nt_42 nt_43 nt_44 nt_45 nt_46 nt_51 nt_52
	 * nt_53 nt_54 nt_55 nt_56 nt_61 nt_62 nt_63
	 * nt_64 nt_65 nt_66 nt_71 nt_72 nt_73 nt_74
	 * nt_75 nt_76 nt_81 nt_82 nt_83 nt_84 nt_85
	 * nt_86 phred_-8 phred_-7 phred_-6 phred_-5
	 * phred_-4 phred_-3 phred_-2 phred_-1 phred_0
	 * phred_1 phred_2 phred_3 phred_4 phred_5
	 * phred_6 phred_7 phred_8 glnt_-81 glnt_-82
	 * glnt_-83 glnt_-84 glnt_-85 glnt_-86 glnt_-71
	 * glnt_-72 glnt_-73 glnt_-74 glnt_-75 glnt_-76
	 * glnt_-61 glnt_-62 glnt_-63 glnt_-64 glnt_-65
	 * glnt_-66 glnt_-51 glnt_-52 glnt_-53 glnt_-54
	 * glnt_-55 glnt_-56 glnt_-41 glnt_-42 glnt_-43
	 * glnt_-44 glnt_-45 glnt_-46 glnt_-31 glnt_-32
	 * glnt_-33 glnt_-34 glnt_-35 glnt_-36 glnt_-21
	 * glnt_-22 glnt_-23 glnt_-24 glnt_-25 glnt_-26
	 * glnt_-11 glnt_-12 glnt_-13 glnt_-14 glnt_-15
	 * glnt_-16 glnt_01 glnt_02 glnt_03 glnt_04
	 * glnt_05 glnt_06 glnt_11 glnt_12 glnt_13
	 * glnt_14 glnt_15 glnt_16 glnt_21 glnt_22
	 * glnt_23 glnt_24 glnt_25 glnt_26 glnt_31
	 * glnt_32 glnt_33 glnt_34 glnt_35 glnt_36
	 * glnt_41 glnt_42 glnt_43 glnt_44 glnt_45
	 * glnt_46 glnt_51 glnt_52 glnt_53 glnt_54
	 * glnt_55 glnt_56 glnt_61 glnt_62 glnt_63
	 * glnt_64 glnt_65 glnt_66 glnt_71 glnt_72
	 * glnt_73 glnt_74 glnt_75 glnt_76 glnt_81
	 * glnt_82 glnt_83 glnt_84 glnt_85 glnt_86
	 * is_germline local_SHM global_SHM
	 *
	 */


	vector<double> feature_vector = {
			rel_position_,
			global_GC_pct_,
			local_GC_pct_,
			global_quality_avg_, 
			local_quality_avg_};

	feature_vector.insert( feature_vector.end(),
			sequence_window_binary_.begin(),
			sequence_window_binary_.end());

	feature_vector.insert( feature_vector.end(),
			quality_window_.begin(),
			quality_window_.end());

	feature_vector.insert( feature_vector.end(),
			gl_sequence_window_binary_.begin(),
			gl_sequence_window_binary_.end());

	feature_vector.push_back( is_germline_ );
	feature_vector.push_back( local_SHM_ );
	feature_vector.push_back( global_SHM_ );

	return feature_vector;
}

bool SequenceFeatures::is_germline() const { return is_germline_; }
double SequenceFeatures::rel_position() const { return rel_position_; }

} // namespace errorx
