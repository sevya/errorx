/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file SequenceRecords.hh
@brief Collection of SequenceRecord objects that holds all
data associated with input sequences and corrected sequences
@details Holds all SequenceRecord objects associated with an
ErrorX query. In addition it holds ErrorXOptions that has 
settings for processing, and ErrorPredictor that does the error correction itself.
@author Alex Sevy (alex@endeavorbio.com)
*/

#ifndef SEQUENCERECORDS_HH_
#define SEQUENCERECORDS_HH_

#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <functional>

#include "SequenceQuery.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "util.hh"

using namespace std;

namespace errorx {

class SequenceRecords {

public:
	/**
		Destructor. Deletes references to member variables options_ 
		and predictor_. Also deletes references to every 
		SequenceRecord contained in records_. 
		Every time a SequenceRecords object is copied a deep copy is 
		made, so none of these pointers are shared.
	*/
	~SequenceRecords();

	/**
		Constructor using ErrorXOptions as argument.
	*/
	SequenceRecords( ErrorXOptions const & options );

	/**
		Copy constructor. Deep copy of all member variables.
	*/
	SequenceRecords( SequenceRecords const & other );

	/**
		Constructor from vector of SequenceRecords. Useful for
		concatenating all of these SequenceRecords into a single object. 
		Deep copy of all member variables so they can be safely 
		deleted after. Uses the ErrorXOptions member variable from
		the first item in the vector to get its options.
	*/
	SequenceRecords( vector<SequenceRecords*> const & others );

	/**
		Constructor from SequenceRecord vector. Useful for
		concatenating many individual SequenceRecord objects
		into a single object. Deep copy of all member variables 
		so they can be safely deleted after.
	*/
	SequenceRecords( vector<SequenceRecord*> const & record_vector, ErrorXOptions const & options );

	/**
		Populates SequenceRecords object with elements from
		a TSV file. TSV file should contain the following 
		columns, in order: 
		SequenceID, NTSequence, GermlineSequence, PhredScore
	*/
	void import_from_tsv();

	/**
		Populates SequenceRecords object with elements from
		a vector of SequenceQuery objects

		@param queries Vector of SequenceQuery objects to 
		use to fill SequenceRecords
	*/
	void import_from_list( vector<SequenceQuery> & queries );

	/**
		After correcting the member SequenceRecord objects, 
		calculates the error rate over the whole set 

		@return float value of estimated error rate
	*/
	float estimate_error_rate() const;

	/**
		Adds a SequenceRecord to the records_ member variable.
		Does not copy the record, just assigns it

		@param record SequenceRecord to add to records_
	*/
	void add_record( SequenceRecord* record );

	/**
		Get the number of SequenceRecord objects held internally

		@return size of records_ variable
	*/
	int size() const;

	/**
		Get the SequenceRecord objects held internally

		@return records_ member variable
	*/
	vector<SequenceRecord*> get_records() const;

	/**
		Get a specific SequenceRecord

		@param i index of desired SequenceRecord

		@return SequenceRecord at index i
	*/
	SequenceRecord* get( int i ) const;

	/**
		Get number of "good" SequenceRecord objects
		SequenceRecords are defined as "good" by the
		IGBlastParser if they fit certain criteria,
		such as confident V assignment, productive, etc.
		Whether a nonproductive record is considered 
		"good" is determined by option allow_nonproductive
		in ErrorXOptions

		@return number of "good" records
	*/
	int good_records() const;

	/** 
		Get number of productive SequenceRecord objects

		@return number of productive records
	*/
	int productive_records() const;

	/**
		Returns a vector of string vectors, where each
		string vector is a summary of a SequenceRecord

		@return vector of summaries of each SequenceRecord
	*/
	vector<vector<string>> get_summary() const;

	/**
		Returns a vector of labels, where the labels describe
		the entries from get_summary()


		@return vector of labels
	*/
	vector<string> get_summary_labels() const;

	/**
		Wraps the get_summary() function and writes to 
		std out.
	*/
	void print_summary() const;

	/**
		Wraps the get_summary() function and writes to 
		the outfile_ variable of ErrorXOptions.
	*/
	void write_summary() const;

	/**
		Runs error correction protocol on each SequenceRecord
		object. Modifies "records" in-place. Static qualifier
		allows it to be used easily in multi-threading.

		@param records Collection of SequenceRecord objects to be
		error corrected
	*/
	static void correct_sequences( SequenceRecords* & records );
	
	/**
		For debugging purposes. Gets features from each SequenceRecord 
		object and writes to a file.
	*/
	void write_features();

	/**
		Get the options file used for these records
	*/
	ErrorXOptions* get_options() const;

	/**
		Count the number of unique sequences and unique clonotypes
		after error correction
	*/
	void count_sequences();
	
	
	/**
		Functions to return the count of unique sequences and clonotypes
		of different types
	*/
	int unique_nt_sequences() const;
	int unique_corrected_nt_sequences() const;
	int unique_aa_sequences() const;
	int unique_clonotypes() const;
	map<string,int> vgene_counts() const;
	map<string,int> jgene_counts() const;
	
private:
	/**
		Chunks the current SequenceRecords object into N parts,
		where N is the number of threads used in multithreading.
		Deep copies of each member variable are made, so the 
		parent object can be safely deleted after chunking.
	*/
	vector<SequenceRecords*> chunk_records();
	
	/**
		Corrects a single SequenceRecords object in one thread.

		@param records SequenceRecords object to be corrected
		@param update callback function to update progress that takes in
		(done,total,mutex*)
		@param m mutex* to keep track of threads
		@param total total number of records over all threads
	*/
	static void correct_sequences_threaded( SequenceRecords* & records, function<void(int,int,mutex*)>* update, mutex* m, int & total);

	/**
		Tracks progress of error correction in multiple threads 

		@param total_records Total num. of records to be corrected
		@param progress vector of atomic ints to track progress, each one 
		corresponding to a separate thread
	*/
	static void track_progress( int & total_records, vector<int*> & progress_vector );


	vector<SequenceRecord*> records_;
	ErrorXOptions* options_;
	ErrorPredictor* predictor_;

	/**
		Std maps that hold the unique sequences and clonotypes contained in 
		the dataset
	*/
	map<string,int> sequence_map_;
	map<string,int,function<bool(string,string)> > corrected_sequence_map_;
	map<string,int> aa_sequence_map_;
	map<string,int,function<bool(string,string)> > clonotype_map_;

	map<string,int> vgene_map_;
	map<string,int> jgene_map_;
};

} // namespace errorx

#endif /* SEQUENCERECORDS_HH_ */
