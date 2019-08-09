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
#include "exceptions.hh"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

using namespace std;
using namespace errorx;

class TestUtil : public CxxTest::TestSuite
{
public:

	void testTranslation() {

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


	void testFrameInference() {


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

	void testEncrypt() {
		string year_cipher = "ZJC-";
		string inf_cipher = "JSH$UU/L";

		vector<int> date = util::parse_formatted_date( "20190206" );
		TS_ASSERT_EQUALS( date[0], 2019 );
		TS_ASSERT_EQUALS( date[1], 02 );
		TS_ASSERT_EQUALS( date[2], 06 );

		// write license to expire in 13 months
		util::write_license( year_cipher );
		TS_ASSERT( util::valid_license() );

		// write license to never expire
		util::write_license( inf_cipher );
		TS_ASSERT( util::valid_license() );

		// put bad license key
		try {
			util::write_license("ZJC- ");
		}
		catch ( BadLicenseException ) {
			TS_ASSERT(1);
		}
		catch (...) {
			TS_ASSERT(0);
		}

		// put bad license key
		// check on the message from bad license
		try {
			util::write_license( "ZXC-" );
			TS_ASSERT(0);
		} catch ( BadLicenseException & exc ) {
			TS_ASSERT_EQUALS( 
				exc.what(), 
				"License is not valid. Please contact alex@endeavorbio.com for assistance"
				);
		}
		catch (...) {
			TS_ASSERT(0);
		}
		util::write_license( inf_cipher );
	}

	void testTokenize() {

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

		one = "1 2 3 4 \t5\t ";
		TS_ASSERT_EQUALS(
				util::tokenize_string<int>( one ),
				output2
		);
	}

	void testTokenizeConsecutive() {

		string one = "a\tb\tc";
		vector<string> output = { "a","b","c" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 0 ),
				output
		);

		one = "a\tb\t\tc";
		output = { "a","b","","c" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 0 ),
				output
		);


		one = "a\tb\t\tc\t";
		output = { "a","b","","c","" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 0 /*token_compress*/, 0 /*trim*/ ),
				output
		);

		one = "a\tb\t \tc\t";
		output = { "a","b"," ","c","" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 0 /*token_compress*/, 0 /*trim*/ ),
				output
		);


		one = "a\tb\t\tc\t";
		output = { "a","b","c" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 1 /*token_compress*/, 1 /*trim*/ ),
				output
		);

		one = "a\tb\t\tc\t";
		output = { "a","b","c","" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 1 /*token_compress*/, 0 /*trim*/ ),
				output
		);

		one = "a\tb\t\tc\t";
		output = { "a","b","","c" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 0 /*token_compress*/, 1 /*trim*/ ),
				output
		);


		one = "a\tb\t \tc\t";
		output = { "a","b"," ","c" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 1, 1 ),
				output
		);

		one = "a\tb\t \tc\t";
		output = { "a","b"," ","c","" };

		TS_ASSERT_EQUALS(
				util::tokenize_string<string>( one, "\t", 1, 0 ),
				output
		);
	}

	void testReverse() {
		string seq = "ABCDEFG";
		TS_ASSERT_EQUALS( util::reverse(seq), "GFEDCBA" );
	}

	void testAvg() {
		vector<int> phred = {31,32,40};
		TS_ASSERT_DELTA( util::phred_avg_realspace( phred ), 32.9377009047, 0.0001 );
		phred = {-1,31,32,40};
		TS_ASSERT_DELTA( util::phred_avg_realspace( phred ), 32.9377009047, 0.0001 );

	}

	void testSplitVector() {
		vector<string> test = {"1","2","3","4","5","6","7","8","9","10","11"};

		vector<vector<string>> test_split = util::split_vector<string>( test, 3 );

		TS_ASSERT_EQUALS( test_split[0], vector<string>( {"1","2","3","4"} ));
		TS_ASSERT_EQUALS( test_split[1], vector<string>( {"5","6","7","8"} ));
		TS_ASSERT_EQUALS( test_split[2], vector<string>( {"9","10","11"} ));

		test.push_back( "12" );
		test_split = util::split_vector<string>( test, 3 );

		TS_ASSERT_EQUALS( test_split[0], vector<string>( {"1","2","3","4"} ));
		TS_ASSERT_EQUALS( test_split[1], vector<string>( {"5","6","7","8"} ));
		TS_ASSERT_EQUALS( test_split[2], vector<string>( {"9","10","11","12"} ));

		vector<int> test_int = {1,2,3,4,5,6,7,8,9,10,11,12};
		vector<vector<int>> split_int = util::split_vector<int>( test_int, 3 );

		TS_ASSERT_EQUALS( split_int[0], vector<int>( {1,2,3,4} ));
		TS_ASSERT_EQUALS( split_int[1], vector<int>( {5,6,7,8} ));
		TS_ASSERT_EQUALS( split_int[2], vector<int>( {9,10,11,12} ));


		test = {"1"};
		test_split = util::split_vector<string>( test, 3 );
		TS_ASSERT_EQUALS( test_split[0], vector<string>( {"1"} ));
		TS_ASSERT_EQUALS( test_split[1], vector<string>( {} ));
		TS_ASSERT_EQUALS( test_split[2], vector<string>( {} ));
	}

	void testScientific() {
		double value = 1056.5;
		TS_ASSERT_EQUALS(
			util::to_scientific( value ),
			"1.06E+03" 
			);

		value = 0.1035;
		TS_ASSERT_EQUALS(
			util::to_scientific( value ),
			"1.03E-01" 
			);

		int intValue = 1056;
		TS_ASSERT_EQUALS(
			util::to_scientific( intValue ),
			"1.06E+03" 
			);

		long longValue = 1056;
		TS_ASSERT_EQUALS(
			util::to_scientific( longValue ),
			"1.06E+03" 
			);
	}


	void testRounded() {
		double value = 1056.5;
		TS_ASSERT_EQUALS(
			util::rounded_string( value ),
			"1056.50" 
			);

		value = 0.1035;
		TS_ASSERT_EQUALS(
			util::rounded_string( value ),
			"0.10" 
			);

		value = 0.1055;
		TS_ASSERT_EQUALS(
			util::rounded_string( value ),
			"0.11" 
			);

		int intValue = 1056;
		TS_ASSERT_EQUALS(
			util::rounded_string( (double)intValue ),
			"1056.00" 
			);

		long longValue = 1056;
		TS_ASSERT_EQUALS(
			util::rounded_string( (double)longValue ),
			"1056.00" 
			);
	}


	void testValueCounts() {
		vector<string> input = {
			"one",
			"two",
			"three",
			"four",
			"one",
			"two",
			"one"
		};

		map<string,int> cmap = util::value_counts( input );
		TS_ASSERT_EQUALS( cmap.size(), 4 );
		TS_ASSERT_EQUALS( cmap[ "one" ], 3 );
		TS_ASSERT_EQUALS( cmap[ "two" ], 2 );
		TS_ASSERT_EQUALS( cmap[ "three" ], 1 );
		TS_ASSERT_EQUALS( cmap[ "four" ], 1 );

	}
		
};



#endif /* UNIT_TEST_TESTUTIL_HH_ */
