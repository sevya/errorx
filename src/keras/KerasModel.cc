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

	// TODO can I simplify this to one statement?
	string model_string = errorx::util::read_from_file( model_path );
	load_weights_from_string( model_string );
}


KerasModel::KerasModel( string const & file ) :
	verbose_( 1 )
{
	// TODO can I simplify this to one statement?
	string model_string = errorx::util::read_from_file( file );
	load_weights_from_string( model_string );
}

KerasModel::~KerasModel() {
	for ( uint i = 0; i < layers_.size(); ++i ) {
		delete layers_[ i ];
	}
}

vector<double> KerasModel::compute_output( DataChunk* dc ) const {
	//cout << endl << "KerasModel compute output" << endl;
	//cout << "Input data size:" << endl;
	if ( verbose_ > 1 ) dc->show_name();

	keras::DataChunk *inp = dc;
	keras::DataChunk *out = 0;
	for ( uint l = 0; l < layers_.size(); ++l ) {
		//cout << "Processing layer " << m_layers[l]->get_name() << endl;
		out = layers_[l]->compute_output( inp );

		//cout << "Input" << endl;
		//inp->show_name();
		//cout << "Output" << endl;
		//out->show_name();
		if ( inp != dc ) delete inp;
		//delete inp;
		inp = 0L;
		inp = out;
	}

	vector<double> flat_out = out->get_1d();
	if ( verbose_ > 1 ) out->show_values();
	delete out;

	return flat_out;
}

uint KerasModel::get_input_rows() const { 
	// TODO make sure layers_ is initialized
	return layers_[ 0 ]->get_input_rows(); 
}

uint KerasModel::get_input_cols() const { 
	// TODO make sure layers_ is initialized
	return layers_[ 0 ]->get_input_cols(); 
}

int KerasModel::get_output_length() const {
	int i = layers_.size() - 1;
	while ((i > 0) && (layers_[i]->get_output_units() == 0)) --i;
	return layers_[i]->get_output_units();
}

void KerasModel::load_weights( string const & input_string ) {
	if ( verbose_ > 1 ) cout << "Reading model from " << input_string << endl;
	ifstream fin( input_string.c_str() );

	string layer_type = "";
	string tmp_str = "";
	int tmp_int = 0;

	fin >> tmp_str >> layers_cnt_;
	if ( verbose_ > 1 ) cout << "Layers " << layers_cnt_ << endl;

	for ( int layer = 0; layer < layers_cnt_; ++layer ) { // iterate over layers
		fin >> tmp_str >> tmp_int >> layer_type;
		if ( verbose_ > 1 ) cout << "Layer " << tmp_int << " " << layer_type << endl;

		Layer* l = create_layer( layer_type );
		// layer is NULL if it's a dropout layer or empty
		if ( l != NULL ) {
			l->load_weights( fin );
			layers_.push_back( l );
		}
	}

	fin.close();
}

void KerasModel::load_weights_from_string( string const & input_string ) {
	istringstream fin( input_string );

	string layer_type = "";
	string tmp_str = "";
	int tmp_int = 0;

	fin >> tmp_str >> layers_cnt_;
	if ( verbose_ > 1 ) cout << "Layers " << layers_cnt_ << endl;

	for ( int layer = 0; layer < layers_cnt_; ++layer ) { // iterate over layers
		fin >> tmp_str >> tmp_int >> layer_type;
		if ( verbose_ > 1 ) cout << "Layer " << tmp_int << " " << layer_type << endl;

		Layer* l = create_layer( layer_type );
		// layer is null if it's a dropout layer or empty
		if ( l != nullptr ) {
			l->load_weights( fin );
			layers_.push_back( l );
		}
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
		// TODO throw an exception instead of cout
		cout << "Error: Layer type " << layer_type << " not supported. Exiting gracefully..." << endl;
		exit(0);
	}


	if ( l == nullptr ) {
		cout << "Layer is empty, maybe it is not defined? Cannot define network." << endl;
	}
		
	return l;
}

Layer* KerasModel::layer( int index ) const { return layers_[ index ]; }
int KerasModel::no_layers() const { return layers_.size(); }



} // namespace keras
