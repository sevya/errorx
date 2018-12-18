/**
@file util.hh
@brief Utility functions for the ErrorX package
@author Alex Sevy (alex.sevy@gmail.com)
*/

#ifndef UTIL_HH_
#define UTIL_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;

namespace errorx{
namespace util {

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
	for ( int ii = 0; ii < array.size(); ++ii ) {
		float_array.push_back( boost::lexical_cast<T>( array[ii] ));
	}
	return float_array;
}

/**
	Get name of OS 

	@return OS as a string, either windows, mac, or linux
*/	
inline string get_os() {
	#ifdef _WIN32
		return "win32";
	#elif _WIN64
		return "win64";
	#elif __APPLE__ || __MACH__
		return "mac";
	#elif __linux__
		return "linux";
	#endif

	throw invalid_argument("Error: OS type not recognized");
}

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
template <typename T>
string to_scientific( T a ) {
	char buffer [256];
	sprintf( buffer, "%.2E", a);
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
	Calculates metrics related to a DNA sequence. Will compute the 
	GC content and the level of SHM and return a std::pair of those 
	values in that order.

	@param sequence DNA sequence
	@param gl_sequence Germline DNA sequence

	@return pair of <GC_content,SHM>
*/	
pair<int,double> calculate_metrics( string & sequence, string & gl_sequence );

/**
	Execute a command in the shell and return its output

	@param cmd command to execute

	@return command output as a string
*/
string exec(const char* cmd);


/**
	Get the root path where the ErrorX directory is located

	@param from_gui whether ErrorX is being invoked from a GUI

	@return path to the base of the ErrorX directory
*/
boost::filesystem::path get_root_path( bool from_gui=0 );

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
	Writes a license key to the filesystem

	@param key the license key to write
*/
void write_license( string key );

/**
	Checks if there is a valid license key on the filesystem

	@return true if there is a valid license, 
	false if there is an invalid or no license
*/
bool valid_license();

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
vector<char> encrypt_string( string const & str );
string decrypt_string( vector<char> const & chars );

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
///////////// END Encryption and license checking modules /////////////

} // namespace util
} // namespace errorx

#endif /* UTIL_HH_ */

