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
		options_.verbose( 0 );
		options_.nthreads( 1 );
		string base = "..";
		options_.errorx_base( base );

		SequenceRecordsPtr records = run_protocol( options_ );

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
			"TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = 0.001;

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}

	void testFromOptionsChar(void) {
		options_.infile( "testing/test.fastq" );

		options_.format( "fastq" );
		options_.outfile( "testing/test_out.tsv");

		options_.species( "mouse" );
		options_.verbose( 0 );
		options_.nthreads( 1 );
		string base = "..";
		options_.errorx_base( base );

		ErrorXOptions options( options_ );
		options.correction( 'X' );

		SequenceRecordsPtr records = run_protocol( options );

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
			"TACTCCXGTGGTACGCCCAAGGACGGAGGCACACXGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGXGCXCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTXCXGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = 0.001;

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}


	void testFromOptionsTSV(void) {
		options_.infile( "testing/test.tsv" );
		options_.format( "tsv" );
		options_.outfile( "testing/test_out.tsv");
		options_.species( "mouse" );
		options_.nthreads( 1 );
		string base = "..";
		options_.errorx_base( base );

		SequenceRecordsPtr records = run_protocol( options_ );

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
			"TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = 0.001;

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
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
		options.errorx_base( ".." );
		SequenceRecordsPtr records = run_protocol( queries, options );

		SequenceRecordPtr record = records->get(0);
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
			"TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = record->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = 0.001;

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}

	void testFromVectorsOptions(void) {
		
		options_.infile( "tmp" );
		options_.format( "tsv" );
		options_.outfile( "");
		options_.species( "mouse" );
		options_.nthreads( 1 );
		string base = "..";
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
		SequenceRecordsPtr records = run_protocol( queries, options_ );

		SequenceRecordPtr record = records->get(0);
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
			"TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG"
			);

		vector<pair<int,double>> predicted_errors = record->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = 0.001;

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}

private:
	ErrorXOptions options_;

};

#endif /* UNITTESTS_HH_ */

