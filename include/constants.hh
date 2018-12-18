/**
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
const double OPTIMIZED_THRESHOLD = 0.884311;
const double OPTIMIZED_PRECISION =  0.663;
const double OPTIMIZED_RECALL = 0.448;

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
	The longest antibody sequence in theory is around
	420 NTs, if you're using the longest available V,
	D, and J. It's possible it could be longer with a
	lot of non-templated NTs but this is a reasonable
	upper bound
*/
const int MAX_AB_LENGTH = 420;

} // namespace constants
} // namespace errorx