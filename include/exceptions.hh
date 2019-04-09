/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file exceptions.hh
@brief Class holds custom exception types used in ErrorX
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <iostream>
#include <exception>
#include <string>

using namespace std;

namespace errorx {

/**
	Exception is thrown when there is no valid license
	file found, and ErrorX is therefore running in 
	trial mode
*/	
class InvalidLicenseException : public exception {
public:
	const char* what () const throw () {
		return "You are currently running the trial version of ErrorX, "
				"which can only process 100 sequences at a time. "
				"Please limit your query size or contact alex@endeavorbio.com "
				"to request access to the full version.";
	}
};


/**
	Exception is thrown when a bad license is given on the command line
*/	
class BadLicenseException : public exception {
public:
	const char* what () const throw () {
		return "License is not valid. Please contact alex@endeavorbio.com for assistance";
	}
};

} // namespace errorx


/** 
	Exceptions thrown by the Keras module
*/
namespace keras {

/**
	Exception is thrown when an unimplemented function is called
*/	
class FunctionNotImplemented : public exception {
public:
	FunctionNotImplemented() : message_("Sorry, you called a function that's not implemented yet!") {}
	FunctionNotImplemented( string const & message ) : message_( message ) {}

	const char* what() const throw() {
		return message_.c_str();
	}

	string message_;
};

/**
	Exception is thrown when a layer type is used that is not support
*/	
class InvalidLayer : public exception {
public:
	InvalidLayer( string const & layer ) : layer_( layer ) {}

	const char* what() const throw() {
		string message = "Layer type "+layer_+" not implemented";
		return message.c_str();
	}

	string layer_;
};


} // namespace keras