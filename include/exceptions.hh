/**
@file exceptions.hh
@brief Class holds custom exception types used in ErrorX
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <iostream>
#include <exception>

namespace errorx {

/**
	Exception is thrown when there is no valid license
	file found, and ErrorX is therefore running in 
	trial mode
*/	
class InvalidLicenseException : public std::exception {
public:
	const char * what () const throw ()
    {
    	return "You are currently running the trial version of ErrorX, "
				"which can only process 500 sequences at a time. "
				"Please limit your query size or request access to the full version.";
    }
};

} // namespace errorx