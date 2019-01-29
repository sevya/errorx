/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecord.cc
@brief A record of one sequence to be error corrected
@details Single nucleotide sequence to be fed to error
correction. Can be initialized from a FASTQ file, TSV file,
or directly from a SequenceQuery.
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "SequenceRecord.hh"

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "constants.hh"
#include "util.hh"

#include <boost/lexical_cast.hpp>

using namespace std;

namespace errorx {

SequenceRecord::SequenceRecord()
{
	initialize_variables();
}


SequenceRecord::SequenceRecord(SequenceRecord const & other) :
		sequenceID_(other.sequenceID_),
		v_gene_(other.v_gene_),
		d_gene_(other.d_gene_),
		j_gene_(other.j_gene_),
		strand_(other.strand_),
		chain_(other.chain_),
		productive_(other.productive_),
		cdr3_nt_sequence_(other.cdr3_nt_sequence_),
		cdr3_aa_sequence_(other.cdr3_aa_sequence_),
		full_nt_sequence_(other.full_nt_sequence_),
		full_gl_nt_sequence_(other.full_gl_nt_sequence_),
		full_aa_sequence_(other.full_aa_sequence_),
		quality_string_(other.quality_string_),
		v_identity_(other.v_identity_),
		v_evalue_(other.v_evalue_),
		d_identity_(other.d_identity_),
		d_evalue_(other.d_evalue_),
		j_identity_(other.j_identity_),
		j_evalue_(other.j_evalue_),
		full_nt_sequence_corrected_(other.full_nt_sequence_corrected_),
		full_aa_sequence_corrected_(other.full_aa_sequence_corrected_),
		n_errors_(other.n_errors_),
		predicted_errors_all_(other.predicted_errors_all_),
		good_sequence_(other.good_sequence_),
		hasV_(other.hasV_),
		hasD_(other.hasD_),
		hasJ_(other.hasJ_),
		// sequencing_machine_(other.sequencing_machine_),
		quality_array_(other.quality_array_),
		gc_count_(other.gc_count_),
		gc_pct_(other.gc_pct_),
		shm_(other.shm_),
		translation_frame_(other.translation_frame_),
		verbose_(other.verbose_),
		allow_nonproductive_(other.allow_nonproductive_)
{}

SequenceRecord::SequenceRecord( SequenceQuery & query,
						int verbose )
{
	initialize_variables();

	sequenceID_ = query.sequenceID();
	full_nt_sequence_ = query.sequence();
	full_gl_nt_sequence_ = query.germline_sequence();
	quality_string_ = query.phred_string();
	verbose_ = verbose;

	calculate_metrics();
}

SequenceRecord::SequenceRecord( vector<string> const & lines, 
	int verbose, 
	bool allow_nonproductive ) {

	initialize_variables();

	verbose_ = verbose;
	allow_nonproductive_ = allow_nonproductive;

	map<string,vector<string>> data_map;

	try {
		for ( int ii = 0; ii < lines.size(); ++ii ) {
			vector<string> tokens = util::tokenize_string<string>(lines[ii]);
			
			if ( tokens.size() < 2 ) continue;

			if ( tokens[1] == "Query:" ) {
				data_map.insert( make_pair("query_string", tokens) );
			}
			else if ( tokens[1] == "V-(D)-J" && tokens[2] == "rearrangement" ) {
				data_map.insert( make_pair("rearrangement_string", util::tokenize_string<string>(lines[ii+1])) );
			} else if ( tokens[1] == "V-(D)-J" && tokens[2] == "junction" ) {
				data_map.insert( make_pair("junction_string", util::tokenize_string<string>(lines[ii+1])) );

			} else if ( tokens[1] == "Sub-region" ) {
				data_map.insert( make_pair("subregion_string", util::tokenize_string<string>(lines[ii+1])) );
			} else if ( tokens[0] == "V" || tokens[0] == "D" || tokens[0] == "J" ) {
				data_map.insert( make_pair(tokens[0]+"region_string", tokens ));
			}
		}

		if ( data_map.find("query_string") == data_map.end() ||
			 data_map.find("rearrangement_string") == data_map.end() ||
			 // EDIT 11.1.18
			 // sometimes the sub-region annotation is not available
			 // not sure why this happens. If it's not there then
			 // I'll just ignore the CDR3 sequence I guess?
			 /// Not a good solution - TODO
			 // data_map.find("subregion_string") == data_map.end() or 
			 data_map.find("junction_string") == data_map.end() ) 
		{
			good_sequence_ = 0;
			if ( verbose_ > 1 ) cout <<  "some crucial element not found" << endl;
			return;
		}

		parse_query_string( data_map["query_string"] );
		parse_rearrangement_string( data_map["rearrangement_string"] );
		
		if ( data_map.find("subregion_string") != data_map.end() ) {
			parse_subregion_string( data_map["subregion_string"] );
		}

		if ( data_map.find("Vregion_string") != data_map.end() ) {
			parse_vregion_string( data_map["Vregion_string"] );
		} else {
			if ( verbose_ > 1 ) cout << "no V region found" << endl;
			good_sequence_ = 0;
		}

		if ( data_map.find("Dregion_string") != data_map.end() ) {
			parse_dregion_string( data_map["Dregion_string"] );
		}

		if ( data_map.find("Jregion_string") != data_map.end() ) {
			parse_jregion_string( data_map["Jregion_string"] );
		}


		parse_junction_string( data_map["junction_string"] );

	} catch ( bad_alloc & ) {
		if ( verbose_ > 1 ) {
			cout << sequenceID_ << ": something is wrong with the format" << endl;
		}
		good_sequence_ = 0;
		return;
	}

	if ( isGood() ) {
		assemble_full_sequence();
		calculate_metrics();
	}
}

void SequenceRecord::initialize_variables() {
	sequenceID_ = "";
	v_gene_ = "";
	d_gene_ = "";
	j_gene_ = "";
	strand_ = "";
	chain_ = chain_type::VH;
	productive_ = 0;
	cdr3_nt_sequence_ = "";
	cdr3_aa_sequence_ = "";
	v_identity_ = -1;
	v_evalue_ = -1;
	v_nts_ = "";
	v_gl_nts_ = "";
	d_identity_ = -1;
	d_evalue_ = -1;
	d_nts_ = "";
	d_gl_nts_ = "";
	j_identity_ = -1;
	j_evalue_ = -1;
	j_nts_ = "";
	j_gl_nts_ = "";
	full_nt_sequence_ = "";
	full_gl_nt_sequence_ = "";
	full_aa_sequence_ = "";
	query_start_ = -1;
	gl_start_ = -1;
	quality_string_full_ = "";
	quality_string_ = "";
	good_sequence_ = 1;
	hasV_ = 0;
	hasD_ = 0;
	hasJ_ = 0;

	full_nt_sequence_corrected_ = "";
	full_aa_sequence_corrected_ = "";
	translation_frame_ = -1;

	n_errors_ = 0;

	verbose_ = 1;
}

void SequenceRecord::parse_query_string( vector<string> const & tokens ) {
	string ID_plus_qualstring = tokens[2];
	vector<string> id_tokens = util::tokenize_string<string>( ID_plus_qualstring, "|" );
	if ( id_tokens.size() > 2 ) {
		throw invalid_argument( "Error: misformed sequence ID. You can't have the character \"|\" in your sequence ID." );
	}
	sequenceID_ = id_tokens[0];
	quality_string_full_ = id_tokens[1];
}

void SequenceRecord::parse_rearrangement_string( vector<string> const & summary_data ) {
	v_gene_ = summary_data[0];
	if ( summary_data.size() == 7 ) {
		// this is a light chain
		j_gene_ = summary_data[1];
		chain_ = chain_type::VL;
		productive_ = summary_data[3]=="No";

		strand_ = summary_data[6];
	} else if ( summary_data.size() == 8 ) {
		// this is a heavy chain
		d_gene_ = summary_data[1];
		j_gene_ = summary_data[2];
		chain_ = chain_type::VH;
		productive_ = summary_data[4]=="No";

		strand_ = summary_data[7];
	}

	hasV_ = v_gene_!="N/A";
	hasD_ = d_gene_!="N/A";
	hasJ_ = j_gene_!="N/A";

	if ( !productive_ && !allow_nonproductive_ ) {
		if ( verbose_ > 1 ) {
			cout << "Sequence is not productive." << endl;
		}
		good_sequence_ = 0;
	}

	if ( !hasV_ ) {
		if ( verbose_ > 1 ) {
			cout << sequenceID_ << ": this sequence can't assign a V gene" << endl;
		}
		good_sequence_ = 0;
	}

}

void SequenceRecord::parse_junction_string( vector<string> const & junction_data ) {
	if ( !isGood() ) return;

	switch ( chain_ ) {
		case VH:
		{
			string vd_junction_nts = (junction_data[1]=="N/A") ? "" : junction_data[1];
			string dregion_nts = (junction_data[2]=="N/A") ? "" : junction_data[2];
			string dj_junction_nts = (junction_data[3]=="N/A") ? "" : junction_data[3];

			full_junction_ = vd_junction_nts + d_nts_ + dj_junction_nts; 

			full_gl_junction_ = string(vd_junction_nts.size(), '-') +
								d_gl_nts_ +
								string(dj_junction_nts.size(), '-');

			break;
		}
		case VL:
		{
			string vj_junction_nts = (junction_data[1]=="N/A") ? "" : junction_data[1];

			full_junction_ = vj_junction_nts;
			full_gl_junction_ = string(vj_junction_nts.size(), '-');

			break;
		}
	}
}

void SequenceRecord::parse_subregion_string( vector<string> const & cdr3_data ) {
	cdr3_nt_sequence_ = cdr3_data[1];
	cdr3_aa_sequence_ = cdr3_data[2];
}

void SequenceRecord::parse_vregion_string( vector<string> const & data ) {
	if ( !isGood() ) return;

	try {
		v_identity_ = boost::lexical_cast<double>( data[3] );
		v_evalue_ = boost::lexical_cast<double>( data[12] );
		v_nts_ = data[16];
		v_gl_nts_ = data[17];
		query_start_ = boost::lexical_cast<int>( data[8] );
		gl_start_ = boost::lexical_cast<int>( data[10] );

		if ( v_evalue_ > constants::V_EVALUE_CUTOFF ) {
			if ( verbose_ > 1 ) {
				cout << "Bad match for V : " << sequenceID_<< endl;
			}
			v_gene_ = "N/A";
			hasV_ = 0;
			good_sequence_ = 0;
			return;
		}

	} catch ( out_of_range & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	} catch ( boost::bad_lexical_cast & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	}
}

void SequenceRecord::parse_dregion_string( vector<string> const & data ) {
	if ( !isGood() ) return;

	try {
		d_identity_ = boost::lexical_cast<double>( data[3] );
		d_evalue_ = boost::lexical_cast<double>( data[12] );
		d_nts_ = data[16];
		d_gl_nts_ = data[17];
		
		if ( d_evalue_ > constants::D_EVALUE_CUTOFF ) {
			d_gene_ = "N/A";
			hasD_ = 0;
			d_gl_nts_ = string( d_nts_.size(), '-' );
		} 

	} catch ( out_of_range & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	} catch ( boost::bad_lexical_cast & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	}
}

void SequenceRecord::parse_jregion_string( vector<string> const & data ) {
	if ( !isGood() ) return;

	try {
		j_identity_ = boost::lexical_cast<double>( data[3] );
		j_evalue_ = boost::lexical_cast<double>( data[12] );
		j_nts_ = data[16];
		j_gl_nts_ = data[17];	

		if ( j_evalue_ > constants::J_EVALUE_CUTOFF ) {
			j_gene_ = "N/A";
			hasJ_ = 0;
			j_gl_nts_ = string( j_nts_.size(), '-' );
		} 	

	} catch ( out_of_range & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	} catch ( boost::bad_lexical_cast & e ) {
		if ( verbose_ > 1 ) {
			cout << e.what() << endl;
		}
		good_sequence_ = 0;
		return;
	}

}


void SequenceRecord::assemble_full_sequence() {

	// Assemble the full NT sequence for both the query and germline
	// Build it stepwise - if there's no confident J assignment I 
	// don't want to include those spurious J nucleotides in the 
	// final sequence. Likewise if there's no D or J

	// We only get to this point if it's a valid V gene
	full_nt_sequence_ = v_nts_;
	full_gl_nt_sequence_ = v_gl_nts_;
	

	// If the J assignment is good, append the junction and J nucleotides
	if ( hasJ_ ) {
		full_nt_sequence_ += full_junction_;
		full_nt_sequence_ += j_nts_;
		full_gl_nt_sequence_ += full_gl_junction_;
		full_gl_nt_sequence_ += j_gl_nts_;
	} 
	// If the J assignment is bad but D assignment is good (which is rare),
	// append the junction but not the J nucleotides
	else if ( hasD_ ) {
		full_nt_sequence_ += full_junction_;
		full_gl_nt_sequence_ += full_gl_junction_;
	}


	if ( full_nt_sequence_.size() != full_gl_nt_sequence_.size() ) {
		if ( verbose_ > 1 ) {
			cout << "Error: full and GL sequences are not the same length: " << sequenceID_ << endl;
		}
		good_sequence_ = 0;
		return;
	}

	// Find the portion of the quality string that corresponds to the query
	// If the assignment was done on the reverse strand, flip the quality string
	if ( strand_ == "-" ) quality_string_full_ = util::reverse( quality_string_full_ );

	try {
		quality_string_ = quality_string_full_.substr( query_start_-1, full_nt_sequence_.size() );
	} catch ( out_of_range & ) {
		if ( verbose_ > 1 ) {
			cout << "Quality string is not long enough. Some formatting error. : " << sequenceID_ << endl;
		}
		good_sequence_ = 0;
		return;
	}

	if ( full_nt_sequence_.size() != quality_string_.size() ) {
		if ( verbose_ > 1 ) {
			cout << "Error: NT sequence and quality strings are not the same length: " << sequenceID_ << endl;
		}
		good_sequence_ = 0;
		return;
	}

	// base translation frame off of the match position of the germline
	// assume that all IMGT germlines start in the same frame

	// start % 3 == 1 -> already in correct frame
	if ( gl_start_ % 3 == 1 ) translation_frame_ = 1;
	else if ( gl_start_ % 3 == 2 ) translation_frame_ = 3;
	else if ( gl_start_ % 3 == 0 ) translation_frame_ = 2;

	full_aa_sequence_ = util::translate( full_nt_sequence_, translation_frame_ );

	// TODO I never double-check if it's productive at this point - 
	// should be inferred from IGBlast output. Should I check again?
}

void SequenceRecord::calculate_metrics() {

	quality_array_ = vector<int>( quality_string_.size() );

	// decode quality string into an array of integer values
	for ( int ii = 0; ii < quality_string_.size(); ++ii ) {
		quality_array_[ii] = decode( quality_string_[ ii ], 33 );
	}

	// calculate GC content and SHM over the whole sequence
	pair<int,double> metrics = util::calculate_metrics( full_nt_sequence_, full_gl_nt_sequence_ );
	gc_count_ = metrics.first;
	gc_pct_ = (double)gc_count_/(double)full_nt_sequence_.length();
	shm_ = metrics.second;
}

int SequenceRecord::decode( char qual, int base ) {
	return (int)qual - base;
}

bool SequenceRecord::isGood() { return good_sequence_; }

void SequenceRecord::print() const {
	if ( good_sequence_ ) {
		cout << sequenceID_ << endl;
		cout << full_nt_sequence_ << endl;
		cout << full_gl_nt_sequence_ << endl;
		cout << full_aa_sequence_ << endl;
		cout << quality_string_ << endl;

	} else {
		cout << sequenceID_ << endl;
		cout << "Nonproductive" << endl;
	}
}

vector<string> SequenceRecord::get_summary() const {
	return vector<string> {
			sequenceID_,
			(hasV_) ? v_gene_ : "N/A",
			(hasV_) ? to_string( v_identity_ ) : "N/A",
			(hasV_) ? util::to_scientific( v_evalue_ ) : "N/A",
			(hasD_) ? d_gene_ : "N/A",
			(hasD_) ? to_string( d_identity_ ) : "N/A",
			(hasD_) ? util::to_scientific( d_evalue_ ) : "N/A",
			(hasJ_) ? j_gene_ : "N/A",
			(hasJ_) ? to_string( j_identity_ ) : "N/A",
			(hasJ_) ? util::to_scientific( j_evalue_ ) : "N/A",
			(strand_!="") ? strand_ : "",
			(chain_==chain_type::VH) ? "VH" : "VL",
			(productive_) ? "True" : "False",
			(cdr3_nt_sequence_=="") ? "N/A": cdr3_nt_sequence_,
			(cdr3_aa_sequence_=="") ? "N/A": cdr3_aa_sequence_,
			(full_nt_sequence_=="") ? "N/A": full_nt_sequence_,
			(full_gl_nt_sequence_=="") ? "N/A": full_gl_nt_sequence_,
			(quality_string_=="") ? "N/A": quality_string_,
			(full_aa_sequence_=="") ? "N/A": full_aa_sequence_,
			(full_nt_sequence_corrected_=="") ? "N/A": full_nt_sequence_corrected_,
			(full_aa_sequence_corrected_=="") ? "N/A": full_aa_sequence_corrected_,
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
	full_nt_sequence_corrected_ = full_nt_sequence_;

	for ( int ii = 0; ii < predicted_errors_all_.size(); ++ii ) {
		position = predicted_errors_all_[ ii ].first;
		probability = predicted_errors_all_[ ii ].second;

		if ( probability > options.error_threshold() ) {
			full_nt_sequence_corrected_[ position ] = options.correction();
			++n_errors_;
		}
	}

	if ( full_aa_sequence_ != "" ) {
		full_aa_sequence_corrected_ = util::translate( full_nt_sequence_corrected_, translation_frame_ );
	}
}

void SequenceRecord::predict_errors( ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {

	// SequenceRecord* const current_record( this );
	for ( int ii = 0; ii < full_nt_sequence_.length(); ++ii ) {
		SequenceFeatures features( this, ii );

		double error_probability = predictor.apply_model( features );

		predicted_errors_all_.push_back( pair<int,double>( ii, error_probability ));
	}
}

vector<vector<double>> SequenceRecord::get_features( ErrorPredictor const & predictor,
		ErrorXOptions const & options ) {

	vector<vector<double>> features_2d;

	for ( int ii = 0; ii < full_nt_sequence_.length(); ++ii ) {
		SequenceFeatures features( this, ii );

		features_2d.push_back( features.get_feature_vector() );
	}
	return features_2d;
}

vector<pair<int,double>> SequenceRecord::get_predicted_errors() { return predicted_errors_all_; }
string SequenceRecord::full_nt_sequence() const { return full_nt_sequence_; }
string SequenceRecord::full_gl_nt_sequence() const { return full_gl_nt_sequence_; }
string SequenceRecord::full_nt_sequence_corrected() const { return full_nt_sequence_corrected_; }
int SequenceRecord::n_errors() const { return n_errors_; }
string SequenceRecord::sequenceID() const { return sequenceID_; }
vector<int> SequenceRecord::quality_array() const { return quality_array_; }
int SequenceRecord::gc_count() const { return gc_count_; }
double SequenceRecord::gc_pct() const { return gc_pct_; }
double SequenceRecord::shm() const { return shm_; }
string SequenceRecord::cdr3_aa_sequence() const { return cdr3_aa_sequence_; }
string SequenceRecord::full_aa_sequence() const { return full_aa_sequence_; }
string SequenceRecord::v_gene() const { return v_gene_; }
string SequenceRecord::d_gene() const { return d_gene_; }
string SequenceRecord::j_gene() const { return j_gene_; }
double SequenceRecord::v_identity() const { return v_identity_; }
double SequenceRecord::d_identity() const { return d_identity_; }
double SequenceRecord::j_identity() const { return j_identity_; }
chain_type SequenceRecord::chain() const { return chain_; }
string SequenceRecord::quality_string() const { return quality_string_; }

} // namespace errorx
