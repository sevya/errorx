/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file constants.hh
@brief Numerical constants used during the ErrorX protocol
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef CONSTANTS_HH_
#define CONSTANTS_HH_

namespace errorx {
namespace constants {

/** 
	Optimized threshold for best balance between precision and recall
	this value may change in future releases but I want it to be 
	defined here
*/
const double OPTIMIZED_THRESHOLD = 0.556956;
const double OPTIMIZED_PRECISION =  0.75;
const double OPTIMIZED_RECALL = 0.4393;

const std::vector<double> OPTIMIZED_THRESHOLD_VECTOR = {
		0.291825, // precision 0.50, recall 0.59
		0.449443, // precision 0.66, recall 0.49
		OPTIMIZED_THRESHOLD, 
		0.726101, // precision 0.85, recall 0.34
		0.910069  // precision 0.95, recall 0.20
		};

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


/**
	The number of queries you can run for free without
	a license
*/
const int FREE_QUERIES = 100;

} // namespace constants
} // namespace errorx

#endif // CONSTANTS_HH_
