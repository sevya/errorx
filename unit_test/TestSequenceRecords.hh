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
		SequenceRecordsPtr records = SequenceRecordsPtr(new SequenceRecords( options ));
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
			SequenceRecordPtr record = SequenceRecordPtr( new SequenceRecord());
			record->full_nt_sequence( uncorrected_sequences[ ii ] );
			record->full_nt_sequence_corrected( corrected_sequences[ ii ] );

			record->v_gene( "IGHV3-23" ); 
			record->cdr3_aa_sequence( "ARCASTFDV" );
			record->j_gene( "IGHJ6" );

			records->add_record( record );
		}	

		TS_ASSERT_EQUALS( records->unique_nt_sequences(1), 4 );
		TS_ASSERT_EQUALS( records->unique_nt_sequences(0), 5 );
	}


	void testClonotypeEquality() {
		ErrorXOptions options( "test.fastq", "fastq" );

		ClonotypeGroup a( options );
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		ClonotypeGroup b( options );
		b.v_gene( "IGHV3-23" ); 
		b.cdr3( "ARCASTFDVR" );
		b.j_gene( "IGHJ6" );

		ClonotypeGroup c( options );
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
		ErrorXOptions options( "test.fastq", "fastq" );

		ClonotypeGroup a( options );
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		ClonotypeGroup b( options );
		b.v_gene( "IGHV3-23" ); 
		b.cdr3( "ARCASTFDVR" );
		b.j_gene( "IGHJ6" );

		ClonotypeGroup c( options );
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
		SequenceRecordsPtr records = SequenceRecordsPtr( new SequenceRecords( options ));
		SequenceRecordPtr record;

		record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCAXTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCAXTFDVR" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-21" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		vector<ClonotypeGroup> groups = records->clonotypes();
		TS_ASSERT_EQUALS( groups.size(), 3 );

		// make sure CDR3 of first record with X AA is replaced
		// after adding new record
		TS_ASSERT_EQUALS( groups[0].cdr3(), "ARCASTFDV" );
		TS_ASSERT_EQUALS( groups[1].cdr3(), "ARCAXTFDVR" );
		TS_ASSERT_EQUALS( groups[2].cdr3(), "ARCASTFDV" );
	}

	void testSomaticVariants() {

		ErrorXOptions options( "test.fastq", "fastq" );

		ClonotypeGroup a( options );
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord());
		record->full_nt_sequence( "CTCTAGACTC" );
		record->full_nt_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record );

		SequenceRecordPtr record2 = SequenceRecordPtr(new SequenceRecord());
		record2->full_nt_sequence( "CTCTAGANTC" );
		record2->full_nt_sequence_corrected( "CTCTAGANTC" );
		a.add_record( record2 );

		SequenceRecordPtr record3 = SequenceRecordPtr(new SequenceRecord());
		record3->full_nt_sequence( "CTCCAGATTC" );
		record3->full_nt_sequence_corrected( "CTCTAGATTC" );
		a.add_record( record3 );

		SequenceRecordPtr record4 = SequenceRecordPtr(new SequenceRecord(*record));
		record4->full_nt_sequence( "CTCTAGACTC" );
		record4->full_nt_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record4 );

		TS_ASSERT_EQUALS( a.size(), 4 );
		TS_ASSERT_EQUALS( a.somatic_variants(0), 3 );
		TS_ASSERT_EQUALS( a.somatic_variants(1), 2 );
	}


		void testAASomaticVariants() {

		ErrorXOptions options( "test.fastq", "fastq" );

		ClonotypeGroup a( options );
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord());
		record->full_nt_sequence( "CTCTAGACTC" );
		record->full_nt_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record );

		SequenceRecordPtr record2 = SequenceRecordPtr(new SequenceRecord());
		record2->full_nt_sequence( "CTCTAGANTC" );
		record2->full_nt_sequence_corrected( "CTCTAGANTC" );
		a.add_record( record2 );

		SequenceRecordPtr record3 = SequenceRecordPtr(new SequenceRecord());
		record3->full_nt_sequence( "CTCCAGATTC" );
		record3->full_nt_sequence_corrected( "CTCTAGATTC" );
		a.add_record( record3 );

		SequenceRecordPtr record4 = SequenceRecordPtr(new SequenceRecord( *record ));
		record4->full_nt_sequence( "CTCTAGACTC" );
		record4->full_nt_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record4 );

		TS_ASSERT_EQUALS( a.size(), 4 );
		TS_ASSERT_EQUALS( a.somatic_variants(0), 3 );
		TS_ASSERT_EQUALS( a.somatic_variants(1), 2 );
	}

	void testSomaticVariantsRecords() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("../");
		SequenceRecordsPtr records = SequenceRecordsPtr( new SequenceRecords( options ));

		SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCAXTFDV" );
		record->j_gene( "IGHJ6" );
		record->full_nt_sequence( "CTCTAGACTC" );
		record->full_nt_sequence_corrected( "CTCTAGACTC" );
		records->add_record( record );

		SequenceRecordPtr record2 = SequenceRecordPtr(new SequenceRecord());
		record2->v_gene( "IGHV3-23" ); 
		record2->cdr3_aa_sequence( "ARCASTFDV" );
		record2->j_gene( "IGHJ6" );
		record2->full_nt_sequence( "CTCTAGACTC" );
		record2->full_nt_sequence_corrected( "CTCTANACTC" );
		records->add_record( record2 );

		vector<ClonotypeGroup> groups = records->clonotypes();
		TS_ASSERT_EQUALS( groups.size(), 1 );
		TS_ASSERT_EQUALS( groups[0].somatic_variants(0), 1 );
		TS_ASSERT_EQUALS( groups[0].somatic_variants(1), 1 );


		records = SequenceRecordsPtr( new SequenceRecords( options ));
		record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		record->full_nt_sequence( "CTCTAGACTC" );
		record->full_nt_sequence_corrected( "CTCTAGACTC" );
		records->add_record( record );

		record2 = SequenceRecordPtr(new SequenceRecord( *record ));
		record2->full_nt_sequence( "CTCTAGANTC" );
		record2->full_nt_sequence_corrected( "CTCTAGANTC" );
		records->add_record( record2 );

		SequenceRecordPtr record3 = SequenceRecordPtr(new SequenceRecord( *record ));
		record3->full_nt_sequence( "CTCCAGATTC" );
		record3->full_nt_sequence_corrected( "CTCTAGATTC" );
		records->add_record( record3 );

		SequenceRecordPtr record4 = SequenceRecordPtr(new SequenceRecord( *record ));
		record4->full_nt_sequence( "CTCTAGACTC" );
		record4->full_nt_sequence_corrected( "CTCTAGACTC" );
		records->add_record( record4 );

		groups = records->clonotypes();
		TS_ASSERT_EQUALS( groups.size(), 1 );

		TS_ASSERT_EQUALS( groups[0].somatic_variants(0), 3 );
		TS_ASSERT_EQUALS( groups[0].somatic_variants(1), 2 );
	}


	void testVJCounts() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("../");
		SequenceRecordsPtr records = SequenceRecordsPtr( new SequenceRecords( options ));

		SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord());
		record->v_gene( "IGHV3-23" ); 
		record->cdr3_aa_sequence( "ARCASTFDV" );
		record->j_gene( "IGHJ6" );
		records->add_record( record );

		SequenceRecordPtr record2 = SequenceRecordPtr( new SequenceRecord( *record ));
		record->cdr3_aa_sequence( "ARCASTFDW" );
		records->add_record( record2 );

		SequenceRecordPtr record3 = SequenceRecordPtr(new SequenceRecord( *record ));
		record3->v_gene( "IGHV3-21" ); 
		records->add_record( record3 );

		SequenceRecordPtr record4 = SequenceRecordPtr(new SequenceRecord( *record ));
		record4->j_gene( "IGHJ3" ); 
		records->add_record( record4 );

		SequenceRecordPtr record5 = SequenceRecordPtr(new SequenceRecord( *record ));
		record5->j_gene( "IGHJ1" ); 
		records->add_record( record5 );

		map<string,int> counts;

		counts = records->vgene_counts();
		TS_ASSERT_EQUALS( counts.size(), 2 );
		TS_ASSERT_EQUALS( counts["IGHV3-23"], 4 );
		TS_ASSERT_EQUALS( counts["IGHV3-21"], 1 );

		counts = records->jgene_counts();
		TS_ASSERT_EQUALS( counts.size(), 3 );
		TS_ASSERT_EQUALS( counts["IGHJ6"], 3 );
		TS_ASSERT_EQUALS( counts["IGHJ3"], 1 );
		TS_ASSERT_EQUALS( counts["IGHJ1"], 1 );

		counts = records->vjgene_counts();
		TS_ASSERT_EQUALS( counts.size(), 4 );
		TS_ASSERT_EQUALS( counts["IGHV3-23_IGHJ6"], 2 );
		TS_ASSERT_EQUALS( counts["IGHV3-21_IGHJ6"], 1 );
		TS_ASSERT_EQUALS( counts["IGHV3-23_IGHJ3"], 1 );
		TS_ASSERT_EQUALS( counts["IGHV3-23_IGHJ1"], 1 );
	}
};

#endif /* UNITTESTS_HH_ */

