/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file filesystem.hh
@brief Utility functions for dealing with filesystem paths in
the ErrorX package
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <string>

namespace errorx {
namespace my_filesystem {

class path {
	path();

	path( std::string const & path );

	path operator/( path const & p );

	path string();

private:
	std::string path_;
};



} // namespace filesystem
} //namespace errorx
