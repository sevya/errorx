/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file keras/KerasModel.cc
@brief Class representing a Keras model for a neural network
@author Alex Sevy (alex@endeavorbio.com)
*/

#include "keras/KerasModel.hh"
#include "keras/DataChunk.hh"
#include "keras/Layer.hh"
#include "keras/LayerActivation.hh"
#include "keras/LayerDense.hh"

#include "ErrorXOptions.hh"
#include "exceptions.hh"
#include "util.hh"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

using namespace std;

namespace keras {

KerasModel::KerasModel( errorx::ErrorXOptions const & options ) :
	verbose_( options.verbose() )
{
	namespace fs = boost::filesystem;
	fs::path base( options.errorx_base() );
	string model_path = (base / "model.nnet").string();

	load_weights( model_path );
}


KerasModel::KerasModel( string const & file ) :
	verbose_( 1 )
{
	load_weights( file );
}

KerasModel::KerasModel() :
	verbose_( 1 )
{}

KerasModel::KerasModel( KerasModel const & other ) :
	layers_cnt_( other.layers_cnt_ ),
	layers_( other.layers_ ),
	verbose_( other.verbose_ )
{}

KerasModel::~KerasModel() {
	for ( uint i = 0; i < layers_.size(); ++i ) {
		delete layers_[ i ];
	}
}

vector<double> KerasModel::compute_output( DataChunk* dc ) const {

	if ( layers_.empty() ) {
		throw ObjectNotInitialized( 
			"Error: compute_output was called for a KerasModel "
			"that was never initialized. Please initialize object "
			"using load_weights or load_weights_from_string before "
			"computing output.");
	}

	if ( get_input_cols() != dc->num_features() ) {
		throw BadModel(
			"Error: input dimensions do not match. Model takes "
			"data with "+to_string(get_input_cols())+" dimensions "
			"and data chunk has "+to_string(dc->num_features())
			);
	}

	if ( verbose_ > 1 ) dc->show_name();

	DataChunk* inp = dc;
	DataChunk* out = 0;
	for ( uint l = 0; l < layers_.size(); ++l ) {
		out = layers_[l]->compute_output( inp );

		if ( inp != dc ) delete inp;
		//delete inp;
		inp = 0L;
		inp = out;
	}

	vector<double> flat_out = out->get_1d();
	delete out;

	return flat_out;
}

uint KerasModel::get_input_rows() const { 
	if ( layers_.empty() ) {
		throw ObjectNotInitialized( 
			"Error: compute_output was called for a KerasModel "
			"that was never initialized. Please initialize object "
			"using load_weights or load_weights_from_string before "
			"computing output.");
	}
	return layers_[ 0 ]->get_input_rows(); 
}

uint KerasModel::get_input_cols() const { 
	if ( layers_.empty() ) {
		throw ObjectNotInitialized( 
			"Error: compute_output was called for a KerasModel "
			"that was never initialized. Please initialize object "
			"using load_weights or load_weights_from_string before "
			"computing output.");
	}
	return layers_[ 0 ]->get_input_cols(); 
}

int KerasModel::get_output_length() const {
	if ( layers_.empty() ) {
		throw ObjectNotInitialized( 
			"Error: compute_output was called for a KerasModel "
			"that was never initialized. Please initialize object "
			"using load_weights or load_weights_from_string before "
			"computing output.");
	}
	int i = layers_.size() - 1;
	while ((i >= 0) && (layers_[i]->get_output_units() == 0)) --i;
	return layers_[i]->get_output_units();
}

void KerasModel::load_weights( string const & infile ) {

	if ( verbose_ > 1 ) cout << "Reading model from " << infile << endl;
	ifstream fin( infile.c_str() );
	
	if ( !fin.good()) {
		throw invalid_argument("Error: file "+infile+" does not exist.");
	}

	load_weights_from_stream( fin );

	fin.close();
}

void KerasModel::load_weights_from_string( string const & input_string ) {
	istringstream fin( input_string );
	load_weights_from_stream( fin );
}

void KerasModel::load_weights_from_stream( istream & fin ) {
	// Clear all data if previously set
	layers_cnt_ = 0;
	for ( int ii = 0; ii < layers_.size(); ++ii ) delete layers_[ ii ];
	layers_.clear();


	string layer_type = "";
	string tmp_str = "";
	int tmp_int = 0;

	fin >> tmp_str >> layers_cnt_;
	if ( verbose_ > 1 ) cout << "Layers " << layers_cnt_ << endl;

	// iterate over layers
	for ( int layer = 0; layer < layers_cnt_; ++layer ) { 
		fin >> tmp_str >> tmp_int >> layer_type;
		
		if ( tmp_str != "layer" ) {
			throw BadModel(
			"Error: number of layers specified at the top of model file "
			"does not match number defined.\n"
			"Make sure you define exactly "+to_string(layers_cnt_)+" layers."
			);
		}

		if ( verbose_ > 1 ) cout << "Layer " << tmp_int << " " << layer_type << endl;

		Layer* l = create_layer( layer_type );
		// layer is null if it's a dropout layer or empty
		if ( l != nullptr ) {
			l->load_weights( fin );
			layers_.push_back( l );
		}

		// Reset this value so I can tell if line is malformed
		tmp_str = "";
	}

	// Check if there are more layers in the file - if so the 
	// number at the top is probably wrong
	fin >> tmp_str;
	if ( tmp_str == "layer" ) {
		throw BadModel(
		"Error: there are "+to_string(layers_cnt_)+" defined at the top of your model "
		"but more are defined. Please check and make sure these match."
		);
	}
}

Layer* KerasModel::create_layer( string const & layer_type ) {
	/**
		Currently only Activation and Dense layers are supported 
		(and Dropout by ignoring it)
		Should be updated in the future
	*/
	Layer* l = nullptr;

	if ( layer_type == "Activation" ) {
		l = new LayerActivation();
	} else if ( layer_type == "Dense" ) {
		l = new LayerDense();
	} else if ( layer_type == "Dropout" ) {
		// we don't need dropout layer in prediction mode
		// leave it as null
	} else {
		throw InvalidLayer( layer_type );
	}


	if ( l == nullptr ) {
		cout << "Layer is empty, maybe it is not defined? Cannot define network." << endl;
	}
		
	return l;
}

Layer* KerasModel::layer( int index ) const { return layers_[ index ]; }
int KerasModel::no_layers() const { return layers_.size(); }

int KerasModel::verbose() const { return verbose_; }
void KerasModel::verbose( int verbose ) { verbose_ = verbose; }

} // namespace keras
