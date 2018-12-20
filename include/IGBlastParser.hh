/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file IGBlastParser.hh
@brief Class to run IGBlast on a set of query sequences and parse the output
@details IGBlastParser runs the executable, then breaks the output
file into chunks where each chunk is a single query. SequenceRecord does the
heavy lifting in terms of turning that output into a SequenceRecord object 
@author Alex Sevy (alex.sevy@gmail.com)
*/

#ifndef IGBLASTPARSER_HH_
#define IGBLASTPARSER_HH_

#include <iostream>

#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "util.hh"

using namespace std;

namespace errorx {
	
class IGBlastParser {

public:
	/**
		Empty constructor
	*/
	IGBlastParser();

	/**
		Runs IGBlast on a set of input sequences based on the 
		information in ErrorXOptions

		@param options ErrorXOptions that dictate what the input
		and output files are 
	*/
	void blast( ErrorXOptions & options );
	
	/**
		Splits the IGBlast output file into chunks so that it
		can be processed and turned into SequenceRecord(s)

		@param options ErrorXOptions to control processing

		@return A SequenceRecords object constructed from the IGBlast output
	*/
	SequenceRecords* parse_output( ErrorXOptions & options );

private:
	/**
		Tracks progress of the IGBlast query to make a progress bar

		@param options ErrorXOptions to control processing
	*/
	void track_progress( ErrorXOptions & options );

	/**
		Execute the query in another thread to allow progress bar

		@param command command to outsource to another thread
	*/
	void exec_in_thread( string command ) ;

	atomic<bool> thread_finished_;
	string thread_output_;
};

} // namespace errorx

#endif /* IGBLASTPARSER_HH_ */
