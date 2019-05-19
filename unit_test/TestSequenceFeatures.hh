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
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "util.hh"
#include <string>

#include "SequenceRecord.hh"
#include "IGBlastParser.hh"
#include <cmath>

using namespace std;
using namespace errorx;

class TestSequenceFeatures : public CxxTest::TestSuite
{
public:

	void setUp() {
		sequenceID_ = "SRR3175015.933";
		sequence_ =
			"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG";

		gl_sequence_ =
			"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG";

		quality_string_ =
			"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,";

		query_  = new SequenceQuery( sequenceID_, sequence_, gl_sequence_, quality_string_ );
		record_ = new SequenceRecord( *query_ );

		

		raw_vector_ = { 0.63535911602209949, 0.35294117647058826, 0.077207137977055973, 0.097010460385836522, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.85000000000000009, 0.625, 0.70000000000000007, 0.60000000000000009, 0.77500000000000002, 0.55000000000000004, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.18232044198895028 };

		predicted_value_ = 0.027573684313656657;
		predicted_error_rate_ = 0.054154586729391;

		cout.precision( 9 );
	}

	void tearDown() {
		delete query_;
		delete record_;
	}

	void testAbSequence() {
		TS_ASSERT_EQUALS( record_->full_nt_sequence(), 
		"TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG" );

		TS_ASSERT_EQUALS( record_->full_gl_nt_sequence(), 
		"TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG" );
		
		TS_ASSERT_EQUALS( record_->quality_string(),
		"###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4," );
	}

	void testCharEncoding() {
		SequenceFeatures sf = SequenceFeatures( record_, 2 );

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

		// converting ? to binary should throw an exception
		TS_ASSERT_THROWS( sf.nt_to_binary( '?' ), invalid_argument );
	}

	void testSequenceEncoding() {
		SequenceFeatures sf = SequenceFeatures( record_, 2 );

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

	void testSequenceWindow() {
		SequenceFeatures sf = SequenceFeatures( record_, 2 );

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

	void testIntWindow() {
		SequenceFeatures sf = SequenceFeatures( record_, 2 );
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

	void testFeatureVectorSize() {
		SequenceFeatures sf ( record_, 12 );
		vector<double> features = sf.get_feature_vector();
		TS_ASSERT_EQUALS( features.size(), 228 );

	}


	void testPhredWindow() {

		string sequence = "AGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCA";
		string gl_sequence = "AGGGCAGAGTCACGATTACCGCGGACAAATCCACGAGCACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAGGACACGGCCGTGTATTACTGTGCGAGAGA------------------------CTGGGGCCAAGGGACCACGGTCACCGTCTCCTCA";
		string phred_str = "########################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4";

		SequenceQuery query( "test",sequence, gl_sequence, phred_str );
		SequenceRecord* record = new SequenceRecord( query );
		SequenceFeatures sf = SequenceFeatures( record, 122 );

		vector<int> quals = {2,2,2,2,2,2,34,25,28,24,31,22,10,34,21,10,10};
		for ( int ii = 0; ii < sf.quality_window().size(); ++ii ) {
			TS_ASSERT_EQUALS( quals[ii], sf.quality_window()[ ii ] );
		}

		delete record;
	}

	void testQuery() {
		TS_ASSERT_THROWS(
			SequenceQuery( "WhateverName", "ACTG", "GCTA", "FFFG " ),
			invalid_argument
			);

		query_->sequenceID( "Name" );
		query_->sequence("CGGG");
		query_->germline_sequence("CGAG");
		query_->phred_string("FFGF");

		TS_ASSERT_EQUALS( query_->sequenceID(), "Name" );
		TS_ASSERT_EQUALS( query_->sequence(), "CGGG");
		TS_ASSERT_EQUALS( query_->germline_sequence(), "CGAG");
		TS_ASSERT_EQUALS( query_->phred_string(), "FFGF");
	}

	void testPositionalPrediction() {
		ErrorXOptions options( "testing/test.fastq", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );

		string line;
		ifstream infile( "validation.txt" );
		int position;
		double probability;
		double pred;

		while (getline (infile, line)) {
			stringstream line_stream( line );
			line_stream >> position >> probability;
			
			SequenceFeatures features( record_, position );
			pred = predictor.apply_model( features );

			TS_ASSERT_DELTA( probability, pred, pow(10,-5));
		}
	}

	void testFeaturesProperlyCalculated() {
		SequenceFeatures features ( record_, 136 );

		vector<double> results_vector_test = features.get_feature_vector();

		for ( int ii = 0; ii < raw_vector_.size(); ++ii ) {
			TS_ASSERT_DELTA( raw_vector_[ii], results_vector_test[ii], pow(10,-9) )
		}
	}

	void testProperPredictionsFromFeatures() {
		SequenceFeatures features ( record_, 136 );

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );

		TS_ASSERT_DELTA( predicted_value_, predictor.apply_model( features ), pow(10,-9) );
	}

	void testErrorRate_singlethread() {

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );


		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {raw_vector_} );


		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {raw_vector_} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}

		options = ErrorXOptions( "test", "fastq" );
		options.errorx_base( "../" );
		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );

		records->add_record( new SequenceRecord( *record_ ));

		SequenceRecords::correct_sequences( records );

		TS_ASSERT_DELTA( predicted_error_rate_, records->estimate_error_rate(), pow(10,-9) );

		delete records;
	}

	void testErrorRate_multithread() {

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );

		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {raw_vector_} );

		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {raw_vector_} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}

		options = ErrorXOptions( "test", "fastq" );
		options.errorx_base("../");
		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );

		records->add_record( new SequenceRecord( *record_ ));

		SequenceRecords::correct_sequences( records );

		TS_ASSERT_DELTA( predicted_error_rate_, records->estimate_error_rate(), pow(10,-9) );
		
		delete records;
	}


	void testTSVInput_singlethread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path().string()+"/testing/test.tsv", "tsv" );
		options.errorx_base("../");
		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_DELTA( predicted_error_rate_, records->estimate_error_rate(), pow(10,-9) );
		delete records;

	}


	void testTSVInput_multithread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path().string()+"/testing/test.tsv", "tsv" );
		options.errorx_base("../");
		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_DELTA( predicted_error_rate_, records->estimate_error_rate(), pow(10,-9) );
		delete records;

	}


	void testCalculateMetrics() {
		SequenceFeatures sf = SequenceFeatures( record_, 2 );
		string one = "ACGTACGT";
		string two = "ACGTCGGT";
		pair<double,double> pair = sf.calculate_metrics( one, two );
		double gc_count = pair.first;
		double shm = pair.second;

		TS_ASSERT_EQUALS( gc_count, 0.5 );
		TS_ASSERT_EQUALS( shm, 0.25 );
	}

	string sequenceID_;
	string sequence_;
	string gl_sequence_;
	string quality_string_;
	SequenceQuery* query_;
	SequenceRecord* record_;

	vector<double> raw_vector_;

	double predicted_value_;
	double predicted_error_rate_;

};

#endif /* UNITTESTS_HH_ */

