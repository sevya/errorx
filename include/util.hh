/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file util.hh
@brief Utility functions for the ErrorX package
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef UTIL_HH_
#define UTIL_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;

namespace errorx{
namespace util {

/**
	Use boost::lexical_cast to test if a string is an integer or double
	
	@param str input string

	@return bool if it is an int/double
*/
bool isint( string const & str );
bool isdouble( string const & str );

	
/**
	Break a string into tokens based on the provided delimiter. 
	When multiple characters are present in the delimiter it 
	will split by any of them (e.g. ", " will split on spaces
	and commas). Then casts the elements to type T

	@param str string to split
	@param delim characters to use as a delimiter

	@return vector of elements of type T
*/	
template<typename T>
vector<T> tokenize_string( string str, string delim="\t " ) {

	vector<string> array;
	vector<T> float_array;

	boost::split( array, str, boost::algorithm::is_any_of(delim), boost::token_compress_on );
        for ( size_t ii = 0; ii < array.size(); ++ii ) {
		float_array.push_back( boost::lexical_cast<T>( array[ii] ));
	}
	return float_array;
}

/**
	Get name of OS 

	@return OS as a string, either windows, mac, or linux
*/	
inline string get_os() {
	#if defined(_WIN32) || defined(_WIN64)
		return "win";
	#elif defined(__APPLE__) || defined(__MACH__)
		return "mac";
	#elif defined(__linux__)
		return "linux";
	#endif

	throw invalid_argument("Error: OS type not recognized");
}

/** 
	Set environmental variable, depending on the OS type

	@param key the name of the variable to set
	@param value the value to set it to

	@return 1 if operation was successful, 0 if not
*/
bool set_env(string key, string value);

/**
	Get home directory based on the OS 

	@return path of the home directory
*/	
boost::filesystem::path get_home();

/**
	Split a vector into N equally divided chunks 

	@param vect vector to split
	@param nchunks how many chunks to split into

	@return a vector of length nchunks containing the elements of vect
*/	
template <typename T>
vector<vector<T>> split_vector( vector<T> const & vect, int nchunks ) {
	int chunksize = (int)ceil( (double)vect.size()/nchunks );
	int ii = 0;
	vector<vector<T>> outvector( nchunks );
	int current_chunk = 0;

	while ( ii < vect.size() ) {
		vector<T> subvector;
		for ( int jj = 0; jj < chunksize; ++jj ) {
			if ( ii == vect.size() ) {
				break;
			} else {
				subvector.push_back( vect[ii] );
				++ii;
			}
		}
		outvector[current_chunk] = subvector;
		current_chunk++;
	}
	return outvector;
}

/**
	Convert a numeric type to a string in scientific notation

	@param a number to convert

	@return string of number in scientific notation
*/	
string to_scientific( double a );

/**
	Round a numeric type to a certain number of digits

	@param a number to convert

	@return string of rounded number
*/
template <typename T>
string rounded_string( T a ) {
	// TODO potential overflow - fix this!
	char buffer [256];
	sprintf( buffer, "%.2f", a);
	string a_str = buffer;
	return a_str;
}

/**
	Writes a 2d vector to a file

	@param filename file to write to
	@param vector2d vector to write out
	@param delimiter string to separate vector elements
*/	
void write_vector( string & filename, vector<vector<string>> & vector2d, string & delimiter);

/**
	Translate a DNA sequence to protein

	@param nt_sequence DNA sequence to translate
	@param frame Frame to use for translation - either 1, 2, or 3
	
	@return protein sequence
*/	
string translate( string & nt_sequence, int frame );

/**
	Reverse a string. Returns new copy, not in-place

	@param sequence String to reverse

	@return Reversed string
*/	
string reverse( string & sequence );


/**
	Execute a command in the shell and return its output

	@param cmd command to execute

	@return command output as a string
*/
/** Removed for windows compatibility
string exec(const char* cmd);
*/


/**
	Get the root path where the ErrorX directory is located

	@return path to the base of the ErrorX directory
*/
boost::filesystem::path get_root_path();

/**
	Average PHRED quality scores in realspace. Since they are 
	log values, they have to be transformed into realspace,
	averaged, then transformed back to log space

	@param phred_arr Array of PHRED quality scores

	@return average PHRED score in log space
*/
double phred_avg_realspace( vector<int> const & phred_arr );

/**
	Counts the number of lines in a file

	@param file file to count 

	@return # lines in that file
*/
int count_lines( string & file );

/**
	Counts the number of queries in an IGBlast output file

	@param file file to count 

	@return # queries in that file
*/
int count_queries( string & file );

/**
	Writes a progress bar to the screen while a process is 
	happening in anothet thread

	@param progress fraction finished the process is
	@param done int # of items finished so far
	@param total total # of items to process
*/
void write_progress_bar( float progress, int done, int total );



///////////// Encryption and license checking modules /////////////

/**
	Encrypt or decrypt a character
	@return encrypted/decrypted character
*/
char encrypt( char const c, int const shift );
char decrypt( char const c, int const shift );

/**
	Encrypt or decrypt a string. Strings are encrypted
	to vectors of chars, vectors of chars are decrypted
	to strings
*/
string encrypt_string( string const & str );
string decrypt_string( string const & chars );

/**
	Encrypt a string and write to a file

	@param fname file to write to
	@param message key to encrypt and write
*/
void encrypt_to_file( string const & fname, string const & message );

/**
	Write unencrypted string to a file

	@param fname file to write to
	@param message string to write
*/
void write_to_file( string const & fname, string const & message );

/**
	Decrypt the string contained in a file

	@param fname file to read from
*/
string decrypt_from_file( string const & fname );

/**
	Read the string contained in a file

	@param fname file to read from
*/
string read_from_file( string const & fname );

///////////// END Encryption and license checking modules /////////////


//////////// BEGIN licensing based on date modules ////////////////////
/**
	Gets current date as a vector of ints
	where the vector is { year, month, day }

	@return vector of ints representing date
*/
vector<int> get_current_date();

/**
	Gets date as a formatted string in the format 
	20190206. Takes in an offset to increment the
	date before formatting, in the form of a 
	vector { year, month, day }

	@param offset Amount of time to offset the
	current date before formatting

	@return formatted string representing date
*/
string get_formatted_date( vector<int> offset );

/**
	Parses a formatted date to get the int values. 
	Takes a string of format 20190206. 

	@param date string representing date

	@return vector of ints representing date
*/
vector<int> parse_formatted_date( string date );

/**
	Write a license appending the current date to the key
	Takes in an offset to increment the
	date before formatting, in the form of a 
	vector { year, month, day }

	@param key key to write
	@param vector of ints representing date
*/
void write_license( string key );

/**
	Calculates Julian date for a given date vector
	date vector is of form {year, month, day}

	@param date date to calculate
	@return int julian date
*/
int julian( vector<int> & date );

/**
	Checks if a license is valid based on date and cipher

	@return boolean value true if license is valid
*/
bool valid_license();
//////////// END licensing based on date modules ////////////////////



//////////// aggregation functions ////////////////////

/**
	Compares sequences ignoring the correction character

	@param N correction character
	@return boolean if strings are equal
*/
bool compare( const string & a, const string & b, const char N );

/**
	Adds key to map if not present with a value of 1. If it is present
	increments the value by one

	@param cmap map to check
	@param key key to insert
*/
void add_if_not_present( map<string,int,function<bool(string,string)> > & cmap, string const & key );

vector<pair<string,int>> sort_map( map<string,int> const & cmap, bool ascending=1 );
vector<pair<string,int>> sort_map( map<string,int,function<bool(string,string)>> const & cmap, bool ascending );

//////////// END aggregation functions ////////////////////

} // namespace util
} // namespace errorx

#endif /* UTIL_HH_ */

