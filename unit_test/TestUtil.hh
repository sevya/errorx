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
		TS_ASSERT_THROWS( util::write_license( "ZJC- " ), BadLicenseException );

		// put bad license key
		TS_ASSERT_THROWS( util::write_license( "ZXC-" ), BadLicenseException );

		TS_ASSERT_THROWS( util::write_license( "ZXC-" ), BadLicenseException );

		util::write_license( inf_cipher );
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


	void testComparator(void) {

		// check equivalence of two keys using my custom comparators
		string a;
		string b;

		a = "ATCG";
		b = "ATCG";
		TS_ASSERT( !util::vanilla_compare(a,b) && !util::vanilla_compare(b,a) );

		a = "CTCG";
		b = "ATCG";
		TS_ASSERT( !(!util::vanilla_compare(a,b) && !util::vanilla_compare(b,a)) );

		a = "ATCG";
		b = "CTCG";
		TS_ASSERT( !(!util::vanilla_compare(a,b) && !util::vanilla_compare(b,a)) );

		a = "ATCG";
		b = "NTCG";
		TS_ASSERT( !(!util::vanilla_compare(a,b) && !util::vanilla_compare(b,a)) );


		a = "ATCG";
		b = "NTCG";
		TS_ASSERT( !util::compare(a,b,'N') && !util::compare(b,a,'N') );

		a = "NTCG";
		b = "ATCG";
		TS_ASSERT( !util::compare(a,b,'N') && !util::compare(b,a,'N') );

		a = "ATCG";
		b = "ATCG";
		TS_ASSERT( !util::compare(a,b,'N') && !util::compare(b,a,'N') );

		a = "ATCG";
		b = "CTCG";
		TS_ASSERT( !(!util::compare(a,b,'N') && !util::compare(b,a,'N')) );

		a = "ATCG";
		b = "CTCG";
		TS_ASSERT( !(!util::compare(a,b,'N') && !util::compare(b,a,'N')) );


		a = "IGHV3-23_ARTYSDFDV_IGHJ6";
		b = "IGHV3-23_ARTYSDFDV_IGHJ6";
		TS_ASSERT( !util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a) );

		a = "IGHV3-23_ARTYSXFDV_IGHJ6";
		b = "IGHV3-23_ARTYSDFDV_IGHJ6";
		TS_ASSERT( !util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a) );

		a = "IGHV3-23_ARTYSXFDV_IGHJ6";
		b = "IGHV3-23_CARTYSDFDV_IGHJ6";
		TS_ASSERT( !(!util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a)) );

		a = "IGHV3-23_ARTYSXFDV_IGHJ6";
		b = "IGHV3-21_ARTYSDFDV_IGHJ6";
		TS_ASSERT( !(!util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a)) );

		a = "IGHV3-23_ARTYSDFDV_IGHJ6";
		b = "IGHV3-21_ARTYSDFDV_IGHJ6";
		TS_ASSERT( !(!util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a)) );

		a = "IGHV3-23_ARTYSDFDV_IGHJ6";
		b = "IGHV3-23_ARTYSDFDV_IGHJ2";
		TS_ASSERT( !(!util::compare_clonotypes(a,b) && !util::compare_clonotypes(b,a)) );

		function< bool(string,string) > compareCorrectedSequences = 
			std::bind( &util::compare, 
					   placeholders::_1, 
					   placeholders::_2, 
					   'N'
		);
		map<string,int,function<bool(string,string)> > cmap( compareCorrectedSequences );

		cmap.insert( pair<string,int>( "ACTG", 1 ));
		cmap.insert( pair<string,int>( "NCTG", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 1 );
		cmap.clear();

		cmap.insert( pair<string,int>( "NCTG", 1 ));
		cmap.insert( pair<string,int>( "ACTG", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 1 );
		cmap.clear();

		cmap.insert( pair<string,int>( "NCTG", 1 ));
		cmap.insert( pair<string,int>( "ACTG", 1 ));
		cmap.insert( pair<string,int>( "ACAG", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 2 );
		cmap.clear();

		cmap.insert( pair<string,int>( "CCTG", 1 ));
		cmap.insert( pair<string,int>( "ACTG", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 2 );
		cmap.clear();

		cmap.insert( pair<string,int>( "CCTG", 1 ));
		cmap.insert( pair<string,int>( "TCTG", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 2 );
		cmap.clear();

		cmap.insert( pair<string,int>( "ACTG", 1 ));
		cmap.insert( pair<string,int>( "ACTGN", 1 ));
		TS_ASSERT_EQUALS( cmap.size(), 2 );
		cmap.clear();


	}
};



#endif /* UNIT_TEST_TESTUTIL_HH_ */
