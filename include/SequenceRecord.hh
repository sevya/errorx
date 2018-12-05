/**
@file SequenceRecord.hh
@brief A record of one sequence to be error corrected
@details Single nucleotide sequence to be fed to error
correction. Can be initialized from a FASTQ file, TSV file,
or directly from a SequenceQuery.
@author Alex Sevy (alex.sevy@gmail.com)
*/


#ifndef SEQUENCERECORD_HH_
#define SEQUENCERECORD_HH_

#include <iostream>
#include <string>
#include <vector>
#include "SequenceQuery.hh"
#include "ErrorXOptions.hh"
#include "ErrorPredictor.hh"

using namespace std;

namespace errorx {

/**
	Enum representing whether a SequenceRecord is
	an antibody heavy or light chain
*/
enum chain_type{ VH, VL };

class SequenceRecord {

public:
	
	/**
		Empty constructor. Initializes all variables to
		empty string or 0.
	*/	
	SequenceRecord();

	/**
		Copy constructor. Copies all member variables
	*/	
	SequenceRecord(SequenceRecord const & copy);

	/**
		Constructs a SequenceRecord from a vector of
		string, representing the lines of an IGBlast
		query. Goes through and extracts all information
		about the sequence by parsing the IGBlast output.

		@param lines vector of string representing lines of 
		IGBlast query
		@param verbose verbosity level to write error messages to the screen?
		@param allow_nonproductive if a NT sequence is non-
		productive, still allow it for error correction
	*/
	SequenceRecord( vector<string> const & lines, 
				int verbose=1,
				bool allow_nonproductive=0
				);

	/**
		Constructs a SequenceRecord from a SequenceQuery.
		The SequenceQuery contains only the minimal info
		needed for error correction, which is sequence ID,
		NT sequence, germline sequence, and phred score.

		@param query SequenceQuery object to correct
		@param verbose verbosity level to write error messages to the screen?
		@param allow_nonproductive if a NT sequence is non-
		productive, still allow it for error correction
	*/
	SequenceRecord( SequenceQuery & query,
				int verbose=1
				);

	/**
		Get all the information summarizing a SequenceRecord

		@return vector of strings summarizing the record
	*/
	vector<string> get_summary() const;

	/**
		Print key information related to this record
	*/
	void print() const;

	/**
		Correct this SequenceRecord and modify in place with 
		the corrected sequence. Params are generally passed
		by a parent SequenceRecords object.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	void correct_sequence( ErrorPredictor const & predictor, 
						   ErrorXOptions const & options );


	/**
		Get the predicted probability of error for each base.

		@return vector std::pair, where first is the position
		and second is the probability of error
	*/
	vector<pair<int,double>> get_predicted_errors();

	/**
		Return whether this record is "good". Records can be called
		"bad" for the following reasons:
		1. IGBlast query does not fit the correct format and couldn't be parsed
		2. V gene Evalue > 1e-3
		3. Sequence is not productive (unless allow_nonproductive_ is True)
		4. PHRED score is not same length as sequence

		@return whether sequence is considered good
	*/
	bool isGood();

	/**
		Get the numeric features associated with this record. Params are generally passed
		by a parent SequenceRecords object.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	vector<vector<double>> get_features( ErrorPredictor const & predictor,
		ErrorXOptions const & options );

	/**
		Getters and setters
	*/
	string full_nt_sequence() const;
	string full_gl_nt_sequence() const;
	string full_nt_sequence_corrected() const;
	string cdr3_aa_sequence() const;
	string full_aa_sequence() const;
	string v_gene() const;
	string d_gene() const;
	string j_gene() const;
	double v_identity() const;
	double d_identity() const;
	double j_identity() const;
	int n_errors() const;
	string sequenceID() const;
	vector<int> quality_array() const;
	int gc_count() const;
	double gc_pct() const;
	double shm() const;
	chain_type chain() const;
	string quality_string() const;


private:
	/**
		Initialize all member variables to 0 or ""
	*/
	void initialize_variables();

	/**
		Processes input variables to assign several
		member variabels. First decodes PHRED string 
		into an array of integer values. Next calculates GC 
		content and SHM over the whole sequence.
	*/
	void calculate_metrics();

	/**
		Translates a character from a PHRED string 
		to its integer counterpart. Assumes an offset
		of 33, as used by Illumina.

		@return int value corresponding to PHRED char
	*/
	int decode( char qual, int base );

	/**
		Assemble the full NT sequence for both the query 
		and germline based on the IGBlast query
	*/
	void assemble_full_sequence();

	/**
		Parse line of IGBlast query corresponding to V region

		@param data V region line of IGBlast query 
	*/
	void parse_vregion_string( vector<string> const & data );

	/**
		Parse line of IGBlast query corresponding to D region

		@param data D region line of IGBlast query 
	*/
	void parse_dregion_string( vector<string> const & data );

	/**
		Parse line of IGBlast query corresponding to D region

		@param data D region line of IGBlast query 
	*/
	void parse_jregion_string( vector<string> const & data );

	/**
		Parse line of IGBlast query corresponding to V region

		@param data V region line of IGBlast query 
	*/
	void parse_query_string( vector<string> const & tokens );

	void parse_rearrangement_string( vector<string> const & summary_data );

	void parse_junction_string( vector<string> const & junction_data );

	void parse_subregion_string( vector<string> const & tokens );

	/**
		Predict error probabilities for each base and modify
		this record in place. Wrapped by correct_sequence.

		@param predictor ErrorPredictor for prediction
		@param options Options for processing
	*/
	void predict_errors( ErrorPredictor const & predictor,
			ErrorXOptions const & options );

	string sequenceID_;
	string v_gene_;
	string d_gene_;
	string j_gene_;
	string strand_;
	chain_type chain_;
	bool productive_;

	string cdr3_nt_sequence_;
	string cdr3_aa_sequence_;

	string full_nt_sequence_;
	string full_gl_nt_sequence_;
	string full_aa_sequence_;

	string quality_string_;

	double v_identity_;
	double v_evalue_;
	double d_identity_;
	double d_evalue_;
	double j_identity_;
	double j_evalue_;

	string full_nt_sequence_corrected_;
	string full_aa_sequence_corrected_;

	int n_errors_;

	vector<pair<int,double>> predicted_errors_all_;

	string v_nts_;
	string v_gl_nts_;
	string d_nts_;
	string d_gl_nts_;
	string j_nts_;
	string j_gl_nts_;

	string full_junction_;
	string full_gl_junction_;

	int query_start_;

	string quality_string_full_;

	bool good_sequence_;

	bool hasV_, hasD_, hasJ_;

	// Machine sequencing_machine_;
	vector<int> quality_array_;

	int gc_count_;
	double gc_pct_;
	double shm_;

	int translation_frame_;

	int verbose_;
	bool allow_nonproductive_;

};

} // namespace errorx


#endif /* SEQUENCERECORD_HH_ */
