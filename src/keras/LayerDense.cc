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

#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

namespace keras {

LayerDense::LayerDense() : 
	Layer( "Dense" ) 
	{}

void LayerDense::load_weights( ifstream & fin ) {
	fin >> input_cnt_ >> neurons_;
	double tmp_double;
	char tmp_char = ' ';
	for ( int i = 0; i < input_cnt_; ++i ) {
		vector<double> tmp_n;
		fin >> tmp_char; // for '['
		for ( int n = 0; n < neurons_; ++n ) {
			fin >> tmp_double;
			tmp_n.push_back( tmp_double );
		}
		fin >> tmp_char; // for ']'
		weights_.push_back( tmp_n );
	}

	fin >> tmp_char; // for '['
	for ( int n = 0; n < neurons_; ++n ) {
		fin >> tmp_double;
		bias_.push_back( tmp_double );
	}
	fin >> tmp_char; // for ']'
}

void LayerDense::load_weights( istringstream & fin ) {
	fin >> input_cnt_ >> neurons_;
	double tmp_double;
	char tmp_char = ' ';
	for ( int i = 0; i < input_cnt_; ++i ) {
		vector<double> tmp_n;
		fin >> tmp_char; // for '['
		for ( int n = 0; n < neurons_; ++n ) {
			fin >> tmp_double;
			tmp_n.push_back( tmp_double );
		}
		fin >> tmp_char; // for ']'
		weights_.push_back( tmp_n );
	}

	fin >> tmp_char; // for '['
	for( int n = 0; n < neurons_; ++n) {
		fin >> tmp_double;
		bias_.push_back( tmp_double );
	}
	fin >> tmp_char; // for ']'
}

DataChunk* LayerDense::compute_output( DataChunk* dc ) {
	//cout << "weights: input size " << m_weights.size() << endl;
	//cout << "weights: neurons size " << m_weights[0].size() << endl;
	//cout << "bias " << m_bias.size() << endl;
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

