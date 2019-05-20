/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ClonotypeGroup.cc
@brief Collection of SequenceRecord objects that represent a single clonotype
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <vector>
#include <string>
#include <functional>

#include "ClonotypeGroup.hh"
#include "SequenceRecord.hh"
#include "util.hh"
#include "ErrorXOptions.hh"

using namespace std;

namespace errorx {

/**
	Destructor. Does not delete references to individual
	SequenceRecord pointers bc these are owned by the 
	SequenceRecords object that spawns the ClonotypeGroup
*/
ClonotypeGroup::~ClonotypeGroup() {}

ClonotypeGroup::ClonotypeGroup( ErrorXOptions const & options ) :
	vgene_( "" ),
	jgene_( "" ),
	cdr3_( "" ),
	records_(),
	options_( options )
	{}

ClonotypeGroup::ClonotypeGroup( ClonotypeGroup const & other ) :
	vgene_( other.vgene_ ),
	jgene_( other.jgene_ ),
	cdr3_( other.cdr3_ ),
	records_( other.records_ ),
	options_( other.options_ )
	{}

bool ClonotypeGroup::operator==( ClonotypeGroup const & other ) {

	// compare v genes
	if ( v_gene() != other.v_gene() ) return 0;

	// compare j genes
	if ( j_gene() != other.j_gene() ) return 0;

	// compare CDR3s
	string cdrA = cdr3();
	string cdrB = other.cdr3();
	if ( cdrA.size() != cdrB.size() ) return 0;

	// Remove all the X amino acids from CDR3 then compare
	string a_noN = "";
	string b_noN = "";

	for ( int ii = cdrA.length()-1; ii >= 0; --ii ) {
		if ( cdrA[ii] != 'X' && cdrB[ii] != 'X' ) {
			a_noN += cdrA[ii];
			b_noN += cdrB[ii];
		}
	}

	reverse( a_noN.begin(), a_noN.end() );
	reverse( b_noN.begin(), b_noN.end() );

	return a_noN == b_noN;
}

bool ClonotypeGroup::operator!=( ClonotypeGroup const & other ) {
	return !((*this)==other);
}

int ClonotypeGroup::somatic_variants() const {

	map<string,int> cmap;
	map<string,int>::iterator it;

	string key;
	for ( int ii = 0; ii < records_.size(); ++ii ) {
		SequenceRecord* current_record = records_[ ii ];

		if ( !current_record->isGood() ) continue;

		key = current_record->full_nt_sequence();
		it = cmap.find( key );
		if ( it == cmap.end() ) {
			cmap.insert( pair<string,int>( key, 1 ));
		} else {
			it->second += 1;
		}
	}

	return cmap.size();
}

int ClonotypeGroup::corrected_somatic_variants() const {
	// Custom comparator for corrected sequences
	// treats sequences the same if they differ only by N nucleotides
	function< bool(string,string) > compareCorrectedSequences = 
		std::bind( &util::compare, 
				   placeholders::_1, 
				   placeholders::_2, 
				   options_.correction()
		);

	// instantiate map of corrected sequence with its comparator
	map<string,int,function<bool(string,string)>> cmap( compareCorrectedSequences );
	map<string,int>::iterator it;

	string key;
	for ( int ii = 0; ii < records_.size(); ++ii ) {
		SequenceRecord* current_record = records_[ ii ];

		if ( !current_record->isGood() ) continue;

		key = current_record->full_nt_sequence_corrected();
		it = cmap.find( key );
		if ( it == cmap.end() ) {
			cmap.insert( pair<string,int>( key, 1 ));
		} else {
			it->second += 1;
		}

	}

	return cmap.size();
}

void ClonotypeGroup::add_record( SequenceRecord* record ) {
	records_.push_back( record );
	
	// if the current CDR3 has an X and the new record doesn't
	// replace CDR3 so that it is a full sequence
	bool old_cdr3_has_X = find( cdr3_.begin(), cdr3_.end(), 'X' ) 
							!= cdr3_.end();

	string new_cdr3 = record->cdr3_aa_sequence();
	bool new_cdr3_has_X = find( new_cdr3.begin(), new_cdr3.end(), 'X' ) 
							!= new_cdr3.end();
	
	if ( old_cdr3_has_X and !new_cdr3_has_X ) cdr3_ = new_cdr3;
}

int ClonotypeGroup::size() const { return records_.size(); }


SequenceRecord* ClonotypeGroup::get( int i ) const {
	return records_[ i ];
}


string ClonotypeGroup::v_gene() const { return vgene_; }
string ClonotypeGroup::j_gene() const { return jgene_; }
string ClonotypeGroup::cdr3() const { return cdr3_; }
string ClonotypeGroup::toString() const { return vgene_+"_"+cdr3_+"_"+jgene_; }

void ClonotypeGroup::v_gene( string const & vgene ) { vgene_ = vgene; }
void ClonotypeGroup::j_gene( string const & jgene ) { jgene_ = jgene; }
void ClonotypeGroup::cdr3( string const & cdr3 ) { cdr3_ = cdr3; }

} // namespace errorx
