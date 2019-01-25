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

		query_ = new SequenceQuery( sequenceID_, sequence_, gl_sequence_, quality_string_ );
		record_ = new SequenceRecord( *query_ );

		raw_vector_ = { 0.75138121546961323, 0.63535911602209949, 0.35294117647058826, 3.0882855190822389, 3.8804184154334607, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 34.0000, 25.0000, 28.0000, 24.0000, 31.0000, 22.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.18232044198895028 };

		scaled_vector_ = { 0.75138121546961323, 0.63535911602209949, 0.35294117647058826, 3.0882855190822389 / 40, 3.8804184154334607 / 40, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 2.0000/ 40, 34.0000/ 40, 25.0000/ 40, 28.0000/ 40, 24.0000/ 40, 31.0000/ 40, 22.0000/ 40, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 1.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.18232044198895028 };

		predicted_value_ = 0.30681691739419625;
		predicted_error_rate_ = 0.032705210149288177;
	}

	void tearDown() {
		delete query_;
		delete record_;
	}

	void testCharEncoding(void) {
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
		// try { 
			TS_ASSERT_THROWS( sf.nt_to_binary( '?' ), invalid_argument );
		// 	TS_ASSERT( 0 );
		// } catch ( invalid_argument & exc ) {
		// 	TS_ASSERT( 1 );
		// }
	}

	void testSequenceEncoding(void) {
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

	void testSequenceWindow(void) {
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

	void testIntWindow(void) {
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

	void testFeatureVectorSize(void) {
		SequenceFeatures sf ( record_, 12 );
		vector<double> features = sf.get_feature_vector();
		TS_ASSERT_EQUALS( features.size(), 229 );

	}


	void testPhredWindow(void) {

		string sequence = "AGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCA";
		string gl_sequence = "AGGGCAGAGTCACGATTACCGCGGACAAATCCACGAGCACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAGGACACGGCCGTGTATTACTGTGCGAGAGA------------------------CTGGGGCCAAGGGACCACGGTCACCGTCTCCTCA";
		string phred_str = "########################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4";

		SequenceQuery query( "test",sequence, gl_sequence, phred_str );
		SequenceRecord* record = new SequenceRecord( query, 1 );
		SequenceFeatures sf = SequenceFeatures( record, 122 );

		cout << "getting quality window:" << endl;
		vector<int> quals = {2,2,2,2,2,2,34,25,28,24,31,22,10,34,21,10,10};
		for ( int ii = 0; ii < sf.quality_window_.size(); ++ii ) {
			TS_ASSERT_EQUALS( quals[ii], sf.quality_window_[ ii ] );
		}

		delete record;
	}

	void testFull(void) {

		SequenceFeatures features ( record_, 136 );

		vector<double> results_vector_test = features.get_feature_vector();
		cout.precision(17);
		for ( int ii = 0; ii < raw_vector_.size(); ++ii ) {
			if ( raw_vector_[ii]!=results_vector_test[ii] ) {
				cout << ii << " : " << raw_vector_[ii] << " vs " << results_vector_test[ii] << endl;
			}
		}
		TS_ASSERT_EQUALS( results_vector_test, raw_vector_ );

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );

		TS_ASSERT_EQUALS( predicted_value_, predictor.apply_model( features ));

	}

	void testErrorRate_singlethread() {

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );


		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {scaled_vector_} );


		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {scaled_vector_} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}


		options = ErrorXOptions( "test", "fastq" );
		options.errorx_base( "../" );
		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );

		records->add_record( new SequenceRecord( *record_ ));

		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( predicted_error_rate_, records->estimate_error_rate() );

		delete records;
	}

	void testErrorRate_multithread() {

		ErrorXOptions options( "tmp", "tsv" );
		options.errorx_base("../");
		options.verbose(0);
		ErrorPredictor predictor( options );

		vector<double> pred1 = predictor.apply_model( vector<vector<double>> {scaled_vector_} );

		for ( int ii = 0; ii < 1000; ++ii ) {
			vector<double> pred2 = predictor.apply_model( vector<vector<double>> {scaled_vector_} );
			TS_ASSERT_EQUALS( pred1[0], pred2[0] );
		}

		options = ErrorXOptions( "test", "fastq" );
		options.errorx_base("../");
		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );

		records->add_record( new SequenceRecord( *record_ ));

		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( predicted_error_rate_, records->estimate_error_rate() );

		delete records;
	}


	void testTSVInput_singlethread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path(0).string()+"/testing/test.tsv", "tsv" );
		options.errorx_base("../");
		options.nthreads( 1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( predicted_error_rate_, records->estimate_error_rate() );
		delete records;

	}


	void testTSVInput_multithread() {
		// sequenceID,nt_sequence,gl_sequence,quality_string
		ErrorXOptions options( util::get_root_path(0).string()+"/testing/test.tsv", "tsv" );
		options.errorx_base("../");
		options.nthreads( -1 );

		SequenceRecords* records = new SequenceRecords( options );
		records->import_from_tsv();
	
		SequenceRecords::correct_sequences( records );

		TS_ASSERT_EQUALS( predicted_error_rate_, records->estimate_error_rate() );
		delete records;

	}

	string sequenceID_;
	string sequence_;
	string gl_sequence_;
	string quality_string_;
	SequenceQuery* query_;
	SequenceRecord* record_;

	vector<double> raw_vector_;
	vector<double> scaled_vector_;

	double predicted_value_;
	double predicted_error_rate_;

};

#endif /* UNITTESTS_HH_ */

