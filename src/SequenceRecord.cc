/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecord.cc
@brief A record of one sequence to be error corrected
@details Single nucleotide sequence to be fed to error
correction. Can be initialized from a FASTQ file, TSV file,
or directly from a SequenceQuery.
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "SequenceRecord.hh"

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "constants.hh"
#include "util.hh"
#include "AbSequence.hh"

#include <boost/lexical_cast.hpp>

using namespace std;

namespace errorx {

SequenceRecord::SequenceRecord() : 
	n_errors_( 0 )
{}


SequenceRecord::SequenceRecord(SequenceRecord const & other) :
	sequence_( other.sequence_ ),
	n_errors_( other.n_errors_ ),
	predicted_errors_all_( other.predicted_errors_all_ )
{}

SequenceRecord::SequenceRecord( SequenceQuery & query ) : 
	n_errors_( 0 )
{
	sequence_.sequenceID( query.sequenceID() );
	sequence_.full_nt_sequence( query.sequence() );
	sequence_.full_gl_nt_sequence( query.germline_sequence() );
	sequence_.quality_string_trimmed( query.phred_string() );

}

SequenceRecord::SequenceRecord( AbSequence const & sequence ) :
	sequence_( sequence ),
	n_errors_( 0 )
{}


void SequenceRecord::print() const {
	sequence_.print();
}

vector<string> SequenceRecord::get_summary() const {
	return vector<string> {
			sequence_.sequenceID(),
			sequence_.v_gene(),
			sequence_.v_identity_fmt(),
			sequence_.v_evalue_fmt(),
			sequence_.d_gene(),
			sequence_.d_identity_fmt(),
			sequence_.d_evalue_fmt(),
			sequence_.j_gene(),
			sequence_.j_identity_fmt(),
			sequence_.j_evalue_fmt(),
			sequence_.strand(),
			sequence_.chain(),
			sequence_.productive_fmt(),
			sequence_.cdr3_nt_sequence(),
			sequence_.cdr3_aa_sequence(),
			sequence_.full_nt_sequence(),
			sequence_.full_gl_nt_sequence(),
			sequence_.quality_string_trimmed(),
			sequence_.full_aa_sequence(),
			sequence_.full_nt_sequence_corrected(),
			sequence_.full_aa_sequence_corrected(),
			to_string( n_errors_ )
	};
}


void SequenceRecord::correct_sequence(
		ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {
	if ( !isGood() ) return;

	predict_errors( predictor, options );

	int position;
	double probability;
	string full_nt_sequence_corrected = sequence_.full_nt_sequence();
	n_errors_ = 0;

	for ( int ii = 0; ii < predicted_errors_all_.size(); ++ii ) {
		position = predicted_errors_all_[ ii ].first;
		probability = predicted_errors_all_[ ii ].second;

		if ( probability > options.error_threshold() ) {
			full_nt_sequence_corrected[ position ] = options.correction();
			++n_errors_;
		}
	}

	sequence_.full_nt_sequence_corrected( full_nt_sequence_corrected );

	if ( sequence_.full_aa_sequence() != "N/A" ) {

		sequence_.full_aa_sequence_corrected( 
			util::translate( full_nt_sequence_corrected, sequence_.translation_frame() )
		);
	}
}

void SequenceRecord::predict_errors( ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {

	// just in case it's been used before
	predicted_errors_all_.clear();
		
	for ( int ii = 0; ii < sequence_.full_nt_sequence().length(); ++ii ) {
		SequenceFeatures features( this, ii );

		double error_probability = predictor.apply_model( features );
		predicted_errors_all_.push_back( pair<int,double>( ii, error_probability ));
	}
}

vector<vector<double>> SequenceRecord::get_features( ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {

	vector<vector<double>> features_2d;

	for ( int ii = 0; ii < sequence_.full_nt_sequence().length(); ++ii ) {
		SequenceFeatures features( this, ii );

		features_2d.push_back( features.get_feature_vector() );
	}
	return features_2d;
}

AbSequence SequenceRecord::sequence() const { return sequence_; }
bool SequenceRecord::isGood() const { return sequence_.isGood(); }
vector<pair<int,double>> SequenceRecord::get_predicted_errors() const { return predicted_errors_all_; }
string SequenceRecord::full_nt_sequence() const { return sequence_.full_nt_sequence(); }
string SequenceRecord::full_gl_nt_sequence() const { return sequence_.full_gl_nt_sequence(); }
string SequenceRecord::full_nt_sequence_corrected() const { return sequence_.full_nt_sequence_corrected(); }
int SequenceRecord::n_errors() const { return n_errors_; }
string SequenceRecord::sequenceID() const { return sequence_.sequenceID(); }
string SequenceRecord::cdr3_aa_sequence() const { return sequence_.cdr3_aa_sequence(); }
string SequenceRecord::full_aa_sequence() const { return sequence_.full_aa_sequence(); }
string SequenceRecord::v_gene() const { return sequence_.v_gene(); }
string SequenceRecord::d_gene() const { return sequence_.d_gene(); }
string SequenceRecord::j_gene() const { return sequence_.j_gene(); }
double SequenceRecord::v_identity() const { return sequence_.v_identity(); }
double SequenceRecord::d_identity() const { return sequence_.d_identity(); }
double SequenceRecord::j_identity() const { return sequence_.j_identity(); }
string SequenceRecord::chain() const { return sequence_.chain(); }
bool SequenceRecord::productive() const { return sequence_.productive(); }
string SequenceRecord::quality_string() const { return sequence_.quality_string_trimmed(); }
int SequenceRecord::gl_start() const { return sequence_.gl_start(); }


} // namespace errorx
