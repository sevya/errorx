/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file IGBlastParser.cc
@brief Class to run IGBlast on a set of query sequences and parse the output
@details IGBlastParser runs the executable, then breaks the output
file into chunks where each chunk is a single query. SequenceRecord does the
heavy lifting in terms of turning that output into a SequenceRecord object 
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

#include "IGBlastParser.hh"
#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "util.hh"
#include "constants.hh"

#include "AbSequence.hh"

#include <boost/filesystem.hpp>

using namespace std;

namespace errorx {

IGBlastParser::IGBlastParser() :
	thread_finished_(false)
{}

void IGBlastParser::blast( ErrorXOptions & options ) {

	namespace fs = boost::filesystem;

	fs::path root = options.errorx_base();
	string os = util::get_os();
	if ( os == "win" ) os += ".exe";
	string exename = "igblastn_"+util::get_os();
	fs::path executable = root / "bin" / exename;

	string species = options.species();
	string igtype = options.igtype();

	fs::path germline_db_V = root / "database" / igtype / species / (species+"_gl_V");
	fs::path germline_db_D = root / "database" / igtype / species / (species+"_gl_D");
	fs::path germline_db_J = root / "database" / igtype / species / (species+"_gl_J");

	fs::path aux_data = root / "optional_file" / (species+"_gl.aux");
	options.igblast_output( options.infasta()+".out" );

	string command = executable.string() +
		" -germline_db_V "+germline_db_V.string()+
		" -germline_db_D "+germline_db_D.string()+
		" -germline_db_J "+germline_db_J.string()+
		" -query "+options.infasta()+
		" -auxiliary_data "+aux_data.string() +
		" -num_alignments_V 1 -num_alignments_D 1"+
		" -num_clonotype 0"+
		" -ig_seqtype "+options.igtype()+
		" -num_alignments_J 1 -outfmt \"7 std qframe sframe qseq sseq\""+
		" -out "+options.igblast_output()+" -num_threads "+to_string(options.nthreads());

	if ( options.verbose() > 1 ) {
		cout << command << endl;
	}

	// If the IGBlast output file already exists, delete it
	ifstream infile(options.igblast_output());
	bool exists = infile.good();
	infile.close();
    if ( exists ) remove( options.igblast_output().c_str() );

    // IGBlast needs an environmental variable called IGDATA pointing 
    // to the path to database
	util::set_env( "IGDATA", root.string() );

	// TODO: FIGURE out a better way to capture output, since this doesn't work
	thread_finished_ = false;

	thread worker_thread = thread( &IGBlastParser::exec_in_thread, this, command );
	

	if ( options.verbose() > 0 ) {
		cout << "Running IGBlast..." << endl;
		track_progress( options );
	}
	worker_thread.join();
}

SequenceRecords* IGBlastParser::parse_output( ErrorXOptions & options  )
{
	ios_base::sync_with_stdio( false );
	string line;
	ifstream file( options.igblast_output() );


	vector<string> lines;

	SequenceRecords* records = new SequenceRecords( options );

	if ( !file.good() ) {
		throw invalid_argument( options.igblast_output()+" is not a valid file." );
		return records;
	}

	while ( getline (file, line) ) {
		vector<string> tokens = util::tokenize_string<string>( line );

		// If this is the first line of a query, and it's not the first line 
		// in the file, make a SequenceRecord from the previous query
		if ( tokens.size()==2 && 
			 tokens[1] == "IGBLASTN" && 
			 !lines.empty() ) {
			
			AbSequence sequence = parse_lines( lines, options );
			SequenceRecord* record = new SequenceRecord( sequence );

			records->add_record( record );

			lines.clear();
		} 
		// If all the queries are done, and I'm at the end of the file, 
		// make a SequenceRecord and finish up
		else if ( tokens.size()==4 && 
				  tokens[0] == "Total" && 
				  tokens[1] == "queries" ) {
			
			AbSequence sequence = parse_lines( lines, options );

			SequenceRecord* record = new SequenceRecord( sequence );

			records->add_record( record );
			lines.clear();
			break;
		} 
		// Otherwise, just add the line to the lines vector and keep going
		else {
			lines.push_back( line );
		}
	}

	return records;
}

void IGBlastParser::track_progress( ErrorXOptions & options ) {
	int done = 0;
	int last_done = 0;
	// float progress = 0.0;
	// float last_progress = -1;

	string infile = options.infile();
	string igblast_output = options.igblast_output();
	int total_records = util::count_lines( infile )/4;

	function<void(int,int,mutex*)> increment = options.increment();
	function<void(void)> reset = options.reset();
	function<void(void)> finish = options.finish();

	/// This mutex doesn't actually do anything - it's just 
	/// there for compatibility
	mutex* m = new mutex;

	increment( 0, total_records, m );

	while (!thread_finished_) {
		done = util::count_queries( igblast_output );
		// only write to screen if the value has changed
		if ( last_done != done ) {
			// increment with the amount that it's changed
			increment( done-last_done, total_records, m );

			last_done = done;
		}
		this_thread::sleep_for( chrono::milliseconds(500) );
	}
	// Finish the progress bar, since it's done now
	finish();
	reset();

	// We're done with mutex
	delete m;

	cout << endl;
}

void IGBlastParser::exec_in_thread( string command ) {
	// TODO: come up with a more robust way to capture the output of this command
	system( command.c_str() );
//	thread_output_ = util::exec( command.c_str() );
	thread_finished_ = true;
}


AbSequence IGBlastParser::parse_lines( vector<string> const & lines, ErrorXOptions const & options ) {

	// data map that holds different IGBlast output lines
	map<string,vector<string>> data_map;

	AbSequence sequence;

	// Build record from the contents of the IGBlast output
	for ( int ii = 0; ii < lines.size(); ++ii ) {
		vector<string> tokens = util::tokenize_string<string>( lines[ii] );
		
		// if the line doesn't have enough tokens to parse, move on
		if ( tokens.size() < 2 ) continue;

		// Get query line with the name of sequence
		if ( tokens[1] == "Query:" ) {
			data_map.insert( make_pair("query_string", tokens) );
		}
		// Get VDJ rearrangement line
		else if ( tokens[1] == "V-(D)-J" && tokens[2] == "rearrangement" ) {
			data_map.insert( make_pair("rearrangement_string", util::tokenize_string<string>(lines[ii+1])) );
		} 
		// Get VDJ junction line
		else if ( tokens[1] == "V-(D)-J" && tokens[2] == "junction" ) {
			data_map.insert( make_pair("junction_string", util::tokenize_string<string>(lines[ii+1])) );

		} 
		// Get subregion line with info on CDR3
		else if ( tokens[1] == "Sub-region" ) {
			data_map.insert( make_pair("subregion_string", util::tokenize_string<string>(lines[ii+1])) );
		} 
		// Get V/D/J line 
		else if ( tokens[0] == "V" || tokens[0] == "D" || tokens[0] == "J" ) {
			data_map.insert( make_pair(tokens[0]+"region_string", tokens ));
		}
	}

	// I need all three of these lines to have a successful record.
	// if not mark it as bad and return
	if ( data_map.find("query_string")         == data_map.end() ||
		 data_map.find("rearrangement_string") == data_map.end() ||
		 data_map.find("junction_string")      == data_map.end() ) 
	{
		sequence.good_ = 0;
		sequence.failure_reason_ = "One of the following fields is missing from the IGBlast output: query_string, rearrangement_string, or junction_string";
		return sequence;
	}


	// Now it's time to parse all of these lines
	vector<string> data;
	// Step 1. Get the sequence ID and the quality string
	data = data_map["query_string"];
	string ID_plus_qualstring = data[2];
	vector<string> id_tokens = util::tokenize_string<string>( ID_plus_qualstring, "|" );
	
	if ( id_tokens.size() > 2 ) {
		throw invalid_argument( 
			"Error: misformed sequence ID. You can't "
			"have the character \"|\" in your sequence ID." 
			);
	}
	
	sequence.sequenceID_ = id_tokens[0];
	sequence.phred_      = id_tokens[1];



	// Step 2. Get rearrangement info
	// decide if this is a VH/VB or VL/VA
	data = data_map["rearrangement_string"];
	bool vh = (data.size() == 8);

	int chain_idx      = vh ? 3 : 2;
	int productive_idx = vh ? 4 : 3;
	int strand_idx     = vh ? 7 : 6;

	sequence.chain_      = data[chain_idx];
	sequence.productive_ = data[productive_idx]=="No";
	sequence.strand_     = data[strand_idx];

	// bad chain ID - warn and keep going
	vector<string> valid_chains = {"VH","VL","VA","VB","VK"};
	if ( find( valid_chains.begin(), valid_chains.end(), sequence.chain() )
			== valid_chains.end() ) {
		// TODO: implement TCRG and D
		cout << "Warning: invalid chain type "+sequence.chain_+" detected" << endl;
	}


	// Step 3. If the subregion line is there, get the CDR3 information
	if ( data_map.find("subregion_string") != data_map.end() ) {
		data = data_map["subregion_string"];
		sequence.cdr3_nt_sequence_ = data[1];
		sequence.cdr3_aa_sequence_ = data[2];
	}

	// Step 4. Get the junction information
	data = data_map["junction_string"];

	if ( sequence.chain_ == "VH" || sequence.chain_ == "VB" ) {
		sequence.jxn_nts_ = vector<string>{ data[1], data[2], data[3] };
	} else {
		sequence.jxn_nts_ = vector<string>{ data[1] };
	}

	// Step 5. Get the V gene information
	sequence.hasV_ = 0;
	if ( data_map.find("Vregion_string") != data_map.end() ) {
		data = data_map["Vregion_string"];
		try {
			sequence.v_gene_      = data[2];
			sequence.v_identity_  = boost::lexical_cast<double>( data[3] );
			sequence.v_evalue_    = boost::lexical_cast<double>( data[12] );
			sequence.v_nts_       = data[16];
			sequence.v_gl_nts_    = data[17];

			sequence.query_start_ = boost::lexical_cast<int>( data[8] );
			sequence.gl_start_    = boost::lexical_cast<int>( data[10] );

			sequence.hasV_ = ( sequence.v_evalue_ < constants::V_EVALUE_CUTOFF );

		} catch ( out_of_range & e ) {
			return sequence;
		} catch ( boost::bad_lexical_cast & e ) {
			return sequence;
		}
	}


	// Step 6. Get the D gene information
	sequence.hasD_ = 0;
	if ( data_map.find("Dregion_string") != data_map.end() ) {
		data = data_map["Dregion_string"];
		try {
			sequence.d_gene_     = data[2];
			sequence.d_identity_ = boost::lexical_cast<double>( data[3] );
			sequence.d_evalue_   = boost::lexical_cast<double>( data[12] );
			sequence.d_nts_      = data[16];
			sequence.d_gl_nts_   = data[17];
			
			sequence.hasD_ = ( sequence.d_evalue_ < constants::D_EVALUE_CUTOFF );

		} catch ( out_of_range & e ) {
			return sequence;
		} catch ( boost::bad_lexical_cast & e ) {
			return sequence;
		}
	}



	// Step 7. Get the J gene information
	sequence.hasJ_ = 0;
	if ( data_map.find("Jregion_string") != data_map.end() ) {
		data = data_map["Jregion_string"];
		try {
			sequence.j_gene_     = data[2];
			sequence.j_identity_ = boost::lexical_cast<double>( data[3] );
			sequence.j_evalue_   = boost::lexical_cast<double>( data[12] );
			sequence.j_nts_      = data[16];
			sequence.j_gl_nts_   = data[17];	

			sequence.hasJ_ = ( sequence.j_evalue_ < constants::J_EVALUE_CUTOFF );

		} catch ( out_of_range & e ) {
			return sequence;
		} catch ( boost::bad_lexical_cast & e ) {
			return sequence;
		}
	}

	
	// Step 8. Now I have all the information I need to build a sequence
	sequence.build( options );

	return sequence;
}

} // namespace errorx


