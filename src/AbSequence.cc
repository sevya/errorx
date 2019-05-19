/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file AbSequence.cc
@brief Represents an antibody sequence
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "AbSequence.hh"
#include "ErrorXOptions.hh"
#include "util.hh"
#include "constants.hh"

#include <iostream>

using namespace std;

namespace errorx { 

AbSequence::AbSequence() : 
	sequenceID_( "" ),
	v_gene_( "" ),
	d_gene_( "" ),
	j_gene_( "" ),
	v_nts_( "" ),
	d_nts_( "" ),
	j_nts_( "" ),
	v_gl_nts_( "" ),
	d_gl_nts_( "" ),
	j_gl_nts_( "" ),
	v_identity_( -1 ),
	d_identity_( -1 ),
	j_identity_( -1 ),
	v_evalue_( -1 ),
	d_evalue_( -1 ),
	j_evalue_( -1 ),
	chain_( "" ),
	productive_( 0 ),
	strand_( "" ),
	query_start_( -1 ),
	gl_start_( -1 ),
	translation_frame_( -1 ),
	phred_( "" ),
	phred_trimmed_( "" ),
	full_nt_sequence_( "" ),
	full_gl_nt_sequence_( "" ),
	full_aa_sequence_( "" ),
	cdr3_nt_sequence_( "" ),
	cdr3_aa_sequence_( "" ),
	full_nt_sequence_corrected_( "" ),
	full_aa_sequence_corrected_( "" ),
	good_( 1 ),
	failure_reason_( "" )
	{}

AbSequence::AbSequence( AbSequence const & other ) : 
	sequenceID_( other.sequenceID_ ),
	v_gene_( other.v_gene_ ),
	d_gene_( other.d_gene_ ),
	j_gene_( other.j_gene_ ),
	v_nts_( other.v_nts_ ),
	d_nts_( other.d_nts_ ),
	j_nts_( other.j_nts_ ),
	jxn_nts_( other.jxn_nts_ ),
	v_gl_nts_( other.v_gl_nts_ ),
	d_gl_nts_( other.d_gl_nts_ ),
	j_gl_nts_( other.j_gl_nts_ ),
	v_identity_( other.v_identity_ ),
	d_identity_( other.d_identity_ ),
	j_identity_( other.j_identity_ ),
	v_evalue_( other.v_evalue_ ),
	d_evalue_( other.d_evalue_ ),
	j_evalue_( other.j_evalue_ ),
	chain_( other.chain_ ),
	productive_( other.productive_ ),
	strand_( other.strand_ ),
	query_start_( other.query_start_ ),
	gl_start_( other.gl_start_ ),
	translation_frame_( other.translation_frame_ ),
	phred_( other.phred_ ),
	phred_trimmed_( other.phred_trimmed_ ),
	full_nt_sequence_( other.full_nt_sequence_ ),
	full_gl_nt_sequence_( other.full_gl_nt_sequence_ ),
	full_aa_sequence_( other.full_aa_sequence_ ),
	cdr3_nt_sequence_( other.cdr3_nt_sequence_ ),
	cdr3_aa_sequence_( other.cdr3_aa_sequence_ ),
	full_nt_sequence_corrected_( other.full_nt_sequence_corrected_ ),
	full_aa_sequence_corrected_( other.full_aa_sequence_corrected_ ),
	good_( other.good_ ),
	failure_reason_( other.failure_reason_ )
{}

void AbSequence::build() {
	ErrorXOptions options( "tmp.fastq","fastq" );
	build( options );
}

void AbSequence::build( ErrorXOptions const & options ) {

	// Consider the E value of gene matches and nullify data if need be
	if ( !hasV_ ) v_gene_ = "N/A";
	if ( !hasD_ ) d_gene_ = "N/A";
	if ( !hasJ_ ) j_gene_ = "N/A";

	// Subroutines to assemble the full sequence
	build_nt_sequence();
	build_phred();
	translate_sequence( options.allow_nonproductive() );

	// if verbosity is turned on, explain why the sequence was bad
	if ( options.verbose() > 1 and !good_ ) {
		cout << "Sequence ID " << sequenceID_ << 
		" failed for the following reason:\n" << 
		failure_reason_ << endl;
	}

}

void AbSequence::build_nt_sequence() {
	if ( !good_ ) return;

	// Assemble the junction based on the given information
	string full_junction;
	string full_gl_junction;

	// if junction NTs are N/A, replace with blank 
	for ( int ii = 0; ii < jxn_nts_.size(); ++ii ) {
		jxn_nts_[ ii ] = (jxn_nts_[ ii ]=="N/A") ? "" : jxn_nts_[ ii ];
	}

	// assemble junction based on whether it's a VDJ or VJ junction
	if ( chain_ == "VH" || chain_ == "VB" ) {
		assert( jxn_nts_.size()==3 );

		// if D is a bad match, fill it with dashes
		string dregion    = jxn_nts_[1];
		string dregion_gl = ( hasD_ ) ? d_gl_nts_   : string(d_gl_nts_.size(), '-');
		
		full_junction = jxn_nts_[0] + dregion + jxn_nts_[2]; 

		// if there are N nucleotides, these are undefined in the germline,
		// so fill them in with dashes
		// consider whether the D match is any good
		full_gl_junction = string(jxn_nts_[0].size(), '-') +
							dregion_gl +
							string(jxn_nts_[2].size(), '-');
	} else { 
		assert( jxn_nts_.size()==1 );

		full_junction = jxn_nts_[0];
		// if there are N nucleotides, these are undefined in the germline,
		// so fill them in with dashes
		full_gl_junction = string(jxn_nts_[0].size(), '-');
	}

	// Assemble the full NT sequence for both the query and germline
	// Build it stepwise - if there's no confident J assignment I 
	// don't want to include those spurious J nucleotides in the 
	// final sequence. Likewise if there's no D or J

	if ( !hasV_ ) {
		good_ = 0;
		failure_reason_ = "no good V gene assignment";
		return;
	}

	full_nt_sequence_    = v_nts_;
	full_gl_nt_sequence_ = v_gl_nts_;
	

	// If the J assignment is good, append the junction and J nucleotides
	if ( hasJ_ ) {
		full_nt_sequence_    += full_junction;
		full_nt_sequence_    += j_nts_;
		full_gl_nt_sequence_ += full_gl_junction;
		full_gl_nt_sequence_ += j_gl_nts_;
	} 

	// If the J assignment is bad but D assignment is good (which is rare),
	// append the junction but not the J nucleotides
	else if ( hasD_ ) {
		full_nt_sequence_    += full_junction;
		full_gl_nt_sequence_ += full_gl_junction;
	}


	if ( full_nt_sequence_.size() != full_gl_nt_sequence_.size() ) {
		good_ = 0;
		failure_reason_ = "Full and GL sequences are not the same length";
		return;
	}


	if ( full_nt_sequence_.size() < constants::WINDOW ) {
		good_ = 0;
		failure_reason_ = "Sequence is too short - minimum length is " + 
							to_string( constants::WINDOW );
		return;
	}
}

void AbSequence::build_phred() {
	if ( !good_ ) return;

	// Find the portion of the quality string that corresponds to the query
	// If the assignment was done on the reverse strand, flip the quality string
	if ( strand_ == "-" ) phred_ = util::reverse( phred_ );

	try {
		phred_trimmed_ = phred_.substr( query_start_-1, full_nt_sequence_.size() );
	} catch ( out_of_range & ) {
		good_ = 0;
		failure_reason_ = "Phred scores do not match with input sequence";
		return;
	}

	if ( full_nt_sequence_.size() != phred_trimmed_.size() ) {
		good_ = 0;
		failure_reason_ = "NT sequence and quality strings are not the same length";
		return;
	}
}

void AbSequence::translate_sequence( bool allow_nonproductive ) {
	if ( !good_ ) return;

	// base translation frame off of the match position of the germline
	// assume that all IMGT germlines start in the same frame
	// start % 3 == 1 -> already in correct frame
	if      ( gl_start_ % 3 == 1 ) translation_frame_ = 1;
	else if ( gl_start_ % 3 == 2 ) translation_frame_ = 3;
	else if ( gl_start_ % 3 == 0 ) translation_frame_ = 2;

	full_aa_sequence_ = util::translate( full_nt_sequence_, translation_frame_ );

	// Make it a bad sequence if non-productive
	// TODO I never double-check if it's productive at this point - 
	// should be inferred from IGBlast output. Should I check again?
	if ( !allow_nonproductive and !productive_ ) {
		good_ = 0;
		failure_reason_ = "Sequence is non-productive";
	}
}

void AbSequence::print() const {
	if ( good_ ) {
		cout << sequenceID_ << endl;
		cout << full_nt_sequence_ << endl;
		cout << full_gl_nt_sequence_ << endl;
		cout << full_aa_sequence_ << endl;
		cout << phred_trimmed_ << endl;
	} else {
		cout << sequenceID_ << endl;
		cout << failure_reason_ << endl;
	}
}

///// Getters
bool AbSequence::isGood() const { return good_; }
string AbSequence::why() const { return failure_reason_; }
string AbSequence::sequenceID() const { return sequenceID_;}
string AbSequence::v_gene() const { return v_gene_; }
string AbSequence::d_gene() const { return d_gene_; }
string AbSequence::j_gene() const { return j_gene_;}
double AbSequence::v_identity() const { return v_identity_; }
double AbSequence::d_identity() const { return d_identity_; }
double AbSequence::j_identity() const { return j_identity_; }

string AbSequence::v_identity_fmt() const { return (hasV_) ? to_string( v_identity_ ) : "N/A"; }
string AbSequence::d_identity_fmt() const { return (hasD_) ? to_string( d_identity_ ) : "N/A"; }
string AbSequence::j_identity_fmt() const { return (hasJ_) ? to_string( j_identity_ ) : "N/A"; }

string AbSequence::v_evalue_fmt() const { return (hasV_) ? util::to_scientific( v_evalue_ ) : "N/A"; }
string AbSequence::d_evalue_fmt() const { return (hasD_) ? util::to_scientific( d_evalue_ ) : "N/A"; }
string AbSequence::j_evalue_fmt() const { return (hasJ_) ? util::to_scientific( j_evalue_ ) : "N/A"; }

string AbSequence::chain() const { return chain_; }
bool AbSequence::productive() const { return productive_; }
string AbSequence::productive_fmt() const { return (productive_) ? "True" : "False"; }
string AbSequence::strand() const { return strand_; }

string AbSequence::quality_string_trimmed() const { 
	if ( phred_trimmed_=="" ) return "N/A";
	else return phred_trimmed_; 
}

string AbSequence::full_nt_sequence() const { 
	if ( full_nt_sequence_=="" ) return "N/A";
	else return full_nt_sequence_; 
}

string AbSequence::full_gl_nt_sequence() const { 
	if ( full_gl_nt_sequence_=="" ) return "N/A";
	else return full_gl_nt_sequence_; 
}

string AbSequence::full_aa_sequence() const { 
	if ( full_aa_sequence_=="" ) return "N/A";
	else return full_aa_sequence_; 
}
string AbSequence::cdr3_nt_sequence() const { 
	if ( cdr3_nt_sequence_=="" ) return "N/A";
	else return cdr3_nt_sequence_; 
}
string AbSequence::cdr3_aa_sequence() const { 
	if ( cdr3_aa_sequence_=="" ) return "N/A";
	else return cdr3_aa_sequence_; 
}

string AbSequence::full_nt_sequence_corrected() const { 
	if ( full_nt_sequence_corrected_=="" ) return "N/A";
	else return full_nt_sequence_corrected_; 
}
string AbSequence::full_aa_sequence_corrected() const { 
	if ( full_aa_sequence_corrected_=="" ) return "N/A";
	else return full_aa_sequence_corrected_; 
}
int AbSequence::gl_start() const { return gl_start_; }
int AbSequence::translation_frame() const { return translation_frame_; }


///// Setters

void AbSequence::sequenceID( string const & seqID ) { sequenceID_ = seqID; }
void AbSequence::quality_string_trimmed( string const & phred ) { phred_trimmed_ = phred; }

void AbSequence::full_nt_sequence( string const & seq ) { full_nt_sequence_ = seq; }
void AbSequence::full_gl_nt_sequence( string const & seq ) { full_gl_nt_sequence_ = seq; }

void AbSequence::full_nt_sequence_corrected( string const & seq ) { 
	full_nt_sequence_corrected_ = seq;
}
void AbSequence::full_aa_sequence_corrected( string const & seq ) { 
	full_aa_sequence_corrected_ = seq;
}

void AbSequence::v_gene( string const & vgene ) { v_gene_ = vgene; }
void AbSequence::j_gene( string const & jgene ) { j_gene_ = jgene; }
void AbSequence::cdr3_aa_sequence( string const & cdr3_aa_sequence ) { cdr3_aa_sequence_ = cdr3_aa_sequence; }

} // namespace errorx