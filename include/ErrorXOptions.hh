/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ErrorXOptions.hh
@brief Controls the options around running the ErrorX protocol
@details 
@author Alex Sevy (alex.sevy@gmail.com)
*/


#ifndef INCLUDE_ERRORXOPTIONS_HH_
#define INCLUDE_ERRORXOPTIONS_HH_

#if defined(_WIN32) || defined(_WIN64)
	#ifdef ERRORX_EXPORTS
		#define ERRORX_API __declspec(dllexport)
	#else
		#define ERRORX_API __declspec(dllimport)
	#endif
#else
	#define ERRORX_API 
#endif

#include <string>

using namespace std;

namespace errorx {
	
class ERRORX_API ErrorXOptions {
public:
	/**
		Empty constructor. Initializes to default values.
		Infile and format must be set before using the options though
	*/	
	ErrorXOptions();

	/**
		Constructor which initializes all to default values, except 
		for required arguments infile and format 
	*/	
	ErrorXOptions( string infile, string format );

	/**
		Copy constructor
	*/	
	ErrorXOptions( ErrorXOptions const & other );

	/**
		Converts a FASTQ file to FASTA file. Uses the infile_ variable
		to read, and outputs to the same name with the extension .fasta,
		which is then set to the variable infasta_
	*/	
	void fastq_to_fasta();

	/**
		Check that options are properly initialized before running. 

		@throws invalid_argument if either infile or format are not specified
	*/	
	void validate();
	
	/*
		Getters
	*/
	string outfile() const;
	string format() const;
	string species() const;
	string infile() const;
	string infasta() const;
	string igblast_output() const;
	string errorx_base() const;
	int verbose() const;
	int nthreads() const;
	double error_threshold() const;
	char correction() const;
	bool trial() const;
	bool allow_nonproductive() const;

	/*
		Setters
	*/
	void outfile( string const & outfile );
	void format( string const & format );
	void species( string const & species );
	void infile( string const & infile );
	void infasta( string const & infasta );
	void igblast_output( string const & igblast_output );
	void errorx_base( string const & errorx_base );
	// void errorx_base( boost::filesystem::path const & errorx_base );
	void verbose( int const & verbose );
	void nthreads( int const & nthreads );
	void error_threshold( double const & error_threshold );
	void correction( char const & correction );
	void trial( bool const & trial );
	void allow_nonproductive( bool const & allow_nonproductive );

private:
	/**
		User specified options:
		
		infile_: input file, either FASTQ or TSV, to use for prediction
		format_: format of infile_, either fastq or tsv
		outfile_: name of the output file to write the final ErrorX predictions. 
		Default out.tsv
		species_: species to use for germline assignment. Currently supported
		are human or mouse. Default human
		verbose_: Verbosity level: should ErrorX output extra warnings and messages? 
		0: don't output any message at all
		1: output progress during processing
		2: output progress and debugging messages
		nthreads_: how many threads to use? -1 for all available. Default -1
		error_threshold_: what probability is enough to call a base an error?
		Default 0.8
		allow_nonproductive_: allow sequences through the protocol even if they
		are nonproductive. Default no
		correction_: when ErrorX corrects a sequence it replaces the original base
		with a new character. What character should be used? Default N
	*/
	string infile_;
	string format_;
	string outfile_;
	string species_;
	int verbose_;
	int nthreads_;
	double error_threshold_;
	bool allow_nonproductive_;
	char correction_;

	/**
		Automatically generated options:
		These options are set internally to ErrorX based on where it's called 
		from and don't need to be changed.
	*/
	string infasta_;
	string igblast_output_;
	string errorx_base_;
	bool trial_;
	
};

} // namespace errorx

#endif /* INCLUDE_ERRORXOPTIONS_HH_ */
