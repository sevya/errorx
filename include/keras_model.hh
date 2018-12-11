/**
@file keras_model.hh
@brief Runs data through a Keras neural network trained in Python
@author Alex Sevy (alex.sevy@gmail.com)
*/


#ifndef KERAS_MODEL__H
#define KERAS_MODEL__H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

namespace keras {
	vector<double> read_1d_array( ifstream &fin, int cols );

	void missing_activation_impl( const string &act );

	vector<vector<double>> conv_single_depth_valid( vector<vector<double>> const & im, vector< vector<double>> const & k );
	vector<vector<double>> conv_single_depth_same( vector<vector<double>> const & im, vector< vector<double>> const & k );

	class DataChunk;
	class DataChunk2D;
	class DataChunkFlat;

	class Layer;
	class LayerFlatten;
	class LayerMaxPooling;
	class LayerActivation;
	class LayerConv2D;
	class LayerDense;

	class KerasModel;

class DataChunk {
public:
	virtual ~DataChunk() {}
	virtual int get_data_dim(void) const { return 0; }
	virtual vector<double> const & get_1d() const { throw "not implemented"; };
	virtual vector<vector<vector<double> > > const & get_3d() const { throw "not implemented"; };
	virtual void set_data(vector<vector<vector<double> > > const &) {};
	virtual void set_data(vector<double> const &) {};
	virtual void read_from_file(const string &fname) {};
	virtual void show_name() = 0;
	virtual void show_values() = 0;
};

class DataChunk2D : public DataChunk {
public:
	vector< vector< vector<double> > > const & get_3d() const { return data; };
	virtual void set_data(vector<vector<vector<double> > > const & d) { data = d; };
	int get_data_dim(void) const { return 3; }

	void show_name() {
		cout << "DataChunk2D " << data.size() << "x" << data[0].size() << "x" << data[0][0].size() << endl;
	}

	void show_values() {
		cout << "DataChunk2D values:" << endl;
    	for ( int i = 0; i < data.size(); ++i ) {
			cout << "Kernel " << i << endl;
			for ( int j = 0; j < data[0].size(); ++j ) {
				for ( int k = 0; k < data[0][0].size(); ++k ) {
					cout << data[i][j][k] << " ";
				}
        		cout << endl;
			}
		}
	}

	void read_from_file(const string &fname);
	vector<vector<vector<double> > > data; // depth, rows, cols

	int m_depth;
	int m_rows;
	int m_cols;
};

class DataChunkFlat : public DataChunk {

public:
	DataChunkFlat(int size) : f(size) { }
	DataChunkFlat(int size, double init) : f(size, init) { }
	DataChunkFlat(void) { }

	vector<double> f;
	vector<double> & get_1d_rw() { return f; }
	vector<double> const & get_1d() const { return f; }
	void set_data(vector<double> const & d) { f = d; };
	int get_data_dim(void) const { return 1; }

	void show_name() {
		cout << "DataChunkFlat " << f.size() << endl;
	}
  
	void show_values() {
		cout << "DataChunkFlat values:" << endl;
		for ( int i = 0; i < f.size(); ++i ) cout << f[i] << " ";
		cout << endl;
	}
	void read_from_file(const string &fname) {};
};

class Layer {
public:
	virtual void load_weights( ifstream &fin ) = 0;
	virtual void load_weights( istringstream &fin ) = 0;
	virtual keras::DataChunk* compute_output( DataChunk* ) = 0;

	Layer(string name) : m_name(name) {}
	virtual ~Layer() {}

	virtual unsigned int get_input_rows() const = 0;
	virtual unsigned int get_input_cols() const = 0;
	virtual unsigned int get_output_units() const = 0;

	string get_name() { return m_name; }
	string m_name;
};


class LayerFlatten : public Layer {
public:
	LayerFlatten() : Layer("Flatten") {}
	void load_weights( ifstream &fin ) {};
	void load_weights( istringstream &fin ) {};
	keras::DataChunk* compute_output( DataChunk* );

	virtual unsigned int get_input_rows() const { return 0; } // look for the value in the preceding layer
	virtual unsigned int get_input_cols() const { return 0; } // same as for rows
	virtual unsigned int get_output_units() const { return 0; }
};


class LayerMaxPooling : public Layer {
public:
	LayerMaxPooling() : Layer("MaxPooling2D") {};

	void load_weights( ifstream &fin );
	void load_weights( istringstream &fin );
	DataChunk* compute_output( DataChunk* );

	virtual unsigned int get_input_rows() const { return 0; } // look for the value in the preceding layer
	virtual unsigned int get_input_cols() const { return 0; } // same as for rows
	virtual unsigned int get_output_units() const { return 0; }

	int m_pool_x;
	int m_pool_y;

};

class LayerActivation : public Layer {
public:
	LayerActivation() : Layer("Activation") {}
	void load_weights( ifstream &fin );
	void load_weights( istringstream &fin );
	DataChunk* compute_output( DataChunk* );

	virtual unsigned int get_input_rows() const { return 0; } // look for the value in the preceding layer
	virtual unsigned int get_input_cols() const { return 0; } // same as for rows
	virtual unsigned int get_output_units() const { return 0; }

	string m_activation_type;
};

class LayerConv2D : public Layer {
public:
	LayerConv2D() : Layer("Conv2D") {}

	void load_weights( ifstream &fin );
	void load_weights( istringstream &fin );
	DataChunk* compute_output( DataChunk* );
	vector<vector<vector<vector<double>>>> m_kernels; // kernel, depth, rows, cols
	vector<double> m_bias; // kernel

	virtual unsigned int get_input_rows() const { return m_rows; }
	virtual unsigned int get_input_cols() const { return m_cols; }
	virtual unsigned int get_output_units() const { return m_kernels_cnt; }

	string m_border_mode;
	int m_kernels_cnt;
	int m_depth;
	int m_rows;
	int m_cols;
};

class LayerDense : public Layer {
public:
	LayerDense() : Layer("Dense") {}

	void load_weights( ifstream &fin );
	void load_weights( istringstream &fin );
	DataChunk* compute_output( DataChunk* );
	vector<vector<double>> m_weights; //input, neuron
	vector<double> m_bias; // neuron

	virtual unsigned int get_input_rows() const { return 1; } // flat, just one row
	virtual unsigned int get_input_cols() const { return m_input_cnt; }
	virtual unsigned int get_output_units() const { return m_neurons; }

	void print_weights();

	int m_input_cnt;
	int m_neurons;
};

class KerasModel {
public:
	KerasModel( const string &input_fname, int verbose );
	KerasModel( const string &input_fname, bool from_string, int verbose);

	~KerasModel();
	vector<double> compute_output( DataChunk *dc ) const;

	unsigned int get_input_rows() const { return m_layers.front()->get_input_rows(); }
	unsigned int get_input_cols() const { return m_layers.front()->get_input_cols(); }
	int get_output_length() const;

private:
  void load_weights(const string &input_fname);
  void load_weights_from_string(const string &input_fname);

  int m_layers_cnt; // number of layers
  vector<Layer *> m_layers; // container with layers
  int m_verbose;

};

} // namespace keras
#endif
