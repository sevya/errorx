/*
 * TestLinking.hh
 *
 *  Created on: Sep 29, 2018
 *		Author: alexsevy
 */

#ifndef UNIT_TEST_TESTUTIL_HH_
#define UNIT_TEST_TESTUTIL_HH_

#include <cxxtest/TestSuite.h>

#include <iostream>

#include "errorx.hh"
#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"

using namespace std;
using namespace errorx;

class TestLinking : public CxxTest::TestSuite
{
public:

	void testLinking() {
		ErrorXOptions options( "testing/test.tsv", "tsv" );
		options.outfile( "testing/test_out.tsv" );
		options.species( "mouse" );
		options.errorx_base( "../" );
		
		run_protocol_write( options );

		SequenceRecords* records = errorx::run_protocol( options );

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

		delete records;

	}
		
};



#endif /* UNIT_TEST_TESTUTIL_HH_ */
