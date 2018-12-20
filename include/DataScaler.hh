/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file DataScaler.hh
@brief A simple class to scale features before prediction
using a neural network
@details This is not general purpose scaler, it is hardcoded
specifically to scale data in one context
@author Alex Sevy (alex.sevy@gmail.com)
*/


#ifndef INCLUDE_DATASCALER_HH_
#define INCLUDE_DATASCALER_HH_

#include <string>
#include <vector>

using namespace std;

namespace errorx {

class DataScaler {

public:
	/**
		Empty constructor.
	*/	
	DataScaler();

	/**
		Scales input features to normalize between 0 and 1. 
		Expects feature_vector to have 228 entries

		@param feature_vector features to scale
	*/	
	const vector<double> scale_data( vector<double> const & feature_vector );

};

} // namespace errorx


#endif /* INCLUDE_DATASCALER_HH_ */
