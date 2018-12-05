import numpy as np
np.random.seed(1337)
from keras.models import Sequential, model_from_json
import json
import argparse

import make_features as mf

np.set_printoptions(threshold=np.inf)
parser = argparse.ArgumentParser(description='This is a simple script to dump Keras model into simple format suitable for porting into pure C++ model')

parser.add_argument('-a', '--architecture', help="JSON with model architecture", required=True)
parser.add_argument('-w', '--weights', help="Model weights in HDF5 format", required=True)

args = parser.parse_args()

print 'Read architecture from', args.architecture
print 'Read weights from', args.weights

model_string = mf.get_model_as_string( args.architecture, args.weights )

cpp_base = '''
/** 
@file model.cc 
@brief Hardcoded model for use in error prediction
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <string>
#include <model.hh>

namespace errorx {
namespace model { 

std::string get_model() {
	return \"%s\";
}

} // namespace model
} // namespace errorx
'''%(model_string)

with open('model.cc','w') as out:
	out.write(cpp_base)

