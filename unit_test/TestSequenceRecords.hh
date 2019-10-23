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
#include "errorx.hh"

using namespace std;
using namespace errorx;

class TestSequenceRecords : public CxxTest::TestSuite
{
public:

	void setUp() {}

	void tearDown() {}

	void testSequenceGrouping() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("..");
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

	void testSequenceGroupingNoClonotype() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("..");
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

			// record->v_gene( "IGHV3-23" ); 
			// record->cdr3_aa_sequence( "ARCASTFDV" );
			// record->j_gene( "IGHJ6" );

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

	void testEquality() {

		ErrorXOptions options( "testing/test_sequences.fastq", "fastq" );
		options.errorx_base("..");
		SequenceRecordsPtr records_one = run_protocol( options );
		SequenceRecordsPtr records_two( new SequenceRecords( *records_one ));
		
		// check that objects are equal after copying, but not pointers
		TS_ASSERT( records_one != records_two );
		TS_ASSERT( (*records_one)==(*records_two) );
		TS_ASSERT( records_one->equals( records_two ));

		SequenceRecordsPtr records_three = run_protocol( options );

		TS_ASSERT( records_one != records_three );
		TS_ASSERT( (*records_one)==(*records_three) );
		TS_ASSERT( records_one->equals( records_three ));

		SequenceQuery query( "SRR3175015.933", "TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG", "TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG", "###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4," );

		SequenceRecordPtr new_record( new SequenceRecord( query ));
		records_three->add_record( new_record );

		TS_ASSERT( (*records_one)!=(*records_three) );
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
		options.errorx_base("..");
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

	void testSomaticVariants2() {

		ErrorXOptions options( "testing/variant_test.fasta", "fasta" );
		options.errorx_base( ".." );
		options.allow_nonproductive( 1 );

		SequenceRecordsPtr records = run_protocol( options );

		vector<ClonotypeGroup> clonotypes = records->clonotypes();

		TS_ASSERT_EQUALS( clonotypes.size(), 5 );

		// Sort clonotypes by # members
		sort( clonotypes.begin(), clonotypes.end(),
			[]( ClonotypeGroup const & a, ClonotypeGroup const & b ) -> bool
		{
			return a.size() > b.size();
		});

		vector<ClonotypeGroup>::const_iterator it = clonotypes.begin();
		SequenceRecordPtr current;

		for ( int ii = 0; ii < it->size(); ++ii ) {
			current = it->get( ii );
			current->full_nt_sequence_corrected( current->full_nt_sequence() );
		}

		TS_ASSERT_EQUALS( it->size(), 7 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/0 ), 5 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/1 ), 5 );

