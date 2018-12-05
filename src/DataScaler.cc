/**
@file DataScaler.cc
@brief A simple class to scale features before prediction
using a neural network
@details This is not general purpose scaler, it is hardcoded
specifically to scale data in one context
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <fstream>

#include "DataScaler.hh"
#include "util.hh"
#include "model.hh"

using namespace std;

namespace errorx {

DataScaler::DataScaler() 
{}

const vector<double> DataScaler::scale_data( vector<double> const & feature_vector ) {

	vector<double> scaled_vector( feature_vector );
	
	// scale features - all are pre-scaled except for PHRED-based features
	// those should be divided by the maximum phred score (40)
	vector<int> indices_to_scale = {
		3,4,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123
	};

	for ( int ii = 0; ii < indices_to_scale.size(); ++ii ) {
		// maximum phred score is 40 - this way we normalize between 0-1
		scaled_vector[ ii ] /= 40.0;
	}

	return scaled_vector;
}

} //namespace errorx

