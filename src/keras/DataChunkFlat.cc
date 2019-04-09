/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/DataChunkFlat.cc
@brief 1-D data chunk to process through Keras model
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "keras/DataChunk.hh"
#include "keras/DataChunkFlat.hh"

#include <vector>
#include <iostream>

using namespace std;

namespace keras {

DataChunkFlat::DataChunkFlat() {}

DataChunkFlat::DataChunkFlat( int size ) : 
	data_( size ) 
	{}

DataChunkFlat::DataChunkFlat( int size, double init ) : 
	data_( size, init ) 
	{}

vector<double> const & DataChunkFlat::get_1d() const { return data_; }
vector<double> & DataChunkFlat::get_1d_rw() { return data_; }

void DataChunkFlat::set_data( vector<double> const & data ) { data_ = data; };

int DataChunkFlat::get_data_dim() const { return 1; }

void DataChunkFlat::show_name() const {
	cout << "DataChunkFlat " << data_.size() << endl;
}

void DataChunkFlat::show_values() const {
	cout << "DataChunkFlat values:" << endl;
	for ( int i = 0; i < data_.size(); ++i ) cout << data_[ i ] << " ";
	cout << endl;
}

// TODO implement if necessary, remove if not
void DataChunkFlat::read_from_file( string const & fname ) {}

} //namespace keras