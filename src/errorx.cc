/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file errorx.cc
@brief Interface to ErrorX error correction
@details Public interface to ErrorX processing. Input 
sequences can be given as a FASTQ file, a TSV file,
or a std::vector of SequenceQuery objects
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "errorx.hh"
#include "SequenceQuery.hh"
#include "IGBlastParser.hh"
#include "ErrorPredictor.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"
#include "exceptions.hh"

using namespace std;

namespace errorx {

SequenceRecordsPtr run_protocol( ErrorXOptions & options ) {
	SequenceRecordsPtr records;

	// Register control-C signal
	util::register_signal();

	options.validate();
	options.trial( !util::valid_license() );	
	options.count_queries();

	// Trial version only allows querying a limited number of sequences
	if ( options.trial() && 
		 options.num_queries() > constants::FREE_QUERIES ) {
		throw InvalidLicenseException();
	}

	if ( options.format() == "fastq" ) {

		// Convert FASTQ to FASTA
		options.fastq_to_fasta();

		// Run IGBlast query on FASTA file
		IGBlastParser parser;
		parser.blast( options );
		// Parse the IGBlast output
		records = parser.parse_output( options );
	} else if ( options.format() == "fasta" ) {
		// Run with FASTA file
		// Set infasta here - normally it would be set by fastq_to_fasta
		options.infasta( options.infile() );
		IGBlastParser parser;
		parser.blast( options );

		// Parse the IGBlast output
		records = parser.parse_output( options );

		// Do a "mock" error correction
		// just put the regular sequence in place of the corrected sequence
		records->mock_correct_sequences();

		// Return here so that the records won't be error corrected
		return records;
	} else {

		// Parse the TSV file
		// TSV files are in the following format: sequenceID,nt_sequence,gl_sequence,quality_string
		records = SequenceRecordsPtr( new SequenceRecords( options ));
		records->import_from_tsv();
	}

	// Predict errors from SequenceRecords as long as it's not a FASTA file
	SequenceRecords::correct_sequences( records );
	return records;
}


SequenceRecordsPtr run_protocol( vector<SequenceQuery> & queries, 
							   ErrorXOptions & options ) {

	// Register control-C signal
	util::register_signal();

	options.validate();
	options.trial( !util::valid_license() );
	options.count_queries();

	// Trial version only allows querying a limited number of sequences
	if ( options.trial() && 
		 options.num_queries() > constants::FREE_QUERIES ) {
		throw InvalidLicenseException();
	}

	SequenceRecordsPtr records;

	
	records = SequenceRecordsPtr( new SequenceRecords( options ));

	records->import_from_list( queries );

	// Predict errors from SequenceRecords
	SequenceRecords::correct_sequences( records );
	return records;
}


void run_protocol_write( ErrorXOptions & options ) {
	SequenceRecordsPtr records = run_protocol( options );
	records->write_summary();
	records.release();
}

void run_protocol_write_features( ErrorXOptions & options ) {
	SequenceRecordsPtr records = run_protocol( options );

	// Write summary
	records->write_summary();
	// Write features
	records->write_features();

	records.release();
}

} // namespace errorx
