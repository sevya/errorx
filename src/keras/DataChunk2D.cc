/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/DataChunk2D.cc
@brief 2-D data chunk to process through Keras model
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "keras/DataChunk.hh"
#include "keras/DataChunk2D.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace keras {

DataChunk2D::DataChunk2D() {}

vector<vector<vector<double>>> const & DataChunk2D::get_3d() const { return data_; };

void DataChunk2D::set_data( vector<vector<vector<double>>> const & data ) { data_ = data; };

int DataChunk2D::get_data_dim() const { return 3; }

void DataChunk2D::show_name() const {
	cout << "DataChunk2D " << data_.size() << "x" << data_[0].size() << "x" << data_[0][0].size() << endl;
}

void DataChunk2D::show_values() const {
	cout << "DataChunk2D values:" << endl;
	for ( int i = 0; i < data_.size(); ++i ) {
		cout << "Kernel " << i << endl;
		for ( int j = 0; j < data_[0].size(); ++j ) {
			for ( int k = 0; k < data_[0][0].size(); ++k ) {
				cout << data_[i][j][k] << " ";
			}
			cout << endl;
		}
	}
}

void DataChunk2D::read_from_file( string const & fname ) {
	ifstream fin( fname.c_str() );
	fin >> depth_ >> rows_ >> cols_;

	for ( int d = 0; d < depth_; ++d ) {
		vector<vector<double> > tmp_single_depth;
		for ( int r = 0; r < rows_; ++r ) {
			vector<double> tmp_row = read_1d_array( fin, cols_ );
			tmp_single_depth.push_back(tmp_row);
		}
		data_.push_back( tmp_single_depth );
	}
	fin.close();
}

vector<double> DataChunk2D::read_1d_array( ifstream & fin, int cols ) {
	vector<double> arr;
	double tmp_double;
	char tmp_char;
	fin >> tmp_char; // for '['
	for ( int n = 0; n < cols; ++n ) {
		fin >> tmp_double;
		arr.push_back(tmp_double);
	}
	fin >> tmp_char; // for ']'
	return arr;
}

} //namespace keras