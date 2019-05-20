/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ClonotypeGroup.cc
@brief Collection of SequenceRecord objects that represent a single clonotype
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <vector>
#include <string>

#include "ClonotypeGroup.hh"
#include "SequenceRecord.hh"

using namespace std;

namespace errorx {

/**
	Destructor. Does not delete references to individual
	SequenceRecord pointers bc these are owned by the 
	SequenceRecords object that spawns the ClonotypeGroup
*/
ClonotypeGroup::~ClonotypeGroup() {}

ClonotypeGroup::ClonotypeGroup() {}

ClonotypeGroup::ClonotypeGroup( ClonotypeGroup const & other ) :
	vgene_( other.vgene_ ),
	jgene_( other.jgene_ ),
	cdr3_( other.cdr3_ ),
	records_( other.records_ ) 
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

/*
bool operator==( ClonotypeGroup const & a, ClonotypeGroup const & b ) {
	// compare v genes
	if ( a.v_gene() != b.v_gene() ) return 0;

	// compare j genes
	if ( a.j_gene() != b.j_gene() ) return 0;

	// compare CDR3s
	string cdrA = a.cdr3();
	string cdrB = b.cdr3();
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

bool operator!=( ClonotypeGroup const & a, ClonotypeGroup const & b ) {
	return !(a==b);
}
*/

void ClonotypeGroup::add_record( SequenceRecord* record ) {
	records_.push_back( record );
}

int ClonotypeGroup::size() const { return records_.size(); }


SequenceRecord* ClonotypeGroup::get( int i ) const {
	return records_[ i ];
}

/**
bool compare( ClonotypeGroup const & a, ClonotypeGroup const & b ) {
	// compare v genes
	if ( a.v_gene() != b.v_gene() ) return a.v_gene() < b.v_gene();

	// compare j genes
	if ( a.j_gene() != b.j_gene() ) return a.j_gene() < b.j_gene();

	// compare CDR3s
	string cdrA = a.cdr3();
	string cdrB = b.cdr3();
	if ( cdrA.size() != cdrB.size() ) return cdrA < cdrB;

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

	return a_noN < b_noN;
}
*/

string ClonotypeGroup::v_gene() const { return vgene_; }
string ClonotypeGroup::j_gene() const { return jgene_; }
string ClonotypeGroup::cdr3() const { return cdr3_; }
string ClonotypeGroup::toString() const { return vgene_+"_"+cdr3_+"_"+jgene_; }

void ClonotypeGroup::v_gene( string const & vgene ) { vgene_ = vgene; }
void ClonotypeGroup::j_gene( string const & jgene ) { jgene_ = jgene; }
void ClonotypeGroup::cdr3( string const & cdr3 ) { cdr3_ = cdr3; }

} // namespace errorx
