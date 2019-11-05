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
	sequence_(),
	n_errors_( 0 )
{}

SequenceRecord::~SequenceRecord() {}


SequenceRecord::SequenceRecord(SequenceRecord const & other) :
	sequence_( other.sequence_ ),
	n_errors_( other.n_errors_ ),
	predicted_errors_all_( other.predicted_errors_all_ )
{}

SequenceRecord::SequenceRecord( SequenceQuery & query ) : 
	sequence_(),
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

SequenceRecord::SequenceRecord( vector<string> const & items ) {
	if ( items.size() != util::get_labels().size() ) {
		throw invalid_argument( "AbSequence built from an incorrect items vector" );
	}

	sequence_.sequenceID_ = items[0];
	sequence_.v_gene_     = items[1];
	sequence_.v_identity_ = ( items[2]=="N/A" ) ? -1 : stod( items[2] );
	sequence_.v_evalue_   = ( items[3]=="N/A" ) ? -1 : stod( items[3] );

	sequence_.d_gene_     = items[4];
	sequence_.d_identity_ = ( items[5]=="N/A" ) ? -1 : stod( items[5] );
	sequence_.d_evalue_   = ( items[6]=="N/A" ) ? -1 : stod( items[6] );

	sequence_.j_gene_     = items[7];
	sequence_.j_identity_ = ( items[8]=="N/A" ) ? -1 : stod( items[8] );
	sequence_.j_evalue_   = ( items[9]=="N/A" ) ? -1 : stod( items[9] );

	sequence_.strand_     = items[10];
	sequence_.chain_      = items[11];
	sequence_.productive_ = (items[12]=="True");
	
	sequence_.cdr1_nt_sequence_ = items[13];
	sequence_.cdr1_aa_sequence_ = items[14];

	sequence_.cdr2_nt_sequence_ = items[15];
	sequence_.cdr2_aa_sequence_ = items[16];


	sequence_.cdr3_nt_sequence_ = items[17];
	sequence_.cdr3_aa_sequence_ = items[18];

	sequence_.full_nt_sequence_ = items[19];
	sequence_.full_gl_nt_sequence_ = items[20];
	sequence_.phred_trimmed_ = items[21];
	sequence_.full_aa_sequence_ = items[22];

	sequence_.full_nt_sequence_corrected_ = items[23];
	sequence_.full_aa_sequence_corrected_ = items[24];

	n_errors_ = ( items[25]=="N/A" ) ? -1 : stoi( items[25] );

	sequence_.hasV_ = ( sequence_.v_gene_!="N/A" );
	sequence_.hasD_ = ( sequence_.d_gene_!="N/A" );
	sequence_.hasJ_ = ( sequence_.j_gene_!="N/A" );
}

bool SequenceRecord::operator==( SequenceRecord const & other ) const {
	return sequence_==other.sequence_ && 
			n_errors_==other.n_errors_ &&
			predicted_errors_all_==other.predicted_errors_all_;
}

bool SequenceRecord::operator!=( SequenceRecord const & other ) const {
	return !((*this)==other);
}

bool SequenceRecord::equals( shared_ptr<SequenceRecord> const & other ) const {
	return (*this)==(*other);
}

void SequenceRecord::print() const {
	sequence_.print();
}

vector<string> SequenceRecord::get_summary( bool fulldata/*=1*/ ) const {
	if ( fulldata ) {
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
			sequence_.cdr1_nt_sequence(),
			sequence_.cdr1_aa_sequence(),
			sequence_.cdr2_nt_sequence(),
			sequence_.cdr2_aa_sequence(),
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
	} else {
		return vector<string> {
			sequence_.sequenceID(),
			sequence_.v_gene(),
			sequence_.d_gene(),
			sequence_.j_gene(),
			sequence_.full_nt_sequence(),
			sequence_.full_nt_sequence_corrected(),
			to_string( n_errors_ )
		};
	}
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
		SequenceFeatures features( *this, ii );
		double error_probability = predictor.apply_model( features );
		predicted_errors_all_.push_back( pair<int,double>( ii, error_probability ));
	}
}

vector<vector<double>> SequenceRecord::get_features( ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {

	vector<vector<double>> features_2d;

	for ( int ii = 0; ii < sequence_.full_nt_sequence().length(); ++ii ) {
		SequenceFeatures features( *this, ii );

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
string SequenceRecord::full_aa_sequence_corrected() const { return sequence_.full_aa_sequence_corrected(); }
string SequenceRecord::v_gene() const { return sequence_.v_gene(); }
string SequenceRecord::d_gene() const { return sequence_.d_gene(); }
string SequenceRecord::j_gene() const { return sequence_.j_gene(); }

string SequenceRecord::v_gene_noallele() const {
	vector<string> tokens = util::tokenize_string<string>( v_gene(), "*" );
	return tokens[0];
}

string SequenceRecord::d_gene_noallele() const {
	vector<string> tokens = util::tokenize_string<string>( d_gene(), "*" );
	return tokens[0];
}

string SequenceRecord::j_gene_noallele() const {
	vector<string> tokens = util::tokenize_string<string>( j_gene(), "*" );
	return tokens[0];
}

string SequenceRecord::clonotype() const {
	return v_gene_noallele() + "_" + 
		   cdr3_aa_sequence() + "_" + 
		   j_gene_noallele();
}

bool SequenceRecord::valid_clonotype() const {
	return sequence_.v_gene() != "N/A" && 
		   sequence_.cdr3_aa_sequence() != "N/A" &&
		   sequence_.j_gene() != "N/A";
}

double SequenceRecord::v_identity() const { return sequence_.v_identity(); }
double SequenceRecord::d_identity() const { return sequence_.d_identity(); }
double SequenceRecord::j_identity() const { return sequence_.j_identity(); }
string SequenceRecord::chain() const { return sequence_.chain(); }
bool SequenceRecord::productive() const { return sequence_.productive(); }
string SequenceRecord::quality_string() const { return sequence_.quality_string_trimmed(); }
int SequenceRecord::gl_start() const { return sequence_.gl_start(); }

void SequenceRecord::full_nt_sequence( string const & seq ) { sequence_.full_nt_sequence(seq); }
void SequenceRecord::full_nt_sequence_corrected( string const & seq ) 
{ sequence_.full_nt_sequence_corrected(seq); }

void SequenceRecord::full_aa_sequence( string const & seq ) { sequence_.full_aa_sequence(seq); }
void SequenceRecord::full_aa_sequence_corrected( string const & seq ) 
{ sequence_.full_aa_sequence_corrected(seq); }

void SequenceRecord::v_gene( string const & vgene ) { sequence_.v_gene( vgene ); }
void SequenceRecord::j_gene( string const & jgene ) { sequence_.j_gene( jgene ); }
void SequenceRecord::cdr3_aa_sequence( string const & cdr3_aa_sequence ) { sequence_.cdr3_aa_sequence( cdr3_aa_sequence ); }


} // namespace errorx
