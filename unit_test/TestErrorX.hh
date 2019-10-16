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

class TestErrorX : public CxxTest::TestSuite {

public:
	void setUp() {
		correct_nt_sequence = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";

		correct_gl_sequence = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";

		correct_phred = ";=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG";

		correct_aa_sequence = "QIQLVQSGPELKKPGETVRISCKASGYTFTTAGMQWVQKMPGKGLKWIGWINTHSGVPKYAEDFKGRFVFSLETSASTAYLQITNLKNEDTATYFVARGGAAFYRNDGGAMDSWGQGTSVTVSS";

		correct_nt_sequence_corrected = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCNNTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";

		correct_aa_sequence_corrected = "QIQLVQSGPELKKPGETVRISCKASGYTFTTAGMQWVQKMPGKGLKWIGWINTHSGVPKYAEDFKGRFVFSLETSASTAYLQITNLKNEDTATYFXARGGAAFYRNDGGAMDSWGQGTSVTVSS";

		correct_cdr3_sequence = "ARGGAAFYRNDGGAMDS";
	}

	void testFromOptions() {
		ErrorXOptions options( "testing/test.fastq", "fastq" );
		options.outfile( "testing/test_out.tsv");
		options.species( "mouse" );
		options.verbose( 0 );
		options.nthreads( 1 );
		options.errorx_base( ".." );

		SequenceRecordsPtr records = run_protocol( options );

		TS_ASSERT_EQUALS( records->get(0)->quality_string(), correct_phred );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence(), correct_nt_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_gl_nt_sequence(), correct_gl_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_aa_sequence(), correct_aa_sequence );

		TS_ASSERT_EQUALS( records->get(0)->cdr3_aa_sequence(), correct_cdr3_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence_corrected(), correct_nt_sequence_corrected );

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

	void testFromOptionsChar() {
		ErrorXOptions options( "testing/test.fastq", "fastq" );
		options.outfile( "testing/test_out.tsv");
		options.species( "mouse" );
		options.verbose( 0 );
		options.nthreads( 1 );
		options.errorx_base( ".." );
		options.correction( 'X' );

		SequenceRecordsPtr records = run_protocol( options );

		TS_ASSERT_EQUALS( records->get(0)->quality_string(), correct_phred );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence(), correct_nt_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_gl_nt_sequence(), correct_gl_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_aa_sequence(), correct_aa_sequence );

		TS_ASSERT_EQUALS( records->get(0)->cdr3_aa_sequence(), correct_cdr3_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence_corrected(), 
			"CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCXXTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG"
		       	);

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = pow( 10, -4 );

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}


	void testFromOptionsTSV() {
		ErrorXOptions options( "testing/test.tsv", "tsv" );
		options.outfile( "testing/test_out.tsv");
		options.nthreads( 1 );
		options.verbose( 0 );
		options.errorx_base( ".." );

		SequenceRecordsPtr records = run_protocol( options );

		TS_ASSERT_EQUALS( records->get(0)->quality_string(), correct_phred );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence(), correct_nt_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_gl_nt_sequence(), correct_gl_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence_corrected(), correct_nt_sequence_corrected );

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = pow( 10, -4 );

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}
	
	void testFromVectors() {
		
		vector<string> sequenceIDs = { "SRR3175015.933" };
		vector<string> sequences = { "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG" };
		
		vector<string> germline_sequences = { 
			"CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG"
			};

		vector<string> phred_scores = { 
			";=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG" 	
			};

		vector<SequenceQuery> queries;
		SequenceQuery query( sequenceIDs[0], sequences[0], germline_sequences[0], phred_scores[0] );
		queries.push_back( query );

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base( ".." );
		options.verbose( 0 );
		SequenceRecordsPtr records = run_protocol( queries, options );

		TS_ASSERT_EQUALS( records->get(0)->quality_string(), correct_phred );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence(), correct_nt_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_gl_nt_sequence(), correct_gl_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence_corrected(), correct_nt_sequence_corrected );

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = pow( 10, -4 );

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}

	void testFromVectorsOptions() {
		ErrorXOptions options( "tmp", "tsv" );
		options.outfile( "" );
		options.nthreads( 1 );
		options.errorx_base( ".." );
		options.verbose( 0 );
		
		vector<string> sequenceIDs = { "SRR3175015.933" };
		vector<string> sequences = { "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG" };

		vector<string> germline_sequences = {
			"CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG"
			};

		vector<string> phred_scores = {
			";=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG"
			};

		vector<SequenceQuery> queries;
		SequenceQuery query( sequenceIDs[0], sequences[0], germline_sequences[0], phred_scores[0] );
		queries.push_back( query );
		SequenceRecordsPtr records = run_protocol( queries, options );

		TS_ASSERT_EQUALS( records->get(0)->quality_string(), correct_phred );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence(), correct_nt_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_gl_nt_sequence(), correct_gl_sequence );

		TS_ASSERT_EQUALS( records->get(0)->full_nt_sequence_corrected(), correct_nt_sequence_corrected );

		vector<pair<int,double>> predicted_errors = records->get(0)->get_predicted_errors();

		string line;
		ifstream file( "validation.txt" );
		double delta = pow( 10, -4 );

		while (getline (file, line)) {
			vector<string> tokens = errorx::util::tokenize_string<string>( line );
			int position = stoi(tokens[0]);
			double value = stod(tokens[1]);

			TS_ASSERT_DELTA( predicted_errors[position].second, value, delta)
		}
	}

	void testMalformedTSV() {
		string file = "bad.tsv";
		ofstream out( file );

		out << "one" << "," << "TACTCCNGTGGTACGCCCAAG" << "," << "TACTCCNGT---ACGCCCAAG" << "," << "###################";
		out.close();

		ErrorXOptions options( file.c_str(), "tsv" );
		options.outfile( "bad.out");
		options.verbose( 0 );
		options.errorx_base( ".." );

		TS_ASSERT_THROWS( run_protocol( options ), BadFileException );

		remove( file.c_str() );

		out = ofstream( file );
		out << "ID" << "\t" << "Sequence" << "\t" << "Germline" << "\t" << "Phred" << "\n";
		out << "one" << "\t" << "TACTCCNGTGGTACGCCCAAG" << "\t" << "TACTCCNGT---ACGCCCAAG" << "\t" << "###################";
		out.close();


		TS_ASSERT_THROWS( run_protocol( options ), invalid_argument );

		remove( file.c_str() );


		file = "valid.tsv";
		out = ofstream( file );
		out << "one" << "\t" << "TACTCCNGTGGTACGCCCAAG" << "\t" << "TACTCCNGT---ACGCCCAAG" << "\t" << "#####################" << "\t" << "\n";
		out.close();

		options.infile( file.c_str() );
		TS_ASSERT_THROWS_NOTHING( run_protocol( options ) );

		remove( file.c_str() );
		remove( "bad.out" );
		
	}

private:
	string correct_nt_sequence;
	string correct_gl_sequence;
	string correct_phred;
	string correct_aa_sequence;
	string correct_nt_sequence_corrected;
	string correct_aa_sequence_corrected;
	string correct_cdr3_sequence;
};

#endif /* UNITTESTS_HH_ */

