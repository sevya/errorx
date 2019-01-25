/*
 * TestErrorX.hh
 *
 *  Created on: Nov 13, 2018
 *      Author: alexsevy
 */

#ifndef TESTERRORX_HH_
#define TESTERRORX_HH_

#include <cxxtest/TestSuite.h>

#include "errorx.hh"
#include "ErrorXOptions.hh"
#include "SequenceQuery.hh"

using namespace std;
using namespace errorx;

class TestErrorX : public CxxTest::TestSuite
{

public:

	void testFromOptions(void) {
		options_.infile( "testing/test.fastq" );

		options_.format( "fastq" );
		options_.outfile( "testing/test_out.tsv");

		options_.species( "mouse" );
		options_.verbose( 1 );
		options_.nthreads( 1 );
		string base = "../";
		options_.errorx_base( base );

		SequenceRecords* records = run_protocol( options_ );

		TS_ASSERT_EQUALS(
			records->get(0)->quality_string(),
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_gl_nt_sequence(),
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_aa_sequence(),
			"YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->cdr3_aa_sequence(),
			"AGEEALSFVYY"
			);


		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence_corrected(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		double delta = 0.0000001;
		TS_ASSERT_DELTA( predicted_errors[132].second, 0.486122388, delta)
		TS_ASSERT_DELTA( predicted_errors[133].second, 0.348359625, delta)
		TS_ASSERT_DELTA( predicted_errors[134].second, 0.143818731, delta)
		TS_ASSERT_DELTA( predicted_errors[135].second, 0.458915133, delta)
		TS_ASSERT_DELTA( predicted_errors[136].second, 0.306816917, delta)
		TS_ASSERT_DELTA( predicted_errors[137].second, 0.022584986, delta)
		TS_ASSERT_DELTA( predicted_errors[138].second, 0.513773250, delta)
	}

	void testFromOptionsChar(void) {
		options_.infile( "testing/test.fastq" );

		options_.format( "fastq" );
		options_.outfile( "testing/test_out.tsv");

		options_.species( "mouse" );
		options_.verbose( 1 );
		options_.nthreads( 1 );
		string base = "../";
		options_.errorx_base( base );

		ErrorXOptions options( options_ );
		options.correction( 'X' );

		SequenceRecords* records = run_protocol( options );

		TS_ASSERT_EQUALS(
			records->get(0)->quality_string(),
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_gl_nt_sequence(),
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_aa_sequence(),
			"YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->cdr3_aa_sequence(),
			"AGEEALSFVYY"
			);


		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence_corrected(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACXGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGXGCXCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCXGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		double delta = 0.0000001;
		TS_ASSERT_DELTA( predicted_errors[132].second, 0.486122388, delta)
		TS_ASSERT_DELTA( predicted_errors[133].second, 0.348359625, delta)
		TS_ASSERT_DELTA( predicted_errors[134].second, 0.143818731, delta)
		TS_ASSERT_DELTA( predicted_errors[135].second, 0.458915133, delta)
		TS_ASSERT_DELTA( predicted_errors[136].second, 0.306816917, delta)
		TS_ASSERT_DELTA( predicted_errors[137].second, 0.022584986, delta)
		TS_ASSERT_DELTA( predicted_errors[138].second, 0.513773250, delta)
	}


	void testFromOptionsTSV(void) {
		options_.infile( "testing/test.tsv" );
		options_.format( "tsv" );
		options_.outfile( "testing/test_out.tsv");
		options_.species( "mouse" );
		options_.nthreads( 1 );
		string base = "../";
		options_.errorx_base( base );

		SequenceRecords* records = run_protocol( options_ );

		TS_ASSERT_EQUALS(
			records->get(0)->quality_string(),
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		TS_ASSERT_EQUALS(
			records->get(0)->full_gl_nt_sequence(),
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG"
			);


		TS_ASSERT_EQUALS(
			records->get(0)->full_nt_sequence_corrected(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		double delta = 0.0000001;
		TS_ASSERT_DELTA( predicted_errors[132].second, 0.486122388, delta)
		TS_ASSERT_DELTA( predicted_errors[133].second, 0.348359625, delta)
		TS_ASSERT_DELTA( predicted_errors[134].second, 0.143818731, delta)
		TS_ASSERT_DELTA( predicted_errors[135].second, 0.458915133, delta)
		TS_ASSERT_DELTA( predicted_errors[136].second, 0.306816917, delta)
		TS_ASSERT_DELTA( predicted_errors[137].second, 0.022584986, delta)
		TS_ASSERT_DELTA( predicted_errors[138].second, 0.513773250, delta)
	}
	
	void testFromVectors(void) {
		
		vector<string> sequenceIDs = { "SRR3175015.933" };
		vector<string> sequences = { 
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG" };
		vector<string> germline_sequences = { 
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG"
			};
		vector<string> phred_scores = { 
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,"
			 	};

		vector<SequenceQuery> queries;
		SequenceQuery query( sequenceIDs[0], sequences[0], germline_sequences[0], phred_scores[0] );
		queries.push_back( query );

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base( "../" );
		SequenceRecords* records = run_protocol( queries, options );

		SequenceRecord* record = records->get(0);
		TS_ASSERT_EQUALS(
			record->quality_string(),
			phred_scores[0]
			);

		TS_ASSERT_EQUALS(
			record->full_nt_sequence(),
			sequences[0]
			);

		TS_ASSERT_EQUALS(
			record->full_gl_nt_sequence(),
			germline_sequences[0]
			);

		TS_ASSERT_EQUALS(
			record->full_nt_sequence_corrected(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = record->get_predicted_errors();

		double delta = 0.0000001;
		TS_ASSERT_DELTA( predicted_errors[132].second, 0.486122388, delta)
		TS_ASSERT_DELTA( predicted_errors[133].second, 0.348359625, delta)
		TS_ASSERT_DELTA( predicted_errors[134].second, 0.143818731, delta)
		TS_ASSERT_DELTA( predicted_errors[135].second, 0.458915133, delta)
		TS_ASSERT_DELTA( predicted_errors[136].second, 0.306816917, delta)
		TS_ASSERT_DELTA( predicted_errors[137].second, 0.022584986, delta)
		TS_ASSERT_DELTA( predicted_errors[138].second, 0.513773250, delta)
	}

	void testFromVectorsOptions(void) {
		
		options_.infile( "tmp" );
		options_.format( "tsv" );
		options_.outfile( "");
		options_.species( "mouse" );
		options_.nthreads( 1 );
		string base = "../";
		options_.errorx_base( base );

		vector<string> sequenceIDs = { "SRR3175015.933" };
		vector<string> sequences = { 
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG" };
		vector<string> germline_sequences = { 
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG"
			};
		vector<string> phred_scores = { 
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,"
			 	};


		vector<SequenceQuery> queries;
		SequenceQuery query( sequenceIDs[0], sequences[0], germline_sequences[0], phred_scores[0] );
		queries.push_back( query );
		SequenceRecords* records = run_protocol( queries, options_ );

		SequenceRecord* record = records->get(0);
		TS_ASSERT_EQUALS(
			record->quality_string(),
			phred_scores[0]
			);

		TS_ASSERT_EQUALS(
			record->full_nt_sequence(),
			sequences[0]
			);

		TS_ASSERT_EQUALS(
			record->full_gl_nt_sequence(),
			germline_sequences[0]
			);

		TS_ASSERT_EQUALS(
			record->full_nt_sequence_corrected(),
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = record->get_predicted_errors();

		double delta = 0.0000001;
		TS_ASSERT_DELTA( predicted_errors[132].second, 0.486122388, delta)
		TS_ASSERT_DELTA( predicted_errors[133].second, 0.348359625, delta)
		TS_ASSERT_DELTA( predicted_errors[134].second, 0.143818731, delta)
		TS_ASSERT_DELTA( predicted_errors[135].second, 0.458915133, delta)
		TS_ASSERT_DELTA( predicted_errors[136].second, 0.306816917, delta)
		TS_ASSERT_DELTA( predicted_errors[137].second, 0.022584986, delta)
		TS_ASSERT_DELTA( predicted_errors[138].second, 0.513773250, delta)
	}

private:
	ErrorXOptions options_;

};

#endif /* UNITTESTS_HH_ */

