/// @file main.cc
/// @brief
/// @details
/// @author Alex Sevy (alex.sevy@gmail.com)

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
		("error-threshold,e", program_options::value<double>()->default_value(util::OPTIMIZED_THRESHOLD,to_string(util::OPTIMIZED_THRESHOLD)), "Probability cutoff for a base to be considered an error. "
				"Higher=more stringent in calling errors. Don't change this value unless you know what you are doing.")
		("infile", program_options::value<vector<string>>(), "input file")
		("verbose,v", program_options::value<int>()->default_value(1), 
		"Verbosity level: should ErrorX output extra warnings and messages?\n"
		"0: don't output any message at all\n"
		"1: output progress during processing\n"
		"2: output progress and debugging messages\n"
		"(default=1)")
		;


	program_options::positional_options_description positional;
	positional.add("infile", -1);

	program_options::variables_map vm;

	program_options::store(program_options::command_line_parser(argc, argv).
			  options(desc).positional(positional).run(), vm);
	program_options::notify(vm);

	if ( vm.count("help") or argc == 1 ) {
		cout << desc << "\n";
		return 0;
	}

	try {
		string infile;
		if ( vm.count("infile") ) {
			vector<string> infiles = vm["infile"].as<vector<string>>();
			if ( infiles.size() > 1 ) {
				cout << "Warning: " << infiles.size() << " files were passed, but only the first will be used" << endl;
			}
			infile = infiles[0];
		} else {
			cout << "Error - please enter an input file to analyze." << endl;
			return 0;
		}

		string format;
		if ( vm.count("format") ) {
			format = vm["format"].as<string>();
			if ( format != "fastq" and format != "tsv" ) {
				cout << "Error: input format must be either fastq or tsv" << endl;
				return 0;
			}
		} else {
			cout << "Error - you must enter the input file format." << endl;
			return 0;
		}

		ErrorXOptions options( infile, format );

		options.outfile( vm["out"].as<string>() );

		options.species( vm["species"].as<string>() );

		options.nthreads( vm["nthreads"].as<int>() );

		options.error_threshold( vm["error-threshold"].as<double>() );

		options.verbose( vm["verbose"].as<int>());

		run_protocol_write_features( options );
		return 1;
	} catch ( std::exception & e ) {
		// cout << "Exception encountered..." << endl;
		cout << e.what() << endl;
		return 0;
	}
}






