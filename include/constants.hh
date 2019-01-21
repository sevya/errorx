/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file constants.hh
@brief Numerical constants used during the ErrorX protocol
@author Alex Sevy (alex.sevy@gmail.com)
*/

namespace errorx {
namespace constants {

/** 
	Optimized threshold for best balance between precision and recall
	this value may change in future releases but I want it to be 
	defined here
*/
const double OPTIMIZED_THRESHOLD = 0.805312;
const double OPTIMIZED_PRECISION =  0.668;
const double OPTIMIZED_RECALL = 0.402;

/**
	Sequence window used for feature calculation
*/
const int WINDOW = 8;

/**
	E value cutoffs when assigning V, D, and J genes
*/
const double V_EVALUE_CUTOFF = 1e-3;
const double D_EVALUE_CUTOFF = 0.01;
const double J_EVALUE_CUTOFF = 0.01;



} // namespace constants
} // namespace errorx
