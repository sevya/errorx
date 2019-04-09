/*
 * TestErrorXOptions.hh
 */

#ifndef TESTERRORXOPTIONS_HH_
#define TESTERRORXOPTIONS_HH_

#include <cxxtest/TestSuite.h>

#include "ErrorXOptions.hh"

using namespace std;
using namespace errorx;

class TestErrorXOptions : public CxxTest::TestSuite
{

public:

	void testBadFile(void) {
		ErrorXOptions options( "fakefile","fastq" );
		options = ErrorXOptions( "fakefile", "fastq" );
		
		TS_ASSERT_THROWS( 
			options.fastq_to_fasta(),
			invalid_argument
			);

		options = ErrorXOptions();
		TS_ASSERT_THROWS( 
			options.validate(),
			invalid_argument
			);

		TS_ASSERT_THROWS( 
			ErrorXOptions( "fakefile","" ),
			invalid_argument
			);

		TS_ASSERT_THROWS( 
			options.format( "TSV" ),
			invalid_argument
			);

		TS_ASSERT_THROWS( 
			options.igtype( "IG" ),
			invalid_argument
			);

		TS_ASSERT_THROWS( 
			options.nthreads( 0 ),
			invalid_argument
			);

		TS_ASSERT_THROWS( 
			options.nthreads( -2 ),
			invalid_argument
			);

		options.error_threshold( 0.3 );
		TS_ASSERT_EQUALS( options.error_threshold(), 0.3 );

		options.allow_nonproductive( 0 );
		TS_ASSERT_EQUALS( options.allow_nonproductive(), 0 );
	}

};

#endif /* TESTERRORXOPTIONS_HH_ */

