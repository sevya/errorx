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
		" -num_alignments_J 1 -outfmt 19"+
		" -out "+options.igblast_output()+
		" -num_threads "+to_string(options.nthreads());

	if ( options.verbose() > 1 ) {
		cout << command << endl;
	}

	// If the IGBlast output file already exists, delete it
	ifstream infile( options.igblast_output() );
	bool exists = infile.good();
	infile.close();
    if ( exists ) remove( options.igblast_output().c_str() );

    // IGBlast needs an environmental variable called IGDATA pointing 
    // to the path to database
	util::set_env( "IGDATA", root.string() );

	// TODO: FIGURE out a better way to capture output, since this doesn't work
	thread_finished_ = false;

	thread worker_thread = thread( &IGBlastParser::exec_in_thread, this, command );
	
	track_progress( options );
	
	worker_thread.join();
}

SequenceRecordsPtr IGBlastParser::parse_output( ErrorXOptions & options  ) {
	ios_base::sync_with_stdio( false );
	string line;
	ifstream file( options.igblast_output() );


	vector<string> lines;

	SequenceRecordsPtr records = SequenceRecordsPtr( new SequenceRecords( options ));

	if ( !file.good() ) {
		throw BadFileException( options.igblast_output()+" is not a valid file." );
		return records;
	}

	// Throw out the first header line
	getline( file, line );

	while ( getline (file, line) ) {
		vector<string> tokens = util::tokenize_string<string>( line, 
			"\t" /* delim */, 0 /* token_compress */, 0 /* trim */
			);


		AbSequence sequence = parse_line( tokens, options );
		SequenceRecordPtr record( new SequenceRecord( sequence ));

		records->add_record( record );	
	}

	return records;
}

