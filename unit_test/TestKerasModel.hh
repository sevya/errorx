

#ifndef UNITTESTS_HH_
#define UNITTESTS_HH_

#include <cxxtest/TestSuite.h>
#include <exception>

#include "keras/KerasModel.hh"
#include "keras/LayerActivation.hh"
#include "keras/LayerDense.hh"
#include "keras/DataChunkFlat.hh"

using namespace std;
using namespace errorx;
using namespace keras;

class TestKerasModel : public CxxTest::TestSuite
{
public:

	void setUp() {
		vector<double> data = { 0.63535911602209949, 0.35294117647058826, 0.077207137977055973, 0.097010460385836522, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.050000000000000003, 0.85000000000000009, 0.625, 0.70000000000000007, 0.60000000000000009, 0.77500000000000002, 0.55000000000000004, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.18232044198895028 };
		dc_ = new DataChunkFlat();
		dc_->set_data( data );
	}

	void tearDown() {
		delete dc_;
	}

	void testModel(void) {
		TS_ASSERT_THROWS( KerasModel( "model.nnet" ), invalid_argument );
		
		KerasModel model( "../model.nnet" );
		TS_ASSERT_EQUALS( model.get_input_rows(), 1 );
		TS_ASSERT_EQUALS( model.get_input_cols(), 228 );
		TS_ASSERT_EQUALS( model.get_output_length(), 1);
		TS_ASSERT_EQUALS( model.no_layers(), 12 );
		
		for ( int ii = 0; ii < 12; ++ii ) {
			if ( ii%2 == 0 ) {
				TS_ASSERT_EQUALS( model.layer( ii )->get_name(), "Dense" );
			} else {
				TS_ASSERT_EQUALS( model.layer( ii )->get_name(), "Activation" );
			}
		}

		vector<double> output = model.compute_output( dc_ );
		TS_ASSERT_EQUALS( output.size(), 1 );

		TS_ASSERT_EQUALS( output[ 0 ], 0.027573684313656657 );
	}

	DataChunkFlat* dc_;
};

#endif /* UNITTESTS_HH_ */

