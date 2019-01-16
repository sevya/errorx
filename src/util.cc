/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file util.cc
@brief Utility functions for the ErrorX package
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "util.hh"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>

using namespace std;

namespace errorx {
namespace util {

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

// Returns pair consisting of (GC_count, SHM)
pair<int,double> calculate_metrics( string & sequence, string & gl_sequence ) {
	int mutations = 0;
	int gc_count = 0;
	for ( int ii = 0; ii < sequence.length(); ++ii ) {
		if ( sequence[ii] != gl_sequence[ii] && gl_sequence[ii] != '-' ) mutations++;
		if ( sequence[ii] == 'G' || sequence[ii] == 'C' ) gc_count++;
	}
	return pair<int,double> (gc_count, (double)mutations/(double)sequence.length());
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

boost::filesystem::path get_root_path( bool from_gui ) {

	namespace fs = boost::filesystem;
	fs::path path = boost::dll::program_location();

	// TODO adapt this for Windows
	if ( from_gui ) {
		fs::path app_bundle = path.parent_path().parent_path();
		return app_bundle / "Resources";
	} else { 
		return path.parent_path().parent_path();
	}
}

double phred_avg_realspace( vector<int> const & phred_arr ) {
	double sum = 0;
	int count = 0;

	for ( int ii = 0; ii < phred_arr.size(); ++ii ) {
		if ( phred_arr[ii] != -1 ) {
			sum += pow(10, (-(float)phred_arr[ii])/10);
			count++;
		}
	}
	double avg = float(sum)/float(count);
	return 10*-log10(avg);
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

vector<char> encrypt_string( string const & str ) {

	vector<int> keys = {1,5,2,56,7,12,56,7,23,23,9};

	vector<char> out_vector( str.size() );
	for ( int ii = 0; ii < str.size(); ++ii ) {
		char x = str[ ii ];
		out_vector[ ii ] = util::encrypt( x, keys[ ii%(keys.size()) ] );
	}
	return out_vector;
}

string decrypt_string( vector<char> const & chars ) {
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
	vector<char> chars = encrypt_string( message );
	for ( int ii = 0; ii < chars.size(); ++ii ) out << chars[ ii ];
	out.close();
}

string decrypt_from_file( string const & fname ) {
	ifstream infile( fname );
	if ( !infile.good()) {
		throw invalid_argument("Error: file "+fname+" does not exist.");
	}
	char x;
	
	vector<char> chars;
	while (infile.get(x)) {
		chars.push_back( x );
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

void write_license( string key ) {
	boost::filesystem::path config = util::get_home() / ".errorx";
	write_to_file( config.string(), key );
}

bool valid_license() {
	
	boost::filesystem::path config = util::get_home() / ".errorx";

	string key;
	try {
		key = decrypt_from_file( config.string() );
	} catch ( invalid_argument & ) {
		// license file does not exist - run in trial version
		return 0;
	}

	vector<string> valid_keys = {
		"MattRyan",
		"DevontaFreeman",
		"TevinColeman",
		"JulioJones",
		"DeionJones",
		"VicBeasley",
		"KeanuNeal",
		"GradyJarrett",
		"CalvinRidley",
		"MattBryant"
	};

	// if key is found in list of valid keys - return 1 for
	// valid license
	return find( valid_keys.begin(), valid_keys.end(), key ) 
		 != valid_keys.end();

}
///////////// END encryption modules /////////////


} // namespace util
} // namespace errorx
