/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file filesystem.cc
@brief Utility functions for dealing with filesystem paths in
the ErrorX package
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <string>

#include "my_filesystem.hh"


/// set includes for filesystem stuff based on OS
#if defined(_WIN32) || defined(_WIN64)

#elif defined(__APPLE__) || defined(__MACH__)
	#include <mach-o/dyld.h>
#elif defined(__linux__)
	#include <unistd.h>
#endif


using namespace std;

namespace errorx {
namespace my_filesystem {


std::string filesep() {
	#if defined(_WIN32) || defined(_WIN64)
		return "\\";
	#else
		return "/";
	#endif
}

std::string get_executable_path() {
	#if defined(_WIN32) || defined(_WIN64)

	#elif defined(__APPLE__) || defined(__MACH__)
		char path[1024];
		uint32_t size = sizeof(path);
		if ( _NSGetExecutablePath(path, &size) == 0 ) {
			return std::string(path);
		} else {
			throw invalid_argument("Could not resolve executable path.");
		}
	#elif defined(__linux__)
		#include <unistd.h>
	#endif

	
}


path::path() : path_("") {}

path::path( std::string const & p ) : path_(p) {}

path path::operator/( path const & p ) { return path(path_ + filesep() + p.string()); }

path path::string() { return path_; }

} // namespace filesystem
} //namespace errorx