#include "errorx.hh"
#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"

#include <iostream>

int main() {
	errorx::ErrorXOptions options( "input_files/test.tsv", "tsv" );
	options.outfile( "new/linking_tsv_out.tsv" );
	options.species( "mouse" );
	options.errorx_base( "../" );
	options.verbose( 0 );

	errorx::run_protocol_write( options );

	options.infile( "input_files/test.fastq" );
	options.format( "fastq" );
    options.outfile( "new/linking_fastq_out.tsv" );
    options.species( "mouse" );

    errorx::run_protocol_write( options );
}
