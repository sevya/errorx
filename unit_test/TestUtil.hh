/*
 * TestUtil.hh
 *
 *  Created on: Sep 29, 2018
 *		Author: alexsevy
 */

#ifndef UNIT_TEST_TESTUTIL_HH_
#define UNIT_TEST_TESTUTIL_HH_

#include <cxxtest/TestSuite.h>

#include "util.hh"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace errorx;

class TestUtil : public CxxTest::TestSuite
{
public:

	void testTranslation(void) {

		string one = "GGGTGGGACAGGGCCCGGGAAAAGGGGTTGAGGGGAGGGGAGGGATCAACGGGAACAGTGGGGGCACAAACGAGGCAG";
		TS_ASSERT_EQUALS(
			util::translate(one, 1),
			"GWDRAREKGLRGGEGSTGTVGAQTRQ"
			);
		TS_ASSERT_EQUALS(
			util::translate(one, 2),
			"GGTGPGKRG*GEGRDQREQWGHKRG"
			);
		TS_ASSERT_EQUALS(
			util::translate(one, 3),
			"VGQGPGKGVEGRGGINGNSGGTNEA"
			);

		string two = "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG";
		
		TS_ASSERT_EQUALS(
			util::translate( two, 1 ),
			"SVVTTGAGSASPQGRGWSGLGKSIIVEAPTTTRPSRVESPYQ*TRPRTSSP*S*AL*PPRTRLCITVRE");

		TS_ASSERT_EQUALS(
			util::translate( two, 2 ),
			"QWLLLELDPPAPREGAGVDWGNQS*WKHQLQPVPQESSHHISRHVQEPVLPEAELCDRRGHGCVLLCER");

		TS_ASSERT_EQUALS(
			util::translate( two, 3 ),
			"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCAR");


		two = "TCNGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG";
		
		TS_ASSERT_EQUALS(
			util::translate( two, 1 ),
			"XVVTTGAGSASPQGRGWSGLGKSIIVEAPTTTRPSRVESPYQ*TRPRTSSP*S*AL*PPRTRLCITVRE");
	}


	void testFrameInference(void) {


		string two = "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG";
		int start = 2;
		int frame = -1;
		// start % 3 == 1 -> already in correct frame
		if ( start % 3 == 1 ) frame = 1;
		else if ( start % 3 == 2 ) frame = 3;
		else if ( start % 3 == 0 ) frame = 2;

		TS_ASSERT_EQUALS(
			util::translate( two, frame ),
			"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCAR");


		start = 1;
		if ( start % 3 == 1 ) frame = 1;
		else if ( start % 3 == 2 ) frame = 3;
		else if ( start % 3 == 0 ) frame = 2;
		TS_ASSERT_EQUALS(
			util::translate( two, frame ),
			"SVVTTGAGSASPQGRGWSGLGKSIIVEAPTTTRPSRVESPYQ*TRPRTSSP*S*AL*PPRTRLCITVRE");
		

		start = 3;
		if ( start % 3 == 1 ) frame = 1;
		else if ( start % 3 == 2 ) frame = 3;
		else if ( start % 3 == 0 ) frame = 2;
		TS_ASSERT_EQUALS(
			util::translate( two, 2 ),
			"QWLLLELDPPAPREGAGVDWGNQS*WKHQLQPVPQESSHHISRHVQEPVLPEAELCDRRGHGCVLLCER");

		
	}

	void testEncrypt(void) {
		string matt = "MattRyan";
		vector<char> code = util::encrypt_string( matt );
		string encrypted = "NfvOY(<u";
		for ( int ii = 0; ii < code.size(); ++ii ) {
			TS_ASSERT_EQUALS( code[ii], encrypted[ii] );
		}
	
		util::write_license("MattRyan");
		string home = (util::get_home()/".errorx").string();
		string code2 = util::decrypt_from_file( home );
		TS_ASSERT_EQUALS( code2, matt );

		TS_ASSERT( util::valid_license() );

		util::write_license("Matt Ryan");
		TS_ASSERT( !util::valid_license() );

		util::write_license("MattRyan");
	}

	void testTokenize(void) {

		vector<int> output2 = { 1,2,3,4,5 };

		string one = "1 2 3 4 \t5"; 
		TS_ASSERT_EQUALS(
				util::tokenize_string<int>( one ),
				output2
		);

		one = "1,2,3,4, 5";
		TS_ASSERT_EQUALS(
				util::tokenize_string<int>( one, ", " ),
				output2
		);
	}

	void testReverse(void) {
		string seq = "ABCDEFG";
		TS_ASSERT_EQUALS( util::reverse(seq), "GFEDCBA" );
	}

	void testCalculateMetrics(void) {
		string one = "ACGTACGT";
		string two = "ACGTCGGT";
		pair<int,double> pair = util::calculate_metrics( one, two );
		int gc_count = pair.first;
		double shm = pair.second;

		TS_ASSERT_EQUALS( gc_count, 4 );
		TS_ASSERT_EQUALS( shm, 0.25 );
	}

	void testAvg(void) {
		vector<int> phred = {31,32,40};
		TS_ASSERT_DELTA( util::phred_avg_realspace( phred ), 32.9377009047, 0.0001 );
		phred = {-1,31,32,40};
		TS_ASSERT_DELTA( util::phred_avg_realspace( phred ), 32.9377009047, 0.0001 );

	}

	void testSplitVector(void) {
		vector<string> test = {"1","2","3","4","5","6","7","8","9","10","11"};

		vector<vector<string>> test_split = util::split_vector<string>( test, 3 );

		TS_ASSERT_EQUALS( test_split[0], vector<string>( {"1","2","3","4"} ));
		TS_ASSERT_EQUALS( test_split[1], vector<string>( {"5","6","7","8"} ));
		TS_ASSERT_EQUALS( test_split[2], vector<string>( {"9","10","11"} ));

		test = {"1"};
		test_split = util::split_vector<string>( test, 3 );
		TS_ASSERT_EQUALS( test_split[0], vector<string>( {"1"} ));
		TS_ASSERT_EQUALS( test_split[1], vector<string>( {} ));
		TS_ASSERT_EQUALS( test_split[2], vector<string>( {} ));
	}


	// void testCountLines(void) {

	// 	string file = "testing/test.fastq";
	// 	TS_ASSERT_EQUALS( util::count_lines(file), 4 );
	// }
		
};



#endif /* UNIT_TEST_TESTUTIL_HH_ */