		it++;
		TS_ASSERT_EQUALS( it->size(), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/1 ), 1 );

		it++;
		TS_ASSERT_EQUALS( it->size(), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/1 ), 1 );

		it++;
		TS_ASSERT_EQUALS( it->size(), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/1 ), 1 );

		it++;
		TS_ASSERT_EQUALS( it->size(), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( it->somatic_variants( /*corrected=*/1 ), 1 );
	}


	void testUniqueSequences() {
		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("..");

		AbSequence seq;
		seq.v_gene( "IGHV3-23" ); 
		seq.cdr3_aa_sequence( "ARCASTFDV" );
		seq.j_gene( "IGHJ6" );
		seq.sequenceID( "1" );
		seq.quality_string_trimmed( "GHGHGHGHGHGHGHHGHGHGHGHGH" );
		seq.full_nt_sequence( "CGGAGGACACTGCCATGTATTACTG" );
		seq.full_gl_nt_sequence( "CGGAGGACACTGCCATGTATTACTG" );	
		seq.full_nt_sequence_corrected( "CGGAGGACACTGCCATGTATTACTG" );
		seq.full_aa_sequence( "MFATWIAC" );
		seq.full_aa_sequence_corrected( "MFATWIAC" );

		SequenceRecordsPtr records = SequenceRecordsPtr(new SequenceRecords(options));

		for ( int ii = 0; ii < 8; ++ii ) {
			AbSequence newseq( seq );
			SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord(newseq));
			records->add_record( record );
		}

		records->count_clonotypes();
		TS_ASSERT_EQUALS( records->unique_nt_sequences( /*int corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( records->unique_nt_sequences( /*int corrected=*/1 ), 1 );

		TS_ASSERT_EQUALS( records->unique_aa_sequences( /*int corrected=*/0 ), 1 );
		TS_ASSERT_EQUALS( records->unique_aa_sequences( /*int corrected=*/1 ), 1 );
	}


	void testAASomaticVariants() {

		ErrorXOptions options( "test.fastq", "fastq" );

		ClonotypeGroup a( options );
		a.v_gene( "IGHV3-23" ); 
		a.cdr3( "ARCASTFDV" );
		a.j_gene( "IGHJ6" );

		SequenceRecordPtr record = SequenceRecordPtr(new SequenceRecord());
		record->full_aa_sequence( "CTCTAGACTC" );
		record->full_aa_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record );

		SequenceRecordPtr record2 = SequenceRecordPtr(new SequenceRecord());
		record2->full_aa_sequence( "CTCTAGANTC" );
		record2->full_aa_sequence_corrected( "CTCTAGANTC" );
		a.add_record( record2 );

		SequenceRecordPtr record3 = SequenceRecordPtr(new SequenceRecord());
		record3->full_aa_sequence( "CTCCAGATTC" );
		record3->full_aa_sequence_corrected( "CTCTAGATTC" );
		a.add_record( record3 );

		SequenceRecordPtr record4 = SequenceRecordPtr(new SequenceRecord( *record ));
		record4->full_aa_sequence( "CTCTAGACTC" );
		record4->full_aa_sequence_corrected( "CTCTAGACTC" );
		a.add_record( record4 );

		TS_ASSERT_EQUALS( a.size(), 4 );
		TS_ASSERT_EQUALS( a.somatic_variants_aa(0), 3 );
		TS_ASSERT_EQUALS( a.somatic_variants_aa(1), 2 );
	}

	void testSomaticVariantsRecords() {

		ErrorXOptions options( "test.fastq", "fastq" );
		options.errorx_base("..");
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
		options.errorx_base("..");
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
	
	void testProductivityAssignment() {
		ErrorXOptions options( "testing/productivity.fastq", "fastq" );
		options.errorx_base( ".." );
		options.verbose( 0 );
		SequenceRecordsPtr records = run_protocol( options );
		
		// short sequence
		TS_ASSERT( records->get( 0 )->productive() );
		// no match - non Ig sequence
		TS_ASSERT( !records->get( 1 )->productive() );
		// stop codon present
		TS_ASSERT( !records->get( 2 )->productive() );
		// regular sequence
		TS_ASSERT( records->get( 3 )->productive() );
		// bad D assignment
		TS_ASSERT( records->get( 4 )->productive() );
		// bad J assignment
		TS_ASSERT( records->get( 5 )->productive() );
		// no J found
		TS_ASSERT( records->get( 6 )->productive() );
		// no D and no J assigned
		TS_ASSERT( records->get( 7 )->productive() );
		// late start to assignment
		TS_ASSERT( records->get( 8 )->productive() );
		// only V gene present
		TS_ASSERT( records->get( 9 )->productive() );
		// bad V assignment + stop codon
		TS_ASSERT( !records->get( 10 )->productive() );
		// irrelevant sequence
		TS_ASSERT( !records->get( 11 )->productive() );
	}

	void testVJCounts100() {
		ErrorXOptions options( "testing/100.fastq", "fastq" );
		options.allow_nonproductive( 1 );
		options.verbose( 0 );
		options.errorx_base( ".." );
		SequenceRecordsPtr records = run_protocol( options );
	
		int good_records = records->good_records();
		
		map<string,int> v = records->vgene_counts();
		map<string,int>::const_iterator it;
		int sum = 0;	
		for ( it = v.begin(); it != v.end(); ++it ) sum += it->second;
		
		TS_ASSERT_EQUALS( good_records, sum );

		sum = 0;
		map<string,int> j = records->jgene_counts();
		for ( it = j.begin(); it != j.end(); ++it ) sum += it->second;
		
		TS_ASSERT_EQUALS( good_records, sum );

		options.allow_nonproductive( 0 );
		records = run_protocol( options );
		good_records = records->good_records();
		
		v = records->vgene_counts();
		sum = 0;
		for ( it = v.begin(); it != v.end(); ++it ) sum += it->second;

		TS_ASSERT_EQUALS( good_records, sum );

		sum = 0;
		j = records->jgene_counts();
		for ( it = j.begin(); it != j.end(); ++it ) sum += it->second;

		TS_ASSERT_EQUALS( good_records, sum );
	}

	 void testCDRLengths() {
		ErrorXOptions options( "testing/100.fastq", "fastq" );
		options.allow_nonproductive( 1 );
		options.verbose( 0 );
		options.errorx_base( ".." );
		SequenceRecordsPtr records = run_protocol( options );


		// Set up the correct values for each of these loops
		map<string,vector<int>> correct_lengths;
		
		correct_lengths[ "CDR1" ] = 
vector<int>({8,8,8,8,8,7,8,8,8,8,9,8,8,8,8,8,8,8,8,8,8,6,8,8,5,8,8,8,8,8,8,8,8,8,7,7,8,8,8,8,8,5,8,8,5,8,8,8,8,1,8,8,8,8,8,5,8,8,8,8,8,8,8,8,8,8,8,8,8,1,8,5,8} );
		
		correct_lengths[ "CDR2" ] = 
vector<int>({5,7,8,7,5,8,11,8,8,8,7,8,8,8,8,7,8,8,7,7,7,7,8,7,8,8,7,8,2,8,8,8,8,7,8,7,7,7,7,7,7,8,8,8,7,8,8,8,7,7,8,8,7,8,8,10,8,6,7,9,8,8,8,8,8,8,8,7,6,8,8,7,7} );

		correct_lengths[ "CDR3" ] =
vector<int>({19,17,8,9,23,8,8,17,10,6,13,12,11,31,8,9,15,24,31,13,9,2,19,11,10,30,10,35,29,6,12,9,32,9,9,29,3,9,9,14,9,28,26,5});


		map<string,vector<int>> lengths = records->cdr_lengths();
		
		for ( string cdr : vector<string>( { "CDR1", "CDR2", "CDR3" })) {
			TS_ASSERT_EQUALS( lengths[ cdr ], correct_lengths[ cdr ] );
		}


		map<string,map<int,float>> bins_raw;
		map<string,map<int,float>> bins_norm;

		bins_raw[ "CDR1" ] = { make_pair(1,2),
			make_pair(2,0),
			make_pair(3,0),
			make_pair(4,0),
			make_pair(5,5),
			make_pair(6,1),
			make_pair(7,3),
			make_pair(8,61),
			make_pair(9,1) };

		bins_norm[ "CDR1" ] = { make_pair(1,0.02739726),
			make_pair(2,0),
			make_pair(3,0),
			make_pair(4,0),
			make_pair(5,0.068493151),
			make_pair(6,0.01369863),
			make_pair(7,0.04109589),
			make_pair(8,0.835616438),
			make_pair(9,0.01369863) };

		bins_raw[ "CDR2" ] = { make_pair(2,1),
			make_pair(3,0),
			make_pair(4,0),
			make_pair(5,2),
			make_pair(6,2),
			make_pair(7,25),
			make_pair(8,40),
			make_pair(9,1),
			make_pair(10,1),
			make_pair(11,1)
		};

		bins_norm[ "CDR2" ] = { 
			make_pair(2,0.0136986301369863),
			make_pair(3,0),
			make_pair(4,0),
			make_pair(5,0.0273972602739726),
			make_pair(6,0.0273972602739726),
			make_pair(7,0.342465753424658),
			make_pair(8,0.547945205479452),
			make_pair(9,0.0136986301369863),
			make_pair(10,0.0136986301369863),
			make_pair(11,0.0136986301369863)	
		};

		bins_raw[ "CDR3" ] = {
			make_pair(2,1),
			make_pair(3,1),
			make_pair(4,0),
			make_pair(5,1),
			make_pair(6,2),
			make_pair(7,0),
			make_pair(8,4),
			make_pair(9,9),
			make_pair(10,3),
			make_pair(11,2),
			make_pair(12,2),
			make_pair(13,2),
			make_pair(14,1),
			make_pair(15,1),
			make_pair(16,0),
			make_pair(17,2),
			make_pair(18,0),
			make_pair(19,2),
			make_pair(20,0),
			make_pair(21,0),
			make_pair(22,0),
			make_pair(23,1),
			make_pair(24,1),
			make_pair(25,0),
			make_pair(26,1),
			make_pair(27,0),
			make_pair(28,1),
			make_pair(29,2),
			make_pair(30,1),
			make_pair(31,2),
			make_pair(32,1),
			make_pair(33,0),
			make_pair(34,0),
			make_pair(35,1)	
		};

		bins_norm[ "CDR3" ] = {
			make_pair(2,0.0227272727272727),
			make_pair(3,0.0227272727272727),
			make_pair(4,0),
			make_pair(5,0.0227272727272727),
			make_pair(6,0.0454545454545455),
			make_pair(7,0),
			make_pair(8,0.0909090909090909),
			make_pair(9,0.204545454545455),
			make_pair(10,0.0681818181818182),
			make_pair(11,0.0454545454545455),
			make_pair(12,0.0454545454545455),
			make_pair(13,0.0454545454545455),
			make_pair(14,0.0227272727272727),
			make_pair(15,0.0227272727272727),
			make_pair(16,0),
			make_pair(17,0.0454545454545455),
			make_pair(18,0),
			make_pair(19,0.0454545454545455),
			make_pair(20,0),
			make_pair(21,0),
			make_pair(22,0),
			make_pair(23,0.0227272727272727),
			make_pair(24,0.0227272727272727),
			make_pair(25,0),
			make_pair(26,0.0227272727272727),
			make_pair(27,0),
			make_pair(28,0.0227272727272727),
			make_pair(29,0.0454545454545455),
			make_pair(30,0.0227272727272727),
			make_pair(31,0.0454545454545455),
			make_pair(32,0.0227272727272727),
			make_pair(33,0),
			make_pair(34,0),
			make_pair(35,0.0227272727272727)
		};	

		for ( string cdr : vector<string>({ "CDR1", "CDR2", "CDR3" })) {
			TS_ASSERT_EQUALS( 
				util::bin_values( lengths[ cdr ], /*normalize=*/0 ),
				bins_raw[ cdr ] 
				);
			
			map<int,float> binned = util::bin_values( lengths[ cdr ], /*normalize=*/1 );
			
			for ( int ii = 0; ii < binned.size(); ++ii ) {
				TS_ASSERT_DELTA( 
					binned[ ii ], bins_norm[ cdr ][ ii ], 0.0001
					);
			}
		}
	 }
};

#endif /* UNITTESTS_HH_ */

