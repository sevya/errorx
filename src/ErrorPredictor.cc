/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ErrorPredictor.hh
@brief Runs neural network prediction on DNA sequence features
@details Features of a DNA sequence are generated by SequenceFeatures,
then this class is used to make a NN prediction
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "ErrorPredictor.hh"
#include "ErrorXOptions.hh"
#include "SequenceFeatures.hh"
#include "DataScaler.hh"
#include "keras_model.hh"
#include "util.hh"

using namespace std;

namespace errorx {

ErrorPredictor::ErrorPredictor( ErrorXOptions const & options ) :
		options_( options ),
		keras_model_( options )
{}

ErrorPredictor::ErrorPredictor( ErrorPredictor const & other ) :
		options_( other.options_ ),
		keras_model_( other.options_ )
{}



double ErrorPredictor::apply_model( SequenceFeatures const & features ) const {
	using namespace keras;

	if ( features.is_germline() ) return 0.0;
	
	DataScaler scaler;

	const vector<double> scaled_vector = scaler.scale_data( features.get_feature_vector() );

	DataChunkFlat* data = new DataChunkFlat();

	data->set_data( scaled_vector );

	vector<double> output = keras_model_.compute_output( data );

	delete data;

	return output[0];
	
}

vector<double> ErrorPredictor::apply_model( vector<vector<double>> const feature_vector ) const {
	using namespace keras;

	DataChunkFlat* data = new DataChunkFlat();
	DataScaler scaler;

	vector<double> output;

	for ( int ii = 0; ii < feature_vector.size(); ++ii ) {
		const vector<double> scaled_data = scaler.scale_data( feature_vector[ii] );
		data->set_data( scaled_data );
		vector<double> out = keras_model_.compute_output( data );
		output.push_back( out[0] );
	}

	delete data;

	return output;
}

} // namespace errorx
