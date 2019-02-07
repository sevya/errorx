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

#include "IGBlastParser.hh"
#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "util.hh"

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
	ios_base::sync_with_stdio (false);
	string line;
	ifstream file( options.igblast_output() );


	vector<string> lines;

	SequenceRecords* records = new SequenceRecords( options );

	if ( !file.good() ) {
		throw invalid_argument( options.igblast_output()+" is not a valid file." );
		return records;
	}

	while (getline (file, line)) {
		vector<string> tokens = util::tokenize_string<string>( line );

		// If this is the first line of a query, and it's not the first line 
		// in the file, make a SequenceRecord from the previous query
		if ( tokens.size()==2 && 
			 tokens[1] == "IGBLASTN" && 
			 !lines.empty() ) {
			
			SequenceRecord* record = new SequenceRecord( 
					lines, 
					options.verbose(), 
					options.allow_nonproductive()
					);
			records->add_record( record );

			lines.clear();
		} 
		// If all the queries are done, and I'm at the end of the file, 
		// make a SequenceRecord and finish up
		else if ( tokens.size()==4 && 
				  tokens[0] == "Total" && 
				  tokens[1] == "queries" ) {

			SequenceRecord* record = new SequenceRecord( 
					lines, 
					options.verbose(), 
					options.allow_nonproductive()
					);
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
		float progress = 0.0;
		float last_progress = -1;

		string infile = options.infile();
		string igblast_output = options.igblast_output();
		int total_records = util::count_lines( infile )/4;

		while (!thread_finished_) {
			done = util::count_queries( igblast_output );
			progress = (float)done/(float)total_records;

			// only write to screen if the value has changed
			if ( progress != last_progress ) { 
				util::write_progress_bar( progress, done, total_records );
			}
			last_progress = progress;
		}
		cout << endl;
}

void IGBlastParser::exec_in_thread( string command ) {
	// TODO: come up with a more robust way to capture the output of this command
	system( command.c_str() );
//	thread_output_ = util::exec( command.c_str() );
	thread_finished_ = true;
}

} // namespace errorx


