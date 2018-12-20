/** Copyright (C) Endeavor Bio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecords.cc
@brief Collection of SequenceRecord objects that holds all
data associated with input sequences and corrected sequences
@details Holds all SequenceRecord objects associated with an
ErrorX query. In addition it holds ErrorXOptions that has 
settings for processing, and ErrorPredictor that does the error correction itself.
@author Alex Sevy (alex.sevy@gmail.com)
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "SequenceRecords.hh"
#include "SequenceQuery.hh"
#include "SequenceRecord.hh"
#include "ErrorPredictor.hh"
#include "SequenceFeatures.hh"
#include "DataScaler.hh"
#include "util.hh"
#include "constants.hh"

using namespace std;

namespace errorx {

SequenceRecords::SequenceRecords( ErrorXOptions const & options ) :
	options_(new ErrorXOptions( options )),
	predictor_ (new ErrorPredictor( options.verbose() ))
{}

SequenceRecords::~SequenceRecords() {
	for ( int ii = 0; ii < size(); ++ii ) delete records_[ii];
	delete predictor_;
	delete options_;
}


SequenceRecords::SequenceRecords(SequenceRecords const & other) {
	// make deep copy of everything
	for ( int ii = 0; ii < other.size(); ++ii ) {
		records_.push_back( 
			new SequenceRecord( *other.get(ii) )
		);
	}

	options_ = new ErrorXOptions( *other.options_ );
	predictor_ = new ErrorPredictor( *other.predictor_ );
}

SequenceRecords::SequenceRecords( vector<SequenceRecords*> const & others ) {
	if ( others.size() == 0 ) {
		throw invalid_argument( "Error: trying to create a SequenceRecords object from an empty vector" );
	}

	// make deep copy of everything
	for ( int ii = 0; ii < others.size(); ++ii ) {
		for ( int jj = 0; jj < others[ii]->size(); ++jj ) {

			records_.push_back( 
				new SequenceRecord( *(others[ii]->get(jj)) )
			);
		}
	}


	// note: this just uses the settings from the first item in the vector
	// if they're not all uniform it will cause a bug
	options_ = new ErrorXOptions( *(others[0]->options_ ));
	predictor_ = new ErrorPredictor( *(others[0]->predictor_ ));
}

SequenceRecords::SequenceRecords( vector<SequenceRecord*> const & record_vector, 
	ErrorXOptions const & options ) {

	// make deep copy of everything
	for ( int ii = 0; ii < record_vector.size(); ++ii ) {
		records_.push_back( 
			new SequenceRecord( *record_vector[ii] )
		);
	}
	options_ = new ErrorXOptions( options );
	predictor_ = new ErrorPredictor( options.verbose() );
}

void SequenceRecords::import_from_tsv() {
	ios_base::sync_with_stdio (false);
	string line;
	ifstream file( options_->infile() );

	if ( !file.good() ) {
		throw invalid_argument( options_->infile()+" is not a valid file." );
		return;
	}

	while (getline (file, line)) {
		vector<string> tokens = util::tokenize_string<string>( line, "\t " );
		if ( tokens.size() != 4 ) {
			throw invalid_argument( "Error: line is not properly formatted. Proper format is four fields (SequenceID Full_sequence Quality Germline_sequence) separated by tabs.\nOffending line:\n"+line );
			return;
		}

		SequenceQuery query( tokens[0], tokens[1], tokens[2], tokens[3] );
		SequenceRecord* record = new SequenceRecord( 
				query,
				options_->verbose()
				);
		add_record( record );
	}
}

void SequenceRecords::import_from_list( vector<SequenceQuery> & queries ) {

	for ( int ii = 0; ii < queries.size(); ++ii ) {	
		SequenceRecord* record = new SequenceRecord( 
				queries[ii],
				options_->verbose()
				);

		add_record( record );
	}
}

void SequenceRecords::add_record( SequenceRecord* record ) {
	records_.push_back( record );
}

vector<SequenceRecord*> SequenceRecords::get_records() const { return records_; }

SequenceRecord* SequenceRecords::get( int i ) const {
	if ( i >= size() ) {
		throw out_of_range(
				"Error: index out of bounds. Requested position "+
				to_string(i) + " and only " +
				to_string(size()) + " records exist."
		);
	}
	return records_[i];
}

int SequenceRecords::size() const { return records_.size(); }

float SequenceRecords::estimate_error_rate() const {
 	int total_errors = 0;
 	int total_bases = 0;

 	double recall = constants::OPTIMIZED_RECALL;
 	double precision = constants::OPTIMIZED_PRECISION;

	for ( int ii = 0; ii < records_.size(); ++ii ) {
 		if ( records_[ ii ]->isGood() ) {
			total_bases += records_[ ii ]->full_nt_sequence().size();
 			total_errors += records_[ ii ]->n_errors();
		}
 	}
 	if ( options_->verbose() > 0 ) {
 		cout << total_errors << " errors found in " << total_bases << " total bases." << endl;
 	}

 	return (total_errors*precision/recall)/total_bases;
 }

vector<vector<string>> SequenceRecords::get_summary() const {
	vector<vector<string>> summary_data;

	for ( int ii = 0; ii < records_.size(); ++ii ) {
 		if ( records_[ ii ]->isGood() ) {
			summary_data.push_back( records_[ ii ]->get_summary() );
		}
 	}

	return summary_data;
}

int SequenceRecords::good_records() const {
	int good_records = 0;
	for ( int ii = 0; ii < records_.size(); ++ii ) {
		if ( records_[ii]->isGood() ) good_records++;
	}
	return good_records;
 }

vector<string> SequenceRecords::get_summary_labels() const {

	return vector<string> {
			"SequenceID",
			"V_gene",
			"V_identity",
			"V_Evalue",
			"D_gene",
			"D_identity",
			"D_Evalue",
			"J_gene",
			"J_identity",
			"J_Evalue",
			"Strand",
			"Chain",
			"Productive",
			"CDR3_NT_sequence",
			"CDR3_AA_sequence",
			"Full_NT_sequence",
			"Full_GL_NT_sequence",
			"PHRED_scores",
			"Full_AA_sequence",
			"Full_NT_sequence_corrected",
			"Full_AA_sequence_corrected",
			"N_errors"
	};
}

void SequenceRecords::print_summary() const {
	vector<string> summary_labels = get_summary_labels();
	vector<vector<string>> summary_data = get_summary();

	for ( int ii = 0; ii < summary_labels.size(); ++ii ) {
		cout << summary_labels[ ii ] << "\t";
	}
	cout << endl;
	for ( int ii = 0; ii < summary_data.size(); ++ii ) {
		for ( int jj = 0; jj < summary_data[ii].size(); ++jj ) {
			cout << summary_data[ ii ][ jj ] << "\t";
		}
		cout << endl;
	}
}

void SequenceRecords::write_summary() const {
	ofstream outfile( options_->outfile() );
	if ( !outfile.good() ) {
		throw invalid_argument( options_->outfile()+" is not a valid file." );
		return;
	}
	vector<string> summary_labels = get_summary_labels();
	vector<vector<string>> summary_data = get_summary();

	for ( int ii = 0; ii < summary_labels.size(); ++ii ) {
		outfile << summary_labels[ ii ] << "\t";
	}
	outfile << "\n";
	for ( int ii = 0; ii < summary_data.size(); ++ii ) {
		for ( int jj = 0; jj < summary_data[ii].size(); ++jj ) {
			outfile << summary_data[ ii ][ jj ] << "\t";
		}
		outfile << "\n";
	}
	outfile.close();
}

void SequenceRecords::correct_sequences_threaded( SequenceRecords* & records, 
												  int* & correction_progress ) 
{
	for ( int ii = 0; ii < records->size(); ++ii ) {
		try {
			SequenceRecord* current_record = records->get( ii );
			current_record->correct_sequence( 
				*(records->predictor_), 
				*(records->options_) );

			(*correction_progress)++;

		} catch ( exception & e ) {
			cout << "record could not be processed - exception caught : " 
				<< records->get(ii)->sequenceID() << endl;
			cout << e.what() << endl;
			continue;
		}
	}
}

vector<SequenceRecords*> SequenceRecords::chunk_records() {
	// If there is only one thread, I don't need to split into chunks
	if ( options_->nthreads() == 1 ) {
		// Make a deep copy of this and return it
		vector<SequenceRecords*> records_array = { 
			new SequenceRecords( records_, *options_ ) 
		};

		return records_array;
	} else {
		// Split records_ into chunks 
		// Shallow copy of pointers
		vector<vector<SequenceRecord*>> split_vectors = 
			util::split_vector<SequenceRecord*>( records_, options_->nthreads() );

		// Make an array to hold SequenceRecords
		vector<SequenceRecords*> records_array( options_->nthreads() );

		for ( int ii = 0; ii < options_->nthreads(); ++ii ) {
			// Make a new SequenceRecords object containing only its chunk
			// this is a deep copy so the old records are no longer needed
			records_array[ii] = new SequenceRecords( split_vectors[ii], *options_ );
		}

		return records_array;
	}
}

void SequenceRecords::correct_sequences( SequenceRecords* & records ) {


	int nthreads = records->options_->nthreads();
	int total_records = records->size();
	int verbose = records->options_->verbose();

	// make vector of correction progress atomic ints -
	// these are used to calculate progress across
	// threads without sharing data and slowing it down
	vector<int*> progress_vector( nthreads );
	for ( int ii = 0; ii < nthreads; ++ii ) {
		progress_vector[ ii ] = new int(0);
		// cout << "assigning value at address " << progress_vector[ii] << " : " << *(progress_vector[ii]); // TODO remove
	} 

	// get chunked SequenceRecords
	// this will deep copy the original pointers in records_
	vector<SequenceRecords*> chunked_records = records->chunk_records();

	// delete the original record
	delete records;
		
	vector<thread*> threads( nthreads );

	for ( int ii = 0; ii < nthreads; ++ii ) {
		threads[ii] = new std::thread(
				&SequenceRecords::correct_sequences_threaded,
				ref(chunked_records[ii]),
				ref(progress_vector[ii])
				);
	}

	if ( verbose > 0 ) {
		cout << "Correcting sequences..." << endl;
		track_progress( total_records, progress_vector );
	}

	// Wait for all threads to finish
	for ( int ii = 0; ii < nthreads; ++ii ) {
		threads[ii]->join();
  	}

	// // Now I need to collect all the chunked SequenceRecords into the parent
	records = new SequenceRecords( chunked_records );

	// Delete the threads that I used, along with the chunked records
	// All copies made were deep copies so this won't affect the merged records object
	for ( int ii = 0; ii < nthreads; ++ii ) {
		delete threads[ii];
		delete chunked_records[ii]; 
		delete progress_vector[ii];
	} 
}

void SequenceRecords::write_features() {
	vector<vector<double>> features_2d;
	
	for ( int ii = 0; ii < size(); ++ii ) {
		try {
			
			vector<vector<double>> temp_features = get(ii)->get_features( 
				*predictor_, 
				*options_ 
				);
		
			for ( int jj = 0; jj < temp_features.size(); ++jj ) {
				features_2d.push_back(temp_features[jj]);
			}

		} catch ( exception & e ) {
			cout << "record could not be processed - exception caught : " 
				<< get(ii)->sequenceID() << endl;
			cout << e.what() << endl;
			continue;
		}
	}

	ofstream file;
	file.open( options_->outfile()+".csv" );
	string delimiter = ",";

	/// 11.28.18 AMS changed to remove three features that
	/// are count-based and not normalized
	/// These three are abs_position, global_gc, and local_gc
	vector<string> labels = {"rel_position","global_gc_pct","local_gc_pct","global_avg_phred","local_avg_phred","nt_-81","nt_-82","nt_-83","nt_-84","nt_-85","nt_-86","nt_-71","nt_-72","nt_-73","nt_-74","nt_-75","nt_-76","nt_-61","nt_-62","nt_-63","nt_-64","nt_-65","nt_-66","nt_-51","nt_-52","nt_-53","nt_-54","nt_-55","nt_-56","nt_-41","nt_-42","nt_-43","nt_-44","nt_-45","nt_-46","nt_-31","nt_-32","nt_-33","nt_-34","nt_-35","nt_-36","nt_-21","nt_-22","nt_-23","nt_-24","nt_-25","nt_-26","nt_-11","nt_-12","nt_-13","nt_-14","nt_-15","nt_-16","nt_01","nt_02","nt_03","nt_04","nt_05","nt_06","nt_11","nt_12","nt_13","nt_14","nt_15","nt_16","nt_21","nt_22","nt_23","nt_24","nt_25","nt_26","nt_31","nt_32","nt_33","nt_34","nt_35","nt_36","nt_41","nt_42","nt_43","nt_44","nt_45","nt_46","nt_51","nt_52","nt_53","nt_54","nt_55","nt_56","nt_61","nt_62","nt_63","nt_64","nt_65","nt_66","nt_71","nt_72","nt_73","nt_74","nt_75","nt_76","nt_81","nt_82","nt_83","nt_84","nt_85","nt_86","phred_-8","phred_-7","phred_-6","phred_-5","phred_-4","phred_-3","phred_-2","phred_-1","phred_0","phred_1","phred_2","phred_3","phred_4","phred_5","phred_6","phred_7","phred_8","glnt_-81","glnt_-82","glnt_-83","glnt_-84","glnt_-85","glnt_-86","glnt_-71","glnt_-72","glnt_-73","glnt_-74","glnt_-75","glnt_-76","glnt_-61","glnt_-62","glnt_-63","glnt_-64","glnt_-65","glnt_-66","glnt_-51","glnt_-52","glnt_-53","glnt_-54","glnt_-55","glnt_-56","glnt_-41","glnt_-42","glnt_-43","glnt_-44","glnt_-45","glnt_-46","glnt_-31","glnt_-32","glnt_-33","glnt_-34","glnt_-35","glnt_-36","glnt_-21","glnt_-22","glnt_-23","glnt_-24","glnt_-25","glnt_-26","glnt_-11","glnt_-12","glnt_-13","glnt_-14","glnt_-15","glnt_-16","glnt_01","glnt_02","glnt_03","glnt_04","glnt_05","glnt_06","glnt_11","glnt_12","glnt_13","glnt_14","glnt_15","glnt_16","glnt_21","glnt_22","glnt_23","glnt_24","glnt_25","glnt_26","glnt_31","glnt_32","glnt_33","glnt_34","glnt_35","glnt_36","glnt_41","glnt_42","glnt_43","glnt_44","glnt_45","glnt_46","glnt_51","glnt_52","glnt_53","glnt_54","glnt_55","glnt_56","glnt_61","glnt_62","glnt_63","glnt_64","glnt_65","glnt_66","glnt_71","glnt_72","glnt_73","glnt_74","glnt_75","glnt_76","glnt_81","glnt_82","glnt_83","glnt_84","glnt_85","glnt_86","is_germline","local_SHM","global_SHM"};

	for ( int ii = 0; ii < labels.size(); ++ii ) {
		file << labels[ ii ];
		if ( ii != labels.size()-1 ) file << delimiter;
		
	}
	file << "\n";

	for ( int ii = 0; ii < features_2d.size(); ++ii ) {
		for ( int jj = 0; jj < features_2d[ii].size(); ++jj ) {
			file << features_2d[ ii ][ jj ];
			if ( jj != features_2d[ii].size()-1) file << delimiter;
		}
		file << "\n";
	}
	file.close();
}

void SequenceRecords::track_progress( int & total_records, vector<int*> & progress_vector ) {
	float progress = 0.0;
	float last_progress = -1;

	while ( true ) {
		int progress_sum = 0;
		// add up progress over all threads
		for ( int ii = 0; ii < progress_vector.size(); ++ii ) {
			// cout << "checking value at address " << progress_vector[ii] << " : " << *(progress_vector[ii]) << endl; // TODO remove

			progress_sum += (*progress_vector[ii]);
		}
		progress = (float)progress_sum/(float)total_records;

		if ( progress != last_progress ) { // only write to screen if the value has changed
			util::write_progress_bar( progress, progress_sum, total_records );
		}
		last_progress = progress;

		if ( progress >= 1.0 ) break;
	}
	cout << endl;
}

} // namespace errorx
