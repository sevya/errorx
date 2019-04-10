

#ifndef UNITTESTS_HH_
#define UNITTESTS_HH_

#include <cxxtest/TestSuite.h>
#include <exception>
#include <sstream>

#include "exceptions.hh"
#include "keras/KerasModel.hh"
#include "keras/LayerActivation.hh"
#include "keras/LayerDense.hh"
#include "keras/DataChunkFlat.hh"

#include "ErrorXOptions.hh"

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
		return; 
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


		ErrorXOptions options( "tmp.fastq", "fastq" );
		options.errorx_base( "../" );
		
		KerasModel model2( options );
		TS_ASSERT_EQUALS( model2.get_input_rows(), 1 );
		TS_ASSERT_EQUALS( model2.get_input_cols(), 228 );
		TS_ASSERT_EQUALS( model2.get_output_length(), 1);
		TS_ASSERT_EQUALS( model2.no_layers(), 12 );
		
		for ( int ii = 0; ii < 12; ++ii ) {
			if ( ii%2 == 0 ) {
				TS_ASSERT_EQUALS( model.layer( ii )->get_name(), "Dense" );
			} else {
				TS_ASSERT_EQUALS( model.layer( ii )->get_name(), "Activation" );
			}
		}

		TS_ASSERT_EQUALS( model2.compute_output( dc_ )[ 0 ], 0.027573684313656657 );
	}

	void testStringModels(void) {
		KerasModel model;
		model.verbose( 1 );
		// TODO check for uninitialized model
		string model_str;
		DataChunk* chunk = new DataChunkFlat();
		vector<double> output;
		vector<double> dat = {0.5, 0.6};
		chunk->set_data( dat );
		
		// Check output with no activation
		model_str = 
			"layers 1\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]";

		model.load_weights_from_string( model_str );

		output = model.compute_output( chunk );

		TS_ASSERT_DELTA( output[0], 1.25, 4 );
		TS_ASSERT_DELTA( output[1], 0.56, 4 );
		TS_ASSERT_DELTA( output[2], 0.68, 4);
		TS_ASSERT_DELTA( output[3], 0.80, 4 );

		// Check output with sigmoid activation
		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		model.load_weights_from_string( model_str );
		
		TS_ASSERT_EQUALS( model.get_input_rows(), 1 );
		TS_ASSERT_EQUALS( model.get_input_cols(), 2 );
		TS_ASSERT_EQUALS( model.get_output_length(), 4);
		TS_ASSERT_EQUALS( model.no_layers(), 2 );

		output = model.compute_output( chunk );

		TS_ASSERT_DELTA( output[0], 0.777299861175, 9 );
		TS_ASSERT_DELTA( output[1], 0.636452540282, 9 );
		TS_ASSERT_DELTA( output[2], 0.663738697404, 9 );
		TS_ASSERT_DELTA( output[3], 0.689974481128, 9 );


		// Check output with tanh activation
		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"tanh";

		model.load_weights_from_string( model_str );

		output = model.compute_output( chunk );

		TS_ASSERT_DELTA( output[0], 0.84828364, 9 );
		TS_ASSERT_DELTA( output[1], 0.507977433, 9 );
		TS_ASSERT_DELTA( output[2], 0.591519395, 9 );
		TS_ASSERT_DELTA( output[3], 0.66403677, 49 );


		// Check output with softmax activation
		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"softmax";

		model.load_weights_from_string( model_str );

		output = model.compute_output( chunk );
		TS_ASSERT_DELTA( output[0], 0.369722718, 9 );
		TS_ASSERT_DELTA( output[1], 0.185444067, 9 );
		TS_ASSERT_DELTA( output[2], 0.209087602, 9 );
		TS_ASSERT_DELTA( output[3], 0.235745613, 9 );


		// Check output with relu activation
		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"relu";

		model.load_weights_from_string( model_str );
		dat = { -5.6, 0.6 };
		chunk->set_data( dat );

		output = model.compute_output( chunk );

		TS_ASSERT_DELTA( output[0], 0.64, 4 );
		TS_ASSERT_DELTA( output[1], 0, 4 );
		TS_ASSERT_DELTA( output[2], 0, 4);
		TS_ASSERT_DELTA( output[3], 0, 4 );

		
		
	}

	void testSanityChecks(void) {
		/** 
		TODO set up sanity checks:
		layer number at the top matches num layers
		input data is correct dimensions for model
		*/
		KerasModel model;
		model.verbose( 1 );
		// TODO check for uninitialized model
		string model_str;
		DataChunk* chunk = new DataChunkFlat();
		vector<double> output;
		chunk->set_data( vector<double>{ 0.5, 0.6 } );
		
		// Check that model throws error when uninitialized
		TS_ASSERT_THROWS( model.compute_output( chunk ), ObjectNotInitialized );
		TS_ASSERT_THROWS( model.get_input_rows(), ObjectNotInitialized );
		TS_ASSERT_THROWS( model.get_input_cols(), ObjectNotInitialized );
		TS_ASSERT_THROWS( model.get_output_length(), ObjectNotInitialized );

		// Check if # layers is inconsistent
		model_str = 
			"layers 4\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);

		model_str = 
			"layers 1\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);

		// Check if weights are not the correct dimensions
		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4  0.5 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);

		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);


		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[ 0.1  0.2 ]\n[ 0.3  0.4 ]\n"
			"[ 0.5  0.6 ]\n[ 0.7  0.8 ]\n"
			"[ 0.9  0.1 ]\n[ 0.11 0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);

		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);

		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[0.1  0.2  0.3  0.4 \n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";

		TS_ASSERT_THROWS( 
			model.load_weights_from_string( model_str ), 
			BadModel 
			);



		// Check if data passed in is the correct dimensions

		model_str = 
			"layers 2\n"
			"layer 0 Dense\n"
			"2 4\n"
			"[0.1  0.2  0.3  0.4 ]\n"
			"[ 0.5  0.6  0.7  0.8 ]\n"
			"[ 0.9  0.1  0.11  0.12 ]\n"
			"layer 1 Activation\n"
			"sigmoid";
		model.load_weights_from_string( model_str );
		output = model.compute_output( chunk );

		chunk->set_data( vector<double>{ 0.5, 0.6, 0.3 } );
		TS_ASSERT_THROWS( model.compute_output( chunk ), BadModel );

		chunk->set_data( vector<double>{ 0.5 } );
		TS_ASSERT_THROWS( model.compute_output( chunk ), BadModel );

		// Check what happens if you pass in empty data chunk
	}



	void testDataChunks(void) {
		DataChunkFlat dc_flat;
		try {
			dc_flat.get_3d();
			TS_ASSERT(0);
		} catch ( FunctionNotImplemented & exc ) {
			TS_ASSERT_EQUALS( exc.what(), "function get_3d not implemented" );
		}		
	}

	// void testLayer(void) {
	// 	// build a layer and destroy it
	// 	Layer l = new Layer();
	// 	delete l;

	// 	LayerDense ld = new LayerDense();
	// 	delete ld;

	// 	LayerActivation la = new LayerActivation();
	// 	delete la;
	// }

	DataChunkFlat* dc_;
};

#endif /* UNITTESTS_HH_ */

