/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/LayerActivation.cc
@brief Activation layer for neural network
@author Alex Sevy (alex@endeavorbio.com)
*/


#include "keras/LayerActivation.hh"
#include "keras/DataChunk2D.hh"
#include "keras/DataChunkFlat.hh"

#include "exceptions.hh"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

namespace keras {

LayerActivation::LayerActivation() : 
	Layer( "Activation" ) 
	{}

void LayerActivation::load_weights( ifstream & fin ) { fin >> activation_type_; }

void LayerActivation::load_weights( istringstream & fin ) { fin >> activation_type_; }

DataChunk* LayerActivation::compute_output( DataChunk* dc ) { 

	if ( dc->get_data_dim() == 3 ) {
		throw "Sorry, 3D data is not implemented (yet!)";

		/**
		vector<vector<vector<double>>> y = dc->get_3d();
		if ( activation_type_ == "relu" ) {
			for ( uint i = 0; i < y.size(); ++i ) {
				for ( uint j = 0; j < y[0].size(); ++j ) {
					for ( uint k = 0; k < y[0][0].size(); ++k ) {
						if( y[i][j][k] < 0 ) y[i][j][k] = 0;
					}
				}
			}
			DataChunk *out = new DataChunk2D();
			out->set_data(y);
			return out;
		} else {
			// TODO turn this into an exception
			throw "Activation type "+activation_type_+" not supported";
		}
		*/

	} else if ( dc->get_data_dim() == 1 ) { // flat data, use 1D
		vector<double> y = dc->get_1d();
		if ( activation_type_ == "relu" ) {
			for ( uint k = 0; k < y.size(); ++k ) {
				if ( y[k] < 0) y[k] = 0;
			}
		} else if ( activation_type_ == "softmax" ) {
			double sum = 0.0;
			for ( uint k = 0; k < y.size(); ++k ) {
				y[k] = exp(y[k]);
				sum += y[k];
			}
			for ( uint k = 0; k < y.size(); ++k ) {
				y[k] /= sum;
			}
		} else if ( activation_type_ == "sigmoid" ) {
			for ( uint k = 0; k < y.size(); ++k ) {
				y[k] = 1/(1+exp(-y[k]));
			}
		} else if ( activation_type_ == "tanh" ) {
			for ( uint k = 0; k < y.size(); ++k ) {
				y[k] = tanh(y[k]);
			}
		} else {
			// TODO turn this into an exception
			throw "Activation type "+activation_type_+" not supported";
		}

		keras::DataChunk *out = new DataChunkFlat();
		out->set_data( y );
		return out;
	} 

	else { throw "data dim not supported"; }

	return dc;
}

uint LayerActivation::get_input_rows() const { return 0; } // look for the value in the preceding layer
uint LayerActivation::get_input_cols() const { return 0; } // same as for rows
uint LayerActivation::get_output_units() const { return 0; }


} // namespace keras
