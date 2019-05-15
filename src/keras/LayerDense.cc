/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/LayerDense.cc
@brief Dense layer for a neural network
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "keras/LayerDense.hh"
#include "keras/DataChunkFlat.hh"
#include "keras/DataChunk2D.hh"

#include "util.hh"

#include <istream>
#include <iostream>

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace errorx;

namespace keras {

LayerDense::LayerDense() : 
	Layer( "Dense" ) 
	{}

void LayerDense::load_weights( istream & fin ) {
	fin >> input_cnt_ >> neurons_;
	// double tmp_double;
	char tmp_char = ' ';

	string tmp_double;

	// iterate through input dimension
	for ( int i = 0; i < input_cnt_; ++i ) {
		vector<double> tmp_n;
		fin >> tmp_char; // for '['
		
		// check that data truly starts here like it should
		if ( tmp_char != '[' ) {
			throw BadModel(
			"Error: bad data in neuron weights. Make sure all lines "
			"begin with [ and end with ] and that each neuron is on "
			"its own line"
			);
		}

		// iterate through neurons in each input dim
		for ( int n = 0; n < neurons_; ++n ) {
			fin >> tmp_double;

			if ( !util::isdouble(tmp_double) ) {
				throw BadModel("bad value: "+tmp_double+" needs to be a double");
			}
			tmp_n.push_back( 
				boost::lexical_cast<double>(tmp_double) 
				);
		}

		fin >> tmp_char; // for ']'
		// check that data truly ends here like it should
		if ( tmp_char != ']' ) {
			throw BadModel(
			"Error: bad data in neuron weights. Make sure all lines "
			"begin with [ and end with ] and that each neuron is on "
			"its own line"
			);
		}

		weights_.push_back( tmp_n );
	}

	fin >> tmp_char; // for '['
	// check that data truly starts here like it should
	if ( tmp_char != '[' ) {
		throw BadModel(
		"Error: bad data in bias weights. Make sure all lines "
		"begin with [ and end with ] and that each neuron is on "
		"its own line"
		);
	}

	for ( int n = 0; n < neurons_; ++n ) {
		fin >> tmp_double;
		if ( !util::isdouble(tmp_double) ) throw BadModel();
		bias_.push_back( 
			boost::lexical_cast<double>(tmp_double) 
			);
	}
	fin >> tmp_char; // for ']'
	if ( tmp_char != ']' ) {
		throw BadModel(
		"Error: bad data in bias weights. Make sure all lines "
		"begin with [ and end with ] and that each neuron is on "
		"its own line"
		);
	}
}


DataChunk* LayerDense::compute_output( DataChunk* dc ) {

	int size = weights_[0].size();
	int size8 = size >> 3;
	keras::DataChunkFlat* out = new DataChunkFlat( size, 0 );
	double * y_ret = out->get_1d_rw().data();

	vector<double> const & im = dc->get_1d();

	for ( int j = 0; j < weights_.size(); ++j ) { // iter over input
		const double * w = weights_[j].data();
		double p = im[j];
		int k = 0;
		for ( int i = 0; i < size8; ++i ) { // iter over neurons
			y_ret[k]   += w[k]	 * p;			// vectorize if you can
			y_ret[k+1] += w[k+1] * p;
			y_ret[k+2] += w[k+2] * p;
			y_ret[k+3] += w[k+3] * p;
			y_ret[k+4] += w[k+4] * p;
			y_ret[k+5] += w[k+5] * p;
			y_ret[k+6] += w[k+6] * p;
			y_ret[k+7] += w[k+7] * p;
			k += 8;
		}
		while ( k < size ) { y_ret[k] += w[k] * p; ++k; }
	}
	for ( int i = 0; i < size; ++i ) { // add biases
		y_ret[i] += bias_[i];
	}

	return out;
}

uint LayerDense::get_input_rows() const { return 1; } // flat, just one row
uint LayerDense::get_input_cols() const { return input_cnt_; }
uint LayerDense::get_output_units() const { return neurons_; }

void LayerDense::print_weights() { 
	for ( int ii = 0; ii < weights_.size(); ++ii ) {
		for ( int jj = 0; jj < weights_[ii].size(); ++jj ) {
			cout << ii << "," << jj << " : " << weights_[ ii ][ jj ] << endl;
		}
	}
}


} // namespace keras

