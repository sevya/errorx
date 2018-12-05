/**
@file errorx.cc
@brief Interface to ErrorX error correction
@details Public interface to ErrorX processing. Input 
sequences can be given as a FASTQ file, a TSV file,
or a std::vector of SequenceQuery objects
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "errorx.hh"
#include "SequenceQuery.hh"
#include "IGBlastParser.hh"
#include "ErrorPredictor.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"
#include "exceptions.hh"

#include <boost/program_options.hpp>

using namespace std;

namespace errorx {

SequenceRecords* run_protocol( ErrorXOptions & options ) {
	SequenceRecords* records;

	options.validate();
	options.trial( !util::valid_license() );	
	if ( options.format() == "fastq" ) {
		if ( options.trial() ) {
			// Trial version only allows querying 500 sequences
			// since each FASTQ query is 4 lines check and make
			// sure there are < 2000 lines
			string infile = options.infile();
			int num_lines = util::count_lines( infile );
			if ( num_lines > 2000 ) {
				throw InvalidLicenseException();
			} 
		}

		// Convert FASTQ to FASTA
		if ( options.verbose() > 0 ) {
			cout << "Converting fastq to fasta..." << endl;
		}
		options.fastq_to_fasta();

		// Run IGBlast query on FASTA file
		IGBlastParser parser;
		parser.blast( options );

		// Parse the IGBlast output
		records = parser.parse_output( options );

	} else {
		if ( options.trial() ) {
			// Trial version only allows querying 500 sequences
			string infile = options.infile();
			int num_lines = util::count_lines( infile );
			if ( num_lines > 500 ) {
				cout << 
				"You are currently running the trial version of ErrorX, "
				"which can only process 500 sequences at a time. "
				"Please limit your query size or request access to the full version."
				<< endl;
				exit(0);
			} 
		}

		// Parse the TSV file
		// TSV files are in the following format: sequenceID,nt_sequence,gl_sequence,quality_string
		records = new SequenceRecords( options );
		records->import_from_tsv();
	}

	// Predict errors from SequenceRecords
	SequenceRecords::correct_sequences( records );
	return records;
}


SequenceRecords* run_protocol( vector<SequenceQuery> & queries, 
							   ErrorXOptions & options ) {

	options.trial( !util::valid_license() );

	if ( options.trial() ) {
		// Trial version only allows querying 500 sequences
		if ( queries.size() > 500 ) {
			throw InvalidLicenseException();
		} 
	}

	SequenceRecords* records;

	options.validate();
	
	records = new SequenceRecords( options );

	records->import_from_list( queries );

	// Predict errors from SequenceRecords
	SequenceRecords::correct_sequences( records );
	return records;
}


void run_protocol_write( ErrorXOptions & options ) {
	SequenceRecords* records = run_protocol( options );
	records->write_summary();
}

void run_protocol_write_features( ErrorXOptions & options ) {
	SequenceRecords* records;
	options.trial( !util::valid_license() );
	options.validate();

	if ( options.format() == "fastq" ) {
		if ( options.trial() ) {
			// Trial version only allows querying 500 sequences
			// since each FASTQ query is 4 lines check and make
			// sure there are < 2000 lines
			string infile = options.infile();
			int num_lines = util::count_lines( infile );
			if ( num_lines > 2000 ) {
				throw InvalidLicenseException();
			} 
		}

		// Convert FASTQ to FASTA
		if ( options.verbose() > 0 ) {
			cout << "Converting fastq to fasta..." << endl;
		}
		options.fastq_to_fasta();

		// Run IGBlast query on FASTA file
		IGBlastParser parser;
		parser.blast( options );

		// Parse the IGBlast output
		records = parser.parse_output( options );

	} else {
		if ( options.trial() ) {
			// Trial version only allows querying 500 sequences
			string infile = options.infile();
			int num_lines = util::count_lines( infile );
			if ( num_lines > 500 ) {
				throw InvalidLicenseException();
			} 
		}

		// Parse the TSV file
		// TSV files are in the following format: sequenceID,nt_sequence,gl_sequence,quality_string
		records = new SequenceRecords( options );
		records->import_from_tsv();
	}

	// Write features
	records->write_features();

	
}

} // namespace errorx
