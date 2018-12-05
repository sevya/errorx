/**
@file SequenceQuery.cc
@brief A nucleotide sequence along with the minimal 
information needed for error correction
@details A single nucleotide sequence along with the inferred
germline sequence, sequence ID, and PHRED (quality) score as 
a string of characters
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include "SequenceQuery.hh"

using namespace std;

namespace errorx {

SequenceQuery::SequenceQuery( string sequenceID, 
				   string sequence, 
				   string germline_sequence, 
				   string phred_string ) :
		sequenceID_( sequenceID ),
		sequence_( sequence ),
		germline_sequence_( germline_sequence ),
		phred_string_( phred_string ) 
{
	if ( sequence_.size() != germline_sequence_.size() or 
		 sequence_.size() != phred_string_.size() ) {
		throw invalid_argument( "Error: invalid sequences given for correcting. "
			"Sequence, germline sequence and PHRED string must have same length. "
			"In sequence "+sequenceID_+" lengths are: "+
			to_string(sequence_.size())+", "+
			to_string(germline_sequence_.size())+", "+
			to_string(phred_string_.size()) );
	}
}

SequenceQuery::SequenceQuery( SequenceQuery const & other ) :
		sequenceID_( other.sequenceID_ ),
		sequence_( other.sequence_ ),
		germline_sequence_( other.germline_sequence_ ),
		phred_string_( other.phred_string_ ) {}

void SequenceQuery::sequenceID( string const sequenceID ) { sequenceID_ = sequenceID; }
void SequenceQuery::sequence( string const sequence ) { sequence_ = sequence; }
void SequenceQuery::germline_sequence( string const germline_sequence ) { germline_sequence_ = germline_sequence; }
void SequenceQuery::phred_string( string const phred_string ) { phred_string_ = phred_string; }

string SequenceQuery::sequenceID() const { return sequenceID_; }
string SequenceQuery::sequence() const { return sequence_; }
string SequenceQuery::germline_sequence() const { return germline_sequence_; }
string SequenceQuery::phred_string() const { return phred_string_; }

} // namespace errorx
