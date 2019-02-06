/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file exceptions.hh
@brief Class holds custom exception types used in ErrorX
@author Alex Sevy (alex@endeavorbio.com)
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
	const char* what () const throw ()
	{
		return "You are currently running the trial version of ErrorX, "
				"which can only process 500 sequences at a time. "
				"Please limit your query size or request access to the full version.";
	}
};


/**
	Exception is thrown when a bad license is given on the command line
*/	
class BadLicenseException : public std::exception {
public:
	const char* what () const throw ()
	{
		return "License is not valid. Please contact help@endeavorbio.com for assistance";
	}
};

} // namespace errorx
