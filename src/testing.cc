/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file main.cc
@brief
@details
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <stdlib.h> 

#include <thread>
#include <iostream>
#include <exception>

#include "errorx.hh"
#include "IGBlastParser.hh"
#include "ErrorPredictor.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"
#include "constants.hh"
#include "exceptions.hh"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace errorx;

int main( int argc, char* argv[] ) {
	using namespace boost;

	// Declare the supported options.
	program_options::options_description desc("Usage: errorx --format fastq --out out.tsv --species human --nthreads 4 myfile.fastq\n"
			"Allowed options");

	desc.add_options()
	    ("help,h", "produce help message")
		("format,f", program_options::value<string>(), "input file format. Valid entries are fastq or tsv.")
		("out,o", program_options::value<string>()->default_value("out.tsv"), "output file (Default=out.tsv)")
		("species,s", program_options::value<string>()->default_value("human"), "Species for IGBLAST search. Valid entries are human or mouse. (Default=human)")
		("nthreads,n", program_options::value<int>()->default_value(-1), "Number of threads to use during execution. Enter -1 to use all available (Default=-1)")
		("error-threshold,e", program_options::value<double>()->default_value(constants::OPTIMIZED_THRESHOLD,to_string(constants::OPTIMIZED_THRESHOLD)), "Probability cutoff for a base to be considered an error. "
				"Higher=more stringent in calling errors. Don't change this value unless you know what you are doing.")
		("infile", program_options::value<vector<string>>(), "input file")
		("version", "Print ErrorX version information and exit")
		("verbose,v", program_options::value<int>()->default_value(1), 
		"Verbosity level: should ErrorX output extra warnings and messages?\n"
		"0: don't output any message at all\n"
		"1: output progress during processing\n"
		"2: output progress and debugging messages\n"
		"(default=1)")
		("allow-nonproductive", program_options::bool_switch()->default_value(false), "Allow nonproductive and out-of-frame sequences to be included? (default=No)")
		("license", program_options::value<string>(), "License key to activate full version of ErrorX")
		;


	program_options::positional_options_description positional;
	positional.add("infile", -1);

	program_options::variables_map vm;
	try {
		program_options::store(program_options::command_line_parser(argc, argv).
				options(desc).positional(positional).run(), vm);
		program_options::notify(vm);

		if ( vm.count("help") or argc == 1 ) {
			cout << desc << "\n";
			return 1;
		}

		if ( vm.count("version")) {
			cout << "ErrorX v1.0 by EndeavorBio. Creator: Alex Sevy, alex@endeavorbio.com" << "\n";
			return 1;
		}

		if ( vm.count("license")) {
			util::write_license( vm["license"].as<string>() );
			cout << "Successfully wrote license file!" << endl;
			return 0;
		}

	// try {
		ErrorXOptions options;

		if ( vm.count("infile") ) {
			vector<string> infiles = vm["infile"].as<vector<string>>();
			if ( infiles.size() > 1 ) {
				cout << "Warning: " << infiles.size() << " files were passed, but only the first will be used" << endl;
			}
			options.infile( infiles[0] );
		} else {
			cout << "Error - please enter an input file to analyze." << endl;
			return 1;
		}

		if ( vm.count("format") ) {
			options.format( vm["format"].as<string>());
		} else {
			cout << "Error - you must enter the input file format." << endl;
			return 1;
		}

		options.outfile( vm["out"].as<string>());

		options.species( vm["species"].as<string>());

		options.nthreads( vm["nthreads"].as<int>());

		options.error_threshold( vm["error-threshold"].as<double>());

		options.verbose( vm["verbose"].as<int>());

		options.allow_nonproductive( vm["allow-nonproductive"].as<bool>());

		run_protocol_write_features( options );
		return 0;
	} catch ( program_options::unknown_option & exc) {
		cout << "Error: "<< exc.what() << endl;
		return 1;
	} catch ( InvalidLicenseException & exc ) {
		cout << exc.what() << endl;
		return 1;
	} catch ( std::exception & e ) {
		// cout << "Exception encountered..." << endl;
		cout << e.what() << endl;
		return 1;
	}
}








