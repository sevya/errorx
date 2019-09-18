/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ErrorXOptions.cc
@brief Controls the options around running the ErrorX protocol
@details 
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>

#include "ErrorXOptions.hh"
#include "util.hh"
#include "constants.hh"
#include "ProgressBar.hh"
#include "exceptions.hh"

#include <boost/filesystem.hpp>

using namespace std;

namespace errorx {

ErrorXOptions::ErrorXOptions() :
	infile_(""),
	format_(""),
	outfile_("out.tsv"),
	species_("human"),
	igtype_("Ig"),
	verbose_(1),
	error_threshold_( constants::OPTIMIZED_THRESHOLD ),
	allow_nonproductive_(0),
	correction_('N'),
	infasta_(""),
	igblast_output_(""),
	trial_(0),
	num_queries_(0)
{
	nthreads(-1);
	errorx_base( util::get_root_path().string() );
	
	initialize_callback();
}

// No class members we need to delete
ErrorXOptions::~ErrorXOptions() {}

ErrorXOptions & ErrorXOptions::operator=( ErrorXOptions const & other ) {
	infile_ = other.infile_;
	format_ = other.format_;
	outfile_ = other.outfile_;
	species_ = other.species_;
	igtype_ = other.igtype_;
	verbose_ = other.verbose_;
	nthreads_ = other.nthreads_;
	error_threshold_ = other.error_threshold_;
	allow_nonproductive_ = other.allow_nonproductive_;
	correction_ = other.correction_;
	infasta_ = other.infasta_;
	igblast_output_ = other.igblast_output_;
	errorx_base_ = other.errorx_base_;
	trial_ = other.trial_;
	num_queries_ = other.num_queries_;
	increment_ = other.increment_;
	reset_ = other.reset_;
	finish_ = other.finish_;
	message_ = other.message_;
	quality_map_ = other.quality_map_;
	return *this;
}

ErrorXOptions::ErrorXOptions( string input_file, string file_format ) :
	outfile_("out.tsv"),
	species_("human"),
	igtype_("Ig"),
	verbose_(1),
	error_threshold_( constants::OPTIMIZED_THRESHOLD ),
	allow_nonproductive_(0),
	correction_('N'),
	infasta_(""),
	igblast_output_(""),
	trial_(0),
	num_queries_(0)
{
	nthreads(-1);
	format( file_format );
	infile( input_file );
	errorx_base( util::get_root_path().string() );

	initialize_callback();
}

ErrorXOptions::ErrorXOptions( ErrorXOptions const & other ) :
	infile_(other.infile_),
	format_(other.format_),	
	outfile_(other.outfile_),
	species_(other.species_),
	igtype_(other.igtype_),
	verbose_(other.verbose_),
	nthreads_(other.nthreads_),
	error_threshold_(other.error_threshold_),
	allow_nonproductive_(other.allow_nonproductive_),
	correction_(other.correction_),
	infasta_(other.infasta_),
	igblast_output_(other.igblast_output_),
	errorx_base_(other.errorx_base_),
	trial_(other.trial_),
	num_queries_(other.num_queries_),
	increment_(other.increment_),
	reset_(other.reset_),
	finish_(other.finish_),
	message_(other.message_),
	quality_map_( other.quality_map_)
	{}

void ErrorXOptions::initialize_callback() {
	_bar = ProgressBar();

	if ( verbose_ > 0 ) {
		// Set up callback functions with the ProgressBar class
		// to show a progress bar to the terminal
		function<void(int,int)> terminal_callback = std::bind( 
			&ProgressBar::increment,
			&_bar,
			placeholders::_1,
			placeholders::_2
			);

		increment( terminal_callback );

		function<void(void)> reset_callback = std::bind( 
			&ProgressBar::reset,
			&_bar
			);
		reset( reset_callback );


		function<void(void)> finish_callback = std::bind( 
			&ProgressBar::finish,
			&_bar
			);
		finish( finish_callback );


		function<void(string)> message_callback = std::bind( 
			&ProgressBar::message,
			&_bar,
			placeholders::_1
			);

		message( message_callback );

	} else { 
		// Set up blank callback functions if verbose==0
		// these don't show anything
		function<void(int,int)> terminal_callback = 
			[](int,int) {};
		increment( terminal_callback );

		function<void(void)> blank_callback = 
			[](void) {};

		reset( blank_callback );
		finish( blank_callback );

		function<void(string)> message_callback = 
			[](string) {};
		message( message_callback );
	}
}

void ErrorXOptions::fastq_to_fasta() {
	ios_base::sync_with_stdio( false );


	ifstream infile ( infile_ );
	if ( !infile.good() ) {
		throw BadFileException("Error: file " + infile_ + " does not exist." );
	}

	message_( "Converting fastq to fasta..." );

	// Get base of input file to make FASTA name
	namespace fs = boost::filesystem;
	fs::path inpath( infile_);
	fs::path filebase = inpath.stem();
	fs::path filepath = inpath.parent_path();
	infasta_ = (filepath/filebase).string() + ".fasta";	

	// Read fastq file - composed of four lines
	// Line 1: sequence ID
	// Line 2: nucleotide sequence
	// Line 3: sequence ID again
	// Line 4: quality string
	// If this is not a valid fastq file - throw an exception
	ofstream outfile( infasta_ );
	string line;
	int ii = 0;
	string sequenceID, sequence, qualityStr, sequenceID2;
	int query_no = 1;

	increment_( 0, num_queries_ );

	// Reset quality map if this is not the first time running
	quality_map_.clear();

	while ( getline (infile, line) ) {
		if ( ii == 0 ) sequenceID = line;
		else if ( ii == 1) sequence = line; 
		else if ( ii == 2) sequenceID2 = line;
		else if ( ii == 3) {
			qualityStr = line;

			// Sanity check to make sure FASTQ is valid
			// first line should start with @ and third line should start with +
			if ( sequenceID.substr( 0, 1 ) != "@" ||
				 sequenceID2.substr( 0, 1 ) != "+" ) {
				throw BadFileException( "File "+infile_+" could not be parsed as format "+format_+". Please check to make sure it's properly formed and try again" );
			} 
			vector<string> tokens = util::tokenize_string<string>( sequenceID, " \t" );
			sequenceID = tokens[0].substr(1, tokens[0].length());

			// if the quality map already has this sequence ID then there must be 
			// a duplicate ID. To address this I just append a "_n" to the end and
			// carry on
			string new_seq_id = sequenceID;
			int counter = 1;
			while ( quality_map_.find( new_seq_id ) != quality_map_.end() ) {
				new_seq_id = sequenceID + "_" + to_string( counter );
				counter++;
			}
			sequenceID = new_seq_id;

			outfile << ">" << sequenceID << "\n" << sequence << "\n";

			quality_map_[ sequenceID ] = qualityStr;

			ii = -1;

			++query_no;

			// if query_no is a multiple of 100, increment
			if ( query_no%100 == 0 ) {
				increment_( 100, num_queries_ );
			}

		}
		++ii;
	}

	// finish up progress bar if it was needed
	// if ( query_no >= 1000 ) {
	finish_();
	// }
	// reset_();

	outfile.close();
}

void ErrorXOptions::validate() {
	if ( infile_ == "" ) {
		throw invalid_argument("Error: infile not provided.");
	}

	if ( format_ == "" ) {
		throw invalid_argument("Error: file format not provided.");
	}
}

void ErrorXOptions::count_queries() {
	int no_lines = util::count_lines( infile_ );
	if ( format_ == "fastq" ) {
		// Sanity check to make sure FASTQ is valid
		// needs to be a multiple of four
		if ( no_lines%4 != 0 ) {
			throw BadFileException( "File "+infile_+" could not be parsed as format "+format_+". \n\nNumber of lines must be a multiple of 4. \n\nThis can be caused by an empty line at the end of your file. Please check to make sure it's properly formed and try again" );
		}

		num_queries_ = no_lines/4;

	} else if ( format_ == "tsv" ) {
		num_queries_ = no_lines;
	}
}

unordered_map<string,string> ErrorXOptions::quality_map() const { return quality_map_; }

string ErrorXOptions::get_quality( string const & sequenceID ) const {
	return quality_map_.at( sequenceID );
}

void ErrorXOptions::format( string const & format ) { 
	vector<string> valid_formats = {"tsv", "fastq"};

	if ( find( valid_formats.begin(), valid_formats.end(), format )
			== valid_formats.end() ) {
		string out_msg = "Error: invalid format. Format must be one of the following:\n";
		for ( int ii = 0; ii < valid_formats.size(); ++ii ) {
			out_msg += valid_formats[ii];
			out_msg += " ";
		}
		throw invalid_argument(out_msg);
	}

	format_ = format; 

}

void ErrorXOptions::species( string const & species ) { 
	vector<string> valid_species = {"human", "mouse"};

	if ( find( valid_species.begin(), valid_species.end(), species )
			== valid_species.end() ) {
		string out_msg = "Error: invalid species. Species must be one of the following:\n";
		for ( int ii = 0; ii < valid_species.size(); ++ii ) {
			out_msg += valid_species[ii];
			out_msg += " ";
		}
		throw invalid_argument(out_msg);
	}
	species_ = species; 
}

void ErrorXOptions::igtype( string const & igtype ) { 
	vector<string> valid_types = {"Ig", "TCR"};

	if ( find( valid_types.begin(), valid_types.end(), igtype )
			== valid_types.end() ) {
		string out_msg = "Error: invalid igtype. Igtype must be one of the following:\n";
		for ( int ii = 0; ii < valid_types.size(); ++ii ) {
			out_msg += valid_types[ii];
			out_msg += " ";
		}
		throw invalid_argument(out_msg);
	}
	igtype_ = igtype; 
}

void ErrorXOptions::nthreads( int const nthreads ) { 
	if ( nthreads == -1 ) nthreads_ = thread::hardware_concurrency();
	else if ( nthreads < 1) {
		throw invalid_argument("Error: nthreads must either be -1 or a positive integer");
	}
	else nthreads_ = nthreads;

}

void ErrorXOptions::increment( function<void(int,int)> const & increment ) {
	increment_ = increment;
}

void ErrorXOptions::reset( function<void(void)> const & reset ) {
	reset_ = reset;
}

void ErrorXOptions::finish( function<void(void)> const & finish ) {
	finish_ = finish;
}

void ErrorXOptions::message( function<void(string)> const & message ) {
	message_ = message;
}

string ErrorXOptions::outfile() const { return outfile_; }
string ErrorXOptions::format() const { return format_; }
string ErrorXOptions::species() const { return species_; }
string ErrorXOptions::igtype() const { return igtype_; }
string ErrorXOptions::infile() const { return infile_; }
string ErrorXOptions::infasta() const { return infasta_; }
string ErrorXOptions::igblast_output() const { return igblast_output_; }
string ErrorXOptions::errorx_base() const { return errorx_base_; }
int ErrorXOptions::verbose() const { return verbose_; }
int ErrorXOptions::nthreads() const { return nthreads_; }
double ErrorXOptions::error_threshold() const { return error_threshold_; }
char ErrorXOptions::correction() const { return correction_; }
bool ErrorXOptions::trial() const { return trial_; }
int ErrorXOptions::num_queries() const { return num_queries_; }
bool ErrorXOptions::allow_nonproductive() const { return allow_nonproductive_; }
function<void(int,int)> ErrorXOptions::increment() const { return increment_; }
function<void(void)> ErrorXOptions::reset() const { return reset_; }
function<void(void)> ErrorXOptions::finish() const { return finish_; }
function<void(string)> ErrorXOptions::message() const { return message_; }

void ErrorXOptions::outfile( string const & outfile ) { outfile_ = outfile; }
void ErrorXOptions::infile( string const & infile ) { infile_ = infile; }
void ErrorXOptions::infasta( string const & infasta ) { infasta_ = infasta; }
void ErrorXOptions::igblast_output( string const & igblast_output ) { igblast_output_ = igblast_output; }
void ErrorXOptions::errorx_base( string const & errorx_base ) { errorx_base_ = errorx_base; }
void ErrorXOptions::verbose( int const verbose ) { 
	verbose_ = verbose; 
	initialize_callback();
}
void ErrorXOptions::error_threshold( double const & error_threshold ) { error_threshold_ = error_threshold; }
void ErrorXOptions::correction( char const & correction ) { correction_ = correction; }
void ErrorXOptions::trial( bool const trial ) { trial_ = trial; }
void ErrorXOptions::num_queries( int const num_queries ) { num_queries_ = num_queries; }

void ErrorXOptions::allow_nonproductive( bool const allow_nonproductive ) { allow_nonproductive_ = allow_nonproductive; }
void ErrorXOptions::quality_map( unordered_map<string,string> const & quality_map ) {
	quality_map_ = quality_map;
}



} // namespace errorx