void IGBlastParser::track_progress( ErrorXOptions & options ) {
	int done = 0;
	int last_done = 0;

	string infile = options.infile();
	string igblast_output = options.igblast_output();
	int total_records = options.num_queries();

	function<void(int,int,mutex*)> increment = options.increment();
	function<void(void)> reset = options.reset();
	function<void(void)> finish = options.finish();
	function<void(string)> message = options.message();
	message( "Running IGBlast..." );

	/// This mutex doesn't actually do anything - it's just 
	/// there for compatibility
	mutex* m = new mutex;

	increment( 0, total_records, m );

	while ( !thread_finished_ ) {
		done = util::count_lines( igblast_output );
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
	thread_finished_ = true;
}

AbSequence IGBlastParser::parse_line( vector<string> const & tokens, ErrorXOptions const & options ) {

	AbSequence sequence;

	if ( tokens.size() != 88 ) {
		sequence.good_ = false;
		sequence.failure_reason_ = "Output line does not parse correctly";
		return sequence;
	}

	// if a query is reversed, the sequence ID gets turned from SRR838 to 
	// reversed|SRR838. Here I just take out that reversed portion to get
	// the real ID
	vector<string> id_tokens = util::tokenize_string<string>( tokens[0], "|" );
	sequence.sequenceID_ = id_tokens[id_tokens.size()-1];

	// if the sequence is so bad that it looks nothing like an Ig domain,
	// igblast goes crazy and dosn't even put in a sequence ID
	if ( sequence.sequenceID_ == "" ) {
		sequence.good_ = 0;
		sequence.failure_reason_ = "No sequence ID found in the output - very malformed sequence";

		return sequence;
	}


	// Get the PHRED string that we previously stored in an unordered_map
	// if it's not present mark the sequence as bad and move on
	try {
		unordered_map<string,string> qmap = options.quality_map();
		sequence.phred_ = options.get_quality( sequence.sequenceID_ );
	} catch ( out_of_range & ) {
		sequence.good_ = 0;
		cout << "Warning: quality not found for sequence " << sequence.sequenceID_ << endl;
		sequence.failure_reason_ = "Quality information was not found";

		return sequence;
	}

	sequence.chain_      = tokens[2];
	sequence.productive_ = tokens[5]=="T";
	sequence.strand_     = ( tokens[6]=="F" ) ? "+" : "-";

	// bad chain ID - warn and keep going
	vector<string> valid_chains = { "VH","VL","VA","VB","VK" };
	if ( find( valid_chains.begin(), valid_chains.end(), sequence.chain() )
			== valid_chains.end() ) {
		// TODO: implement TCRG and D
		cout << "Warning: invalid chain type "+sequence.chain_+" detected" << endl;
	}

	sequence.cdr1_nt_sequence_ = tokens[34];
	if ( sequence.cdr1_nt_sequence_ == "" ) {
		sequence.cdr1_nt_sequence_ = "N/A";
		sequence.cdr1_aa_sequence_ = "N/A";
	} else {
		sequence.cdr1_aa_sequence_ = tokens[35];
	}
	
	sequence.cdr2_nt_sequence_ = tokens[38];
	if ( sequence.cdr2_nt_sequence_ == "" ) {
		sequence.cdr2_nt_sequence_ = "N/A";
		sequence.cdr2_aa_sequence_ = "N/A";
	} else {
		sequence.cdr2_aa_sequence_ = tokens[39];
	}

	sequence.cdr3_nt_sequence_ = tokens[42];
	if ( sequence.cdr3_nt_sequence_ == "" ) {
		sequence.cdr3_nt_sequence_ = "N/A";
		sequence.cdr3_aa_sequence_ = "N/A";
	} else {
		sequence.cdr3_aa_sequence_ = tokens[43];
	}



	// Get the V,D,J gene information
	sequence.hasV_ = 0;
	sequence.hasD_ = 0;
	sequence.hasJ_ = 0;

	try {
		sequence.v_gene_ = tokens[7];
		if ( sequence.v_gene_!="" ) {
			sequence.v_identity_  = boost::lexical_cast<double>( tokens[57] );
			sequence.v_evalue_    = boost::lexical_cast<double>( tokens[54] );
			sequence.v_nts_       = tokens[20];
			sequence.v_gl_nts_    = tokens[22];

			sequence.query_start_ = boost::lexical_cast<int>( tokens[60] );
			sequence.gl_start_    = boost::lexical_cast<int>( tokens[62] );

			// My logic for GL start is 1-indexed, not 0-indexed
			// the data given is 0-indexed so I'll increment by 1
			sequence.gl_start_++;

			sequence.hasV_ = ( sequence.v_evalue_ < constants::V_EVALUE_CUTOFF );
		} else {
			sequence.v_gene_ = "N/A";
		}


		sequence.d_gene_ = tokens[8];
		if ( sequence.d_gene_!="" ) {
			sequence.d_identity_ = boost::lexical_cast<double>( tokens[58] );
			sequence.d_evalue_   = boost::lexical_cast<double>( tokens[55] );
			sequence.d_nts_      = tokens[24];
			sequence.d_gl_nts_   = tokens[26];
			
			sequence.hasD_ = ( sequence.d_evalue_ < constants::D_EVALUE_CUTOFF );
		} else {
			sequence.d_gene_ = "N/A";
		}

		sequence.j_gene_ = tokens[9];
		if ( sequence.j_gene_!="" ) {
			sequence.j_identity_ = boost::lexical_cast<double>( tokens[59] );
			sequence.j_evalue_   = boost::lexical_cast<double>( tokens[56] );
			sequence.j_nts_      = tokens[28];
			sequence.j_gl_nts_   = tokens[30];

			sequence.hasJ_ = ( sequence.j_evalue_ < constants::J_EVALUE_CUTOFF );
		} else {
			sequence.j_gene_ = "N/A";
		}


		// Get junction information if there is a junction visible (hasD or hasJ)
		// This is the desired scheme:
		// no D and no J -> no junction
		// no D and J -> { V_end to J_start }
		// D and no J -> { V_end to D_start, D_start to D_end, D_end to sequence end }
		// D and J -> { V_end to D_start, D_start to D_end, D_end to J_start, }

		string aligned_seq = tokens[10];
		if ( !sequence.hasD_ && !sequence.hasJ_ ) {
			sequence.jxn_nts_ = vector<string>{ "" };
		} else if ( !sequence.hasD_ && sequence.hasJ_ ) {
			int v_end = boost::lexical_cast<int>( tokens[15] );
			int j_start = boost::lexical_cast<int>( tokens[18] );
			sequence.jxn_nts_ = vector<string>{ 
				aligned_seq.substr( v_end, j_start-v_end ), // from V to J
			};
		} else if ( sequence.hasD_ && !sequence.hasJ_ ) {
			int v_end = boost::lexical_cast<int>( tokens[15] );
			int d_start = boost::lexical_cast<int>( tokens[16] );
			int d_end = boost::lexical_cast<int>( tokens[17] );
			int seq_end = aligned_seq.size();
			sequence.jxn_nts_ = vector<string> {
				aligned_seq.substr( v_end, d_start-v_end ), // from V to D
				aligned_seq.substr( d_start, d_end-d_start ), // D region
				aligned_seq.substr( d_end, seq_end-d_end )  // from D to end
			}; 
		} else { // hasD_ && hasJ_
			int v_end = boost::lexical_cast<int>( tokens[15] );
			int d_start = boost::lexical_cast<int>( tokens[16] );
			int d_end = boost::lexical_cast<int>( tokens[17] );
			int j_start = boost::lexical_cast<int>( tokens[18] );
			sequence.jxn_nts_ = vector<string>{ 
				aligned_seq.substr( v_end, d_start-v_end ), // from V to D
				aligned_seq.substr( d_start, d_end-d_start ), // D region
				aligned_seq.substr( d_end, j_start-d_end )  // from D to J
			};
		}

	} catch ( out_of_range & ) {
		return sequence;
	} catch ( boost::bad_lexical_cast & ) {
		return sequence;
	}



	// Now I have all the information I need to build a sequence
	sequence.build( options );

	return sequence;

}


} // namespace errorx


