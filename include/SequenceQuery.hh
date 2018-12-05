/**
@file SequenceQuery.hh
@brief A nucleotide sequence along with the minimal 
information needed for error correction
@details A single nucleotide sequence along with the inferred
germline sequence, sequence ID, and PHRED (quality) score as 
a string of characters
@author Alex Sevy (alex.sevy@gmail.com)
*/

#ifndef SEQUENCEQUERY_HH_
#define SEQUENCEQUERY_HH_

#include <string>

using namespace std;

namespace errorx {

class SequenceQuery {

public:

	/**
		Constructor with all the necessary information

		@param sequenceID Name of sequence
		@param sequence NT sequence
		@param germline_sequence Inferred germline sequence (on DNA level)
		@param phred_string PHRED (quality) scores straight from the sequencer 
	*/	
	SequenceQuery( string sequenceID, 
				   string sequence, 
				   string germline_sequence, 
				   string phred_string );
	/**
		Copy constructor
	*/	
	SequenceQuery( SequenceQuery const & other );
	
	/**
		Setters
	*/
	void sequenceID( string const sequenceID );
	void sequence( string const sequence );
	void germline_sequence( string const germline_sequence );
	void phred_string( string const phred_string );

	/**
		Getters
	*/
	string sequenceID() const;
	string sequence() const;
	string germline_sequence() const;
	string phred_string() const;

private:
	string sequenceID_;
	string sequence_;
	string germline_sequence_;
	string phred_string_;
};

} // namespace errorx

#endif /* SEQUENCEQUERY_HH_ */
