/**
@file SequenceRecords.hh
@brief Collection of SequenceRecord objects that holds all
data associated with input sequences and corrected sequences
@details Holds all SequenceRecord objects associated with an
ErrorX query. In addition it holds ErrorXOptions that has 
settings for processing, DataScaler that scales the input
data, and ErrorPredictor that does the error correction itself.
@author Alex Sevy (alex.sevy@gmail.com)
*/

#ifndef SEQUENCERECORDS_HH_
#define SEQUENCERECORDS_HH_

#include <vector>
#include <iostream>
#include <memory>

#include "SequenceQuery.hh"
#include "SequenceRecord.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"
#include "DataScaler.hh"

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

		@return number of "good" records
	*/
	int good_records() const;

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
		@param correction_progress atomic int to track progress
	*/
	static void correct_sequences_threaded( SequenceRecords* & records, atomic<int>* & correction_progress );

	/**
		Tracks progress of error correction in multiple threads 

		@param total_records Total num. of records to be corrected
		@param correction_progress atomic int to track progress
	*/
	static void track_progress( int & total_records, atomic<int>* & correction_progress );

	vector<SequenceRecord*> records_;

	ErrorXOptions* options_;
	ErrorPredictor* predictor_;


};

} // namespace errorx

#endif /* SEQUENCERECORDS_HH_ */
