#include "errorx.hh"
#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"

#include <iostream>

int main() {
	errorx::ErrorXOptions options( "test.tsv", "tsv" );
	options.outfile( "test_out.tsv" );
	options.species( "human" );

	errorx::run_protocol_write( options );

	errorx::SequenceRecords* records = errorx::run_protocol( options );
	std::cout << "Original NT sequence: " << records->get( 0 )->full_nt_sequence() << std::endl;
	std::cout << "Corrected NT sequence: " << records->get( 0 )->full_nt_sequence_corrected() << std::endl;
	records->print_summary();
}
