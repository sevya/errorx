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

#include "ErrorXOptions.hh"
#include "util.hh"
#include "constants.hh"

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
	trial_(0)
{
	nthreads(-1);
	errorx_base( util::get_root_path().string() );

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
	trial_(0)
{
	nthreads(-1);
	format( file_format );
	infile( input_file );
	errorx_base( util::get_root_path().string() );
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
	trial_(other.trial_)
{}


void ErrorXOptions::fastq_to_fasta() {
	ios_base::sync_with_stdio (false);


	ifstream infile ( infile_ );
	if ( !infile.good() ) {
		throw invalid_argument("Error: file " + infile_ + " does not exist." );
	}


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
	ofstream outfile( infasta_ );
	string line;
	int ii = 0;
	string sequenceID, sequence, qualityStr;

	while (getline (infile, line)) {
		if ( ii == 0 ) sequenceID = line;
		else if ( ii == 1) sequence = line; 
		// else if ( ii == 2) do nothing
		else if ( ii == 3) {
			qualityStr = line;
			vector<string> tokens = util::tokenize_string<string>( sequenceID, " \t" );
			sequenceID = tokens[0].substr(1, tokens[0].length());

			outfile << ">" << sequenceID << "|" << qualityStr<< "\n" << sequence << "\n";

			ii = -1;
		}
		++ii;
	}

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

void ErrorXOptions::nthreads( int const & nthreads ) { 
	if ( nthreads == -1 ) nthreads_ = thread::hardware_concurrency();
	else if ( nthreads < 1) {
		throw invalid_argument("Error: nthreads must either be -1 or a positive integer");
	}
	else nthreads_ = nthreads;

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
bool ErrorXOptions::allow_nonproductive() const { return allow_nonproductive_; }

void ErrorXOptions::outfile( string const & outfile ) { outfile_ = outfile; }
void ErrorXOptions::infile( string const & infile ) { infile_ = infile; }
void ErrorXOptions::infasta( string const & infasta ) { infasta_ = infasta; }
void ErrorXOptions::igblast_output( string const & igblast_output ) { igblast_output_ = igblast_output; }
void ErrorXOptions::errorx_base( string const & errorx_base ) { errorx_base_ = errorx_base; }
void ErrorXOptions::verbose( int const & verbose ) { verbose_ = verbose; }
void ErrorXOptions::error_threshold( double const & error_threshold ) { error_threshold_ = error_threshold; }
void ErrorXOptions::correction( char const & correction ) { correction_ = correction; }
void ErrorXOptions::trial( bool const & trial ) { trial_ = trial; }
void ErrorXOptions::allow_nonproductive( bool const & allow_nonproductive ) { allow_nonproductive_ = allow_nonproductive; }



} // namespace errorx
