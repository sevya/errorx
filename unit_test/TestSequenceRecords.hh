#ifndef UNITTESTS_HH_
#define UNITTESTS_HH_

#include <cxxtest/TestSuite.h>

#include <string>
#include <iostream>

#include "ErrorXOptions.hh"
#include "SequenceRecord.hh"
#include "SequenceRecords.hh"
#include "AbSequence.hh"
#include "ClonotypeGroup.hh"
#include "util.hh"

using namespace std;
using namespace errorx;

class TestSequenceRecords : public CxxTest::TestSuite
{
public:

	void setUp() {}

	void tearDown() {}

	void testSequenceGrouping() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("../");
		SequenceRecords* records = new SequenceRecords( options );
		vector<string> corrected_sequences = {
			"ACTGACTGACTGACTGACTGACTGACTGACTG",
			"ACTGACTGACTGTCTGACTGACTGACTGACTG",
			"ACTGACTGACTGACTGACTGACTGACTTACTG",
			"ACTGACTGACTGACTGACTGACTGACTGACTC",
			"ACTGACTGACTGACTGACTGANTGACTGACTG", // not unique
			"ACTGANTGACTGACTGACTGACTGACTGACTG", // not unique
			"NCTGACTGACTGACTGACTGACTGACTGACTG"  // not unique
		};

		vector<string> uncorrected_sequences = {
			"ACTGACTGACTGACTGACTGACTGACTGACTG",
			"ACTGACTGACTGTCTGACTGACTGACTGACTG",
			"ACTGACTGACTGACTGACTGACTGACTTACTG",
			"ACTGACTGACTGACTGACTGACTGACTGACTC",
			"ACTGACTGACTGACTGACTGATTGACTGACTG",
			"ACTGACTGACTGACTGACTGACTGACTGACTC", // not unique
			"ACTGACTGACTGACTGACTGACTGACTGACTG"  // not unique
		};

		for ( int ii = 0; ii < corrected_sequences.size(); ++ii ) {
			SequenceRecord* record = new SequenceRecord();
			record->full_nt_sequence( uncorrected_sequences[ ii ] );
			record->full_nt_sequence_corrected( corrected_sequences[ ii ] );
			records->add_record( record );
		}	
	
		records->count_sequences();
		TS_ASSERT_EQUALS( records->unique_corrected_nt_sequences(), 4 );
		TS_ASSERT_EQUALS( records->unique_nt_sequences(), 5 );
	}


	void testClonotypeEquality() {

		ClonotypeGroup a;
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		ClonotypeGroup b;
		b.v_gene( "IGHV3-23" ); 
		b.cdr3( "ARCASTFDVR" );
		b.j_gene( "IGHJ6" );

		ClonotypeGroup c;
		c.v_gene( "IGHV3-23" ); 
		c.cdr3( "ARCASXFDV" );
		c.j_gene( "IGHJ6" );

		TS_ASSERT( a != b );
		TS_ASSERT( a == c );
		TS_ASSERT( b != c );

		c.j_gene( "IGHJ2" );
		TS_ASSERT( a != c );
	}

	void testVectorInsertion() {
		ClonotypeGroup a;
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		ClonotypeGroup b;
		b.v_gene( "IGHV3-23" ); 
		b.cdr3( "ARCASTFDVR" );
		b.j_gene( "IGHJ6" );

		ClonotypeGroup c;
		c.v_gene( "IGHV3-23" ); 
		c.cdr3( "ARCASXFDV" );
		c.j_gene( "IGHJ6" );

		vector<ClonotypeGroup> vect;
		vector<ClonotypeGroup>::iterator it;

		it = find( vect.begin(), vect.end(), a );
		TS_ASSERT( it == vect.end() );
		vect.push_back( a );

		it = find( vect.begin(), vect.end(), b );
		TS_ASSERT( it == vect.end() );
		vect.push_back( b );

		it = find( vect.begin(), vect.end(), a );
		TS_ASSERT( it != vect.end() );
	}

	void testClonotypeGrouping() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("../");
		SequenceRecords* records = new SequenceRecords( options );
		SequenceRecord* record;

		record = new SequenceRecord();
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = new SequenceRecord();
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCAXTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = new SequenceRecord();
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCAXTFDVR" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = new SequenceRecord();
		record->v_gene( "IGHV3-21" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		vector<ClonotypeGroup> groups = records->get_clonotypes();
		for ( auto it = groups.begin();
			  it != groups.end(); ++it ) {
			cout << it->toString() << endl;
		}
		TS_ASSERT_EQUALS( groups.size(), 3 );
	}

};

#endif /* UNITTESTS_HH_ */

