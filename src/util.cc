/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file util.cc
@brief Utility functions for the ErrorX package
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "util.hh"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>

#include <ctime>

#include "exceptions.hh"

using namespace std;

namespace errorx {
namespace util {

bool isint( string const & str ) {
	try {
		boost::lexical_cast<int>( str );
		return 1;
	} catch ( boost::bad_lexical_cast & e ) {
		return 0;
	}
}

bool isdouble( string const & str ) {
	try {
		boost::lexical_cast<double>( str );
		return 1;
	} catch ( boost::bad_lexical_cast & e ) {
		return 0;
	}
}

void write_vector( string & filename,
		vector<vector<string>> & vector2d,
		string & delimiter ) {

	  ofstream file;
	  file.open( filename );
	  for( int ii = 0; ii < vector2d.size(); ++ii ) {
		  for ( int jj = 0; jj < vector2d[ii].size(); ++jj ) {
			  file << vector2d[ ii ][ jj ] << delimiter;
		  }
		  file << "\n";
	  }
	  file.close();
	  return;
}

string translate( string & nt_sequence, int frame ) {

	map<string,string> codon_table = {
			{"TTT", "F"}, {"TTC", "F"}, {"TTA", "L"},
			{"TTG", "L"}, {"CTT", "L"}, {"CTC", "L"},
			{"CTA", "L"}, {"CTG", "L"}, {"ATT", "I"},
			{"ATC", "I"}, {"ATA", "I"}, {"ATG", "M"},
			{"GTT", "V"}, {"GTC", "V"}, {"GTA", "V"},
			{"GTG", "V"}, {"TCT", "S"}, {"TCC", "S"},
			{"TCA", "S"}, {"TCG", "S"}, {"CCT", "P"},
			{"CCC", "P"}, {"CCA", "P"}, {"CCG", "P"},
			{"ACT", "T"}, {"ACC", "T"}, {"ACA", "T"},
			{"ACG", "T"}, {"GCT", "A"}, {"GCC", "A"},
			{"GCA", "A"}, {"GCG", "A"}, {"TAT", "Y"},
			{"TAC", "Y"}, {"TAA", "*"}, {"TAG", "*"},
			{"CAT", "H"}, {"CAC", "H"}, {"CAA", "Q"},
			{"CAG", "Q"}, {"AAT", "N"}, {"AAC", "N"},
			{"AAA", "K"}, {"AAG", "K"}, {"GAT", "D"},
			{"GAC", "D"}, {"GAA", "E"}, {"GAG", "E"},
			{"TGT", "C"}, {"TGC", "C"}, {"TGA", "*"},
			{"TGG", "W"}, {"CGT", "R"}, {"CGC", "R"},
			{"CGA", "R"}, {"CGG", "R"}, {"AGT", "S"},
			{"AGC", "S"}, {"AGA", "R"}, {"AGG", "R"},
			{"GGT", "G"}, {"GGC", "G"}, {"GGA", "G"},
			{"GGG", "G"}
	};

	frame--;
	string inframe_seq = nt_sequence.substr( frame,nt_sequence.length()-frame );
	while ( inframe_seq.length()%3 != 0 ) inframe_seq = inframe_seq.substr( 0, inframe_seq.length()-1 );
	string aa_seq = "";

	for( int ii = 0; ii < inframe_seq.length(); ii+=3 ) {
		// if codon is not present in table (i.e. has N nucleotides),
		// replace with X
		if ( codon_table.find(inframe_seq.substr(ii,3)) 
			 == codon_table.end() ) {
			aa_seq += "X";
		} else {
			aa_seq += codon_table[ inframe_seq.substr(ii,3) ];
		}
	}
	return aa_seq;
}

string reverse( string & sequence ) {
	return string (sequence.rbegin(), sequence.rend());
}

/** Removed for windows compatibility
string exec(const char* cmd) {
	array<char, 128> buffer;
	string result;
	shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if (!pipe) throw runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}
*/

bool set_env(string key, string value) {
	#if defined(_WIN32) || defined(_WIN64)
		return SetEnvironmentVariable(key.c_str(), value.c_str());
	#elif defined(__APPLE__) || defined(__MACH__) || defined(__linux__)
		int result = setenv(key.c_str(), value.c_str(), 1);
		return result == 0;
	#endif
}

boost::filesystem::path get_home() {
	namespace fs = boost::filesystem;
	if ( get_os() == "win" ) {
		string drive = getenv("HOMEDRIVE");
		string path = getenv("HOMEPATH");
		return fs::path( drive ) / fs::path( path );
	} else {
		string home = getenv("HOME");
		return fs::path( home );
	}
}

boost::filesystem::path get_root_path() {

	namespace fs = boost::filesystem;
	fs::path path = boost::dll::program_location();

	return path.parent_path().parent_path();
}

double phred_avg_realspace( vector<int> const & phred_arr ) {
	double sum = 0;
	int count = 0;

	for ( int ii = 0; ii < phred_arr.size(); ++ii ) {
		if ( phred_arr[ii] >= 0 ) {
			sum += pow(10, (-(float)phred_arr[ii])/10);
			count++;
		}
	}
	double avg = float(sum)/float(count);
	return 10*-log10(avg);
}

string to_scientific( double a ) {
	// TODO potential overflow - fix this!
	char buffer [256];
	sprintf( buffer, "%.2E", a);
	string a_str = buffer;
	return a_str;
}

int count_queries( string & file ) {
	std::ifstream in(file);
	if ( !in.good() ) return 0;
	string line;
	int ii = 0;
	while( getline(in,line) ) {
		vector<string> tokens = util::tokenize_string<string>(line);
		if ( tokens.size() < 2 ) continue;
		if ( tokens[1] == "Query:" ) ++ii;
	}
	return ii;
}

int count_lines( string & file ) {
	std::ifstream in(file);
	if ( !in.good() ) return 0;
	string line;
	int ii = 0;
	char c;
	
	while (in.get(c)) {
		if (c == '\n') ++ii;
	}
	return ii+1;
}

void write_progress_bar( float progress, int done, int total ) {
	cout << "[";
	int barWidth = 70;
	int pos = barWidth * progress;

	for (int ii = 0; ii < barWidth; ++ii) {
		if (ii < pos) cout << "=";
		else if (ii == pos) cout << ">";
		else cout << " ";
	}

	cout << "] " << int(progress * 100.0) << "%: " << done << "/" << total << " records processed \r";
	cout.flush();
}


///////////// Encryption modules /////////////

char decrypt( char const c, char const key ) {
	return c ^ key;
}

char encrypt( char const c, int const shift ) {
	int newval = int(c) + shift;
	if ( newval > 126 ) newval = 33 + (newval - 126);
	return char(newval);
}

char decrypt( char const c, int const shift ) {
	int newval = int(c) - shift;
	if ( newval < 33 ) newval = 126 - (33-newval);
	return char(newval);
}

string encrypt_string( string const & str ) {

	vector<int> keys = {1,5,2,56,7,12,56,7,23,23,9};

	string out_str = "";
	for ( int ii = 0; ii < str.size(); ++ii ) {
		out_str += util::encrypt( str[ii], keys[ ii%(keys.size()) ] );
	}
	return out_str;
}

string decrypt_string( string const & chars ) {
	vector<int> keys = {1,5,2,56,7,12,56,7,23,23,9};
	string out_str = "";
	for ( int ii = 0; ii < chars.size(); ++ii ) {
		out_str += util::decrypt( chars[ ii ], keys[ ii%(keys.size()) ] );
	}
	return out_str;

}

void write_to_file( string const & fname, string const & message ) {
	ofstream out( fname.c_str() );
	out << message;
	out.close();
}

void encrypt_to_file( string const & fname, string const & message ) {
	ofstream out( fname.c_str() );
	string chars = encrypt_string( message );
	for ( int ii = 0; ii < chars.size(); ++ii ) out << chars[ ii ];
	out.close();
}

string decrypt_from_file( string const & fname ) {
	ifstream infile( fname );
	if ( !infile.good()) {
		throw invalid_argument("Error: file "+fname+" does not exist.");
	}
	char x;
	
	string chars;
	while (infile.get(x)) {
		chars += x;
	}
	infile.close();
	return decrypt_string( chars );	
}

string read_from_file( string const & fname ) {
	ifstream infile( fname );
	if ( !infile.good()) {
		throw invalid_argument("Error: file "+fname+" does not exist.");
	}
	string out;
	
	char x;
	while (infile.get(x)) out += x;

	infile.close();
	return out;	
}


///////////// END encryption modules /////////////


///////////// BEGIN date/time modules /////////////
vector<int> get_current_date() {
	time_t now = time(0);

	tm *ltm = localtime(&now);

	// print various components of tm structure.
	int year = 1900 + ltm->tm_year;
	int month = 1 + ltm->tm_mon;
	int day = ltm->tm_mday;


	vector<int> date = { year, month, day };
	return date;
}

string get_formatted_date( vector<int> offset ) {
	vector<int> date = get_current_date();
	for ( int ii = 0; ii < offset.size(); ++ii ) {
		date[ii] += offset[ii];
	}

	// TODO potential buffer overflow - fix this!
	char buffer1 [256];
	sprintf( buffer1, "%.2i", date[0]);
	string year_str = buffer1;
	
	char buffer2 [256];
	sprintf( buffer2, "%.2i", date[1]);
	string month_str = buffer2;

	char buffer3 [256];
	sprintf( buffer3, "%.2i", date[2]);
	string day_str = buffer3;

	return year_str+month_str+day_str;
}

vector<int> parse_formatted_date( string date ) {
	try {
		int year = stoi( date.substr( 0, 4 ));
		int month = stoi(date.substr( 4, 2 ));
		int day = stoi(date.substr( 6, 2 ));
		vector<int> date_vector = {
			year, month, day
		};
		return date_vector; 
	} catch ( exception & ) {
		// If there is a corrupt license
		throw BadLicenseException();
	}
}

void write_license( string key ) {
	
	string decoded = decrypt_string( key );
	vector<int> offset;
	if ( decoded == "YEAR" ) { 
		offset = {1,1,0};
	} else if ( decoded == "INFINITE" ) {
		offset = {1000,0,0};
	} else {
		throw BadLicenseException();
	}

	boost::filesystem::path config = util::get_home() / ".errorx";
	string date_fmt = get_formatted_date( offset );
	string date_encrypted = encrypt_string( date_fmt );

	write_to_file( config.string(), date_encrypted );
}

int julian( vector<int> & date ) {
	int m = (date[1] + 9) % 12;
	int y = date[0] - date[1]/10;
	int d = date[2];
	return (365*y) + (y/4) - (y/100) + (y/400) + (m*306 + 5)/10 + ( d - 1 );
}

bool valid_license() {
	
	boost::filesystem::path config = util::get_home() / ".errorx";

	string key;
	try {
		key = read_from_file( config.string() );
		vector<int> expire_date = parse_formatted_date( decrypt_string( key ));

		vector<int> current_date = get_current_date();

		int expire_J = julian(expire_date);
		int current_J = julian(current_date);

		return (expire_J - current_J) >= 0;

	} catch ( invalid_argument & ) {
		// license file does not exist - run in trial mode
		return 0;
	} catch ( BadLicenseException & ) {
		// license file is corrupted - run in trial mode
		return 0;
	}
}


///////////// END date/time modules /////////////


//////////// aggregation functions ////////////////////

bool compare( const string & a, const string & b, const char N ) {
		
	if ( a.length() != b.length() ) return a<b;

	string a_noN = "";
	string b_noN = "";

	for ( int ii = a.size()-1; ii >= 0; --ii ) {
		if ( a[ii] != N && b[ii] != N ) {
			a_noN += a[ii];
			b_noN += b[ii];
		}
	}

	return a_noN < b_noN;
}

void add_if_not_present( map<string,int,function<bool(string,string)> > & cmap, string const & key ) {
		
	map<string,int>::iterator it;
	it = cmap.find( key );
	if ( it == cmap.end() ) {
		cmap.insert( pair<string,int>( key, 1 ));
	} else {
		it->second += 1;
	}
}

vector<pair<string,int>> sort_map( map<string,int> const & cmap, bool ascending ) {
	// create a empty vector of pairs
	vector<pair<string,int>> vec;

	// copy key-value pairs from the map to the vector
	std::copy( cmap.begin(), cmap.end(), std::back_inserter<vector<pair<string,int>>>(vec));

	// sort the vector by increasing order of its pair's second value
	// if second value are equal, order by the pair's first value
	sort(vec.begin(), vec.end(),
			[ascending]( const pair<string,int>& a, const pair<string,int>& b ) {
				if ( ascending ) {
					if ( a.second != b.second ) return a.second < b.second;
					return a.first < b.first;
				} else {
					if ( a.second != b.second ) return a.second > b.second;
					return a.first > b.first;
				}
			});
	return vec;
}

vector<pair<string,int>> sort_map( map<string,int,function<bool(string,string)>> const & cmap, bool ascending ) {
	// create a empty vector of pairs
	vector<pair<string,int>> vec;

	// copy key-value pairs from the map to the vector
	std::copy( cmap.begin(), cmap.end(), std::back_inserter<vector<pair<string,int>>>(vec));

	// sort the vector by increasing order of its pair's second value
	// if second value are equal, order by the pair's first value
	sort(vec.begin(), vec.end(),
			[ascending]( const pair<string,int>& a, const pair<string,int>& b ) {
				if ( ascending ) {
					if ( a.second != b.second ) return a.second < b.second;
					return a.first < b.first;
				} else {
					if ( a.second != b.second ) return a.second > b.second;
					return a.first > b.first;
				}
			});
	return vec;
}

//////////// END aggregation functions ////////////////////

} // namespace util
} // namespace errorx
