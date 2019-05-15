#include "errorx.hh"
#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"

#include <string>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

int main() {

	using namespace errorx;

	ErrorXOptions options( "/Users/alexsevy/Documents/SRR3174992_pairedend_100.fastq", "fastq" );
    options.outfile( "~/pollution.tsv" );
    options.species( "mouse" );

    SequenceRecords* records = errorx::run_protocol( options );
    records->count_sequences();

    cout << "total parsed clones: " << records->good_records() << endl;
    cout << "unique sequences: " << records->unique_nt_sequences() << endl;
    cout << "unique corrected sequences: " << records->unique_corrected_nt_sequences() << endl;
    cout << "unique aa sequences: " << records->unique_aa_sequences() << endl;
    cout << "unique clonotypes: " << records->unique_clonotypes() << endl;
    return 1;
}
