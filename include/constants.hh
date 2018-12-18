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


} // namespace constants
} // namespace errorx