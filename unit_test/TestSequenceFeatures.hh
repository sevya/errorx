/*
 * UnitTests.h
 *
 *  Created on: Sep 8, 2018
 *	  Author: alexsevy
 */

#ifndef UNITTESTS_HH_
#define UNITTESTS_HH_

#include <cxxtest/TestSuite.h>

#include "SequenceFeatures.hh"
#include "SequenceQuery.hh"
#include "ErrorPredictor.hh"
#include "util.hh"
#include <string>

#include "SequenceRecord.hh"
#include "IGBlastParser.hh"

using namespace std;
using namespace errorx;

class TestSequenceFeatures : public CxxTest::TestSuite
{
public:
	void testCharEncoding(void) {
		SequenceQuery query ("test", "ACGTGACGTGACGTGACGTG", "ACGTGACGTGACGTGACGTG", "FFFFFFFFFFFFFFFFFFFF");
		SequenceRecord* record = new SequenceRecord( query );
		SequenceFeatures sf = SequenceFeatures( record, 2 );

		vector<int> test_vector;

		test_vector = {1,0,0,0,0,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'A' ), test_vector );

		test_vector = {0,1,0,0,0,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'T' ), test_vector );

		test_vector = {0,0,1,0,0,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'C' ), test_vector );

		test_vector = {0,0,0,1,0,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'G' ), test_vector );

		test_vector = {0,0,0,0,1,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'N' ), test_vector );

		test_vector = {0,0,0,0,0,1};
		TS_ASSERT_EQUALS( sf.nt_to_binary( '-' ), test_vector );

		test_vector = {0,0,0,0,0,0};
		TS_ASSERT_EQUALS( sf.nt_to_binary( 'X' ), test_vector );

		TS_ASSERT_EQUALS( sf.nt_to_binary( '?' ), test_vector );

	}

	void testSequenceEncoding(void) {
		SequenceQuery query ("test", "ACGTGACGTGACGTGACGTG", "ACGTGACGTGACGTGACGTG", "FFFFFFFFFFFFFFFFFFFF");
		SequenceRecord* record = new SequenceRecord( query );
		SequenceFeatures sf = SequenceFeatures( record, 2 );

		vector<int> test_vector = {
				1,0,0,0,0,0, //A
				1,0,0,0,0,0, //A
				0,0,1,0,0,0, //C
				0,1,0,0,0,0, //T
				0,0,0,1,0,0, //G
				0,0,1,0,0,0, //C
				0,1,0,0,0,0, //T
				0,0,0,0,1,0, //N
				0,0,0,0,0,1, //-
				0,0,0,0,0,0, //X
				0,1,0,0,0,0, //T
				1,0,0,0,0,0  //A
		};

		TS_ASSERT_EQUALS( sf.encode_sequence( "AACTGCTN-XTA" ), test_vector );
	}

	void testSequenceWindow(void) {
		SequenceQuery query ("test", "ACGTGACGTGACGTGACGTG", "ACGTGACGTGACGTGACGTG", "FFFFFFFFFFFFFFFFFFFF");
		SequenceRecord* record = new SequenceRecord( query );
		SequenceFeatures sf = SequenceFeatures( record, 2 );

		string sequence = "ATCGCAGTCCTA";

		int position;
		int window = 3;

		position = 0;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "XXXATCG" );

		position = 1;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "XXATCGC" );

		position = 2;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "XATCGCA" );

		position = 3;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "ATCGCAG" );

		position = 4;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "TCGCAGT" );

		position = 5;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "CGCAGTC" );

		position = 6;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "GCAGTCC" );

		position = 7;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "CAGTCCT" );

		position = 8;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "AGTCCTA" );

		position = 9;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "GTCCTAX" );

		position = 10;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "TCCTAXX" );

		position = 11;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), "CCTAXXX" );
	}

	void testIntWindow(void) {
		SequenceQuery query ("test", "ACGTGACGTGACGTGACGTG", "ACGTGACGTGACGTGACGTG", "FFFFFFFFFFFFFFFFFFFF");
		SequenceRecord* record = new SequenceRecord( query );
		SequenceFeatures sf = SequenceFeatures( record, 2 );
		vector<int> sequence = {	
				39,37,40,40,32,35,36,31,30,29,25,34
		};

		int position;
		int window = 3;

		position = 0;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({-1,-1,-1,39,37,40,40}));

		position = 1;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({-1,-1,39,37,40,40,32}));

		position = 2;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({-1,39,37,40,40,32,35}));

		position = 3;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({39,37,40,40,32,35,36}));

		position = 4;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({37,40,40,32,35,36,31}));

		position = 5;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({40,40,32,35,36,31,30}));

		position = 6;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({40,32,35,36,31,30,29}));

		position = 7;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({32,35,36,31,30,29,25}));

		position = 8;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({35,36,31,30,29,25,34}));

		position = 9;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({36,31,30,29,25,34,-1}));

		position = 10;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({31,30,29,25,34,-1,-1}));

		position = 11;
		TS_ASSERT_EQUALS( sf.get_window( sequence, position, window ), vector<int> ({30,29,25,34,-1,-1,-1}));

	}

	void testFeatureVector(void) {
		SequenceQuery query ("test",
			  "ATCGCAGTCCTAATCGCAGTCCTA",
			  "ATAGCTGCCCTATTAGCTGCCCTA",
			  "IIJIJIDIIIJIIIJIJIDIIIJI");
		SequenceRecord* record  = new SequenceRecord( query );

		SequenceFeatures sf ( record, 12 );
		vector<double> features = sf.get_feature_vector();
		TS_ASSERT_EQUALS( features.size(), 229 );

	}

	void testFull(void) {

		string sequence =
			"TCCTGTGCAGCCTCTGGATTCACCTTCAGTAGCTATAGAA"
			"TGAACTGGGTCCGCCAGGCTCCAGGGAAGGGGCTGGAGTG"
			"GGTCTCATCCATTAGTAGTAGTAGTAGTTACATATACTAC"
			"GCAGACTCAGTGAAGGGCCGATTCACCATCTCCAGAGACA"
			"ACGCCAAGAACTCACTGTATCTGCAAATGAACAGCCTGAG"
			"AGCCGAGGACACGGCTGTGTATTACTGTGCGAGAGATCGA"
			"GTTGGCTACGGTGACTACGGGTGGGGCCAGGGAACCCTGG"
			"TCACCGTCTCCTCAGGTAAGTTTGTTGTTTGTATGTTTGT"
			"TCAG";

		string nt_sequence =
			"TCCTGTGCAGCCTCTGGATTCACCTTCAGTAGCTATAGCA"
			"TGAACTGGGTCCGCCAGGCTCCAGGGAAGGGGCTGGAGTG"
			"GGTCTCATCCATTAGTAGTAGTAGTAGTTACATATACTAC"
			"GCAGACTCAGTGAAGGGCCGATTCACCATCTCCAGAGACA"
			"ACGCCAAGAACTCACTGTATCTGCAAATGAACAGCCTGAG"
			"AGCCGAGGACACGGCTGTGTATTACTGTGCGAGAGA----"
			"-----CTACGGTGACTAC---TGGGGCCAGGGAACCCTGG"
			"TCACCGTCTCCTCAG-------------------------"
			"----";

		string quality_string =
			"IIIHIIIIIIIIIIIIIGIIIIIIIIIIIIIIIIIIIIII"
			"IIIIIIIIIIIIHIIIGHIIIIIIIIIIIIIIIIJJ=JJJ"
			"JJJ=J=JJJ==JJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"
			"JJ?JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"
			"J;JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"
			"JJJJJJJJJJHHIIIIIIIIHHFIIIIIIHIIIHIIIIII"
			"IIIIIIHIIIIIIIIHIIHIIIIIIIIIIIIHHIIIIIII"
			"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIID"
			"DDDD";

		SequenceQuery query("test", sequence, nt_sequence, quality_string );
		SequenceRecord* record = new SequenceRecord( query );

		SequenceFeatures features ( record, 38 );

		vector<double> results_vector = {
			0.11728395061728394, 0.5185185185185185,0.35294117647058826, 38.858161711630082, 39.999999793721194, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.058823529411764705, 0.0030864197530864196
		};

		double sum1 = 0;
		double sum2 = 0;

		vector<double> results_vector_test = features.get_feature_vector();
		cout.precision(17);
		for ( int ii = 0; ii < results_vector.size(); ++ii ) {
			if ( results_vector[ii]!=results_vector_test[ii] ) {
				cout << ii << " : " << results_vector[ii] << " vs " << results_vector_test[ii] << endl;
			}
		}
		TS_ASSERT_EQUALS( results_vector_test, results_vector );


		vector<double> scaled_vector = {
			0.11728395061728394, 0.5185185185185185,0.35294117647058826, 38.858161711630082/40.0, 39.999999793721194/40.0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.058823529411764705, 0.0030864197530864196
		};

		DataScaler scaler;

		vector<double> scaled_vector_test = scaler.scale_data( features.get_feature_vector());

		sum1 = 0;
		sum2 = 0;

		for ( int ii = 0; ii < scaled_vector_test.size(); ++ii ) sum1 += scaled_vector_test[ii];
		for ( int ii = 0; ii < scaled_vector.size(); ++ii ) sum2 += scaled_vector[ii];

		TS_ASSERT_DELTA( sum1, sum2, 0.0001 );
		// TODO: fix this as soon as I have scaled variables nailed down
	
		ErrorPredictor predictor (0);

		TS_ASSERT_EQUALS( 0.39750912984814457, predictor.apply_model( features ));
	}

	void testErrorRate_singlethread() {
		vector<double> scaled_vector = {
			0.11728395061728394, 0.5185185185185185,0.35294117647058826, 38.858161711630082/40.0, 39.999999793721194/40.0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.058823529411764705, 0.0030864197530864196
		};

		ErrorPredictor predictor (0);

		DataScaler scaler;

		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {scaled_vector} );


		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {scaled_vector} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}


		ErrorXOptions options( "test", "fastq" );

		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );

		SequenceQuery query( "test",
			  "AGGTGCACTTAGTGCAGTCTGGGGCTGAGGTTAGGAAGCCTGGGGCCTCAGTGAAAATCTCCTGCAAGGCTTATGGATACACCTTCAACGACTACCATATACACTGGGTTCGGCAGGCCCCTGGACAAGGACTTGAGTGGATGGAGTTTGTAG",
			  "AGGTGCAGCTGGTGCAGTCTGGGGCTGAGGTGAAGAAGCCTGGGGCCTCAGTGAAGGTCTCCTGCAAGGCTTCTGGATACACCTTCACCGGCTACTATATGCACTGGGTGCGACAGGCCCCTGGACAAGGGCTTGAGTGGATGG---TTGTAG",
			  "4>FFGFC=,,3EGGGGGGGGGGGGGGEGGGG,@,@FGFEGFFGFEGE*@CFGGGG,,=CFGGGEF6<947*?*8AFFF6?BFFAA)?++2.=A96)/+;AFGGFFG4;F))))25<>BFEFFF3>57<C*73:4*9:76)9CFG#########" );
		SequenceRecord* template_record = new SequenceRecord( query );
		records->add_record( template_record );

		// for ( int ii = 0; ii < 20; ++ii ) {
		// 	records->add_record( new SequenceRecord( *template_record ));
		// }


		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( 0.18823529779911041, records->estimate_error_rate() );
	}

	void testErrorRate_multithread() {
		vector<double> scaled_vector = {
			0.100000,0.126469,0.538462,0.710012,0.375000,0.375000,0.959755,0.961883,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.975610,0.975610,0.975610,0.975610,0.975610,0.975610,0.975610,0.975610,0.973684,0.974359,0.974359,0.974359,0.974359,0.974359,0.974359,0.974359,0.974359,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.058824,0.069973
		};

		ErrorPredictor predictor (0);

		DataScaler scaler;

		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {scaled_vector} );

		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {scaled_vector} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}

		ErrorXOptions options( "test", "fastq" );

		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );

		SequenceQuery query( "test",
			  "AGGTGCACTTAGTGCAGTCTGGGGCTGAGGTTAGGAAGCCTGGGGCCTCAGTGAAAATCTCCTGCAAGGCTTATGGATACACCTTCAACGACTACCATATACACTGGGTTCGGCAGGCCCCTGGACAAGGACTTGAGTGGATGGAGTTTGTAG",
			  "AGGTGCAGCTGGTGCAGTCTGGGGCTGAGGTGAAGAAGCCTGGGGCCTCAGTGAAGGTCTCCTGCAAGGCTTCTGGATACACCTTCACCGGCTACTATATGCACTGGGTGCGACAGGCCCCTGGACAAGGGCTTGAGTGGATGG---TTGTAG",
			  "4>FFGFC=,,3EGGGGGGGGGGGGGGEGGGG,@,@FGFEGFFGFEGE*@CFGGGG,,=CFGGGEF6<947*?*8AFFF6?BFFAA)?++2.=A96)/+;AFGGFFG4;F))))25<>BFEFFF3>57<C*73:4*9:76)9CFG#########" );
		SequenceRecord* template_record = new SequenceRecord( query );
		records->add_record( template_record );

		// for ( int ii = 0; ii < 20; ++ii ) {
		// 	records->add_record( new SequenceRecord( *template_record ));
		// }

		SequenceRecords::correct_sequences( records );

		cout << "error rate: " << records->estimate_error_rate() << endl;

		TS_ASSERT_EQUALS( 0.18823529779911041, records->estimate_error_rate() );
	}


	void testTSVInput_singlethread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path(0).string()+"/testing/test_input.tsv", "tsv" );
		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( 0.18823529779911041, records->estimate_error_rate() );
	}


	void testTSVInput_multithread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path(0).string()+"/testing/test_input.tsv", "tsv" );
		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( 0.18823529779911041, records->estimate_error_rate() );
	}

};

#endif /* UNITTESTS_HH_ */

