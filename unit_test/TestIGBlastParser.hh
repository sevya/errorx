/*
 * TestIGBlastParser.hh
 *
 *  Created on: Sep 29, 2018
 *		Author: alexsevy
 */

#ifndef UNIT_TEST_TESTIGBLASTPARSER_HH_
#define UNIT_TEST_TESTIGBLASTPARSER_HH_

#include <cxxtest/TestSuite.h>

#include "SequenceFeatures.hh"
#include "ErrorPredictor.hh"
#include "util.hh"
#include "errorx.hh"
#include <string>
#include <map>

#include "AbSequence.hh"

#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "IGBlastParser.hh"
#include "ErrorXOptions.hh"

using namespace std;
using namespace errorx;

class TestIGBlastParser : public CxxTest::TestSuite {

public:

	void setUp() {
		// Register control-C signal
		util::register_signal();

		ErrorXOptions options( "testing/test_sequences.fastq", "fastq" );
		options.errorx_base( "../" );
		options.verbose( 0 );
		options.allow_nonproductive( 1 );

		options.validate();
		options.count_queries();
		options.fastq_to_fasta();

		IGBlastParser parser;
		parser.blast( options );
		records_ = parser.parse_output( options );
	}

	void tearDown() {}

	void testReversedSequence() {

		AbSequence sequence = records_->get( 0 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG----ATGGTGTATGCTATA-----CTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"GCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVYAISCFDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCARGVMVYAISCFDYWGQGTLVTVSS");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*02");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testForwardSequence() {

		AbSequence sequence = records_->get( 1 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG----ATGGTGTATGCTATA-----CTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"GCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVYAISCFDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCARGVMVYAISCFDYWGQGTLVTVSS");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*02");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testBadD() {

		AbSequence sequence = records_->get( 2 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTAAAAAATATAAGCTGCTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG------------------------CTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"GCGAGAGGCGTCATGGTAAAAAATATAAGCTGCTTTGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVKNISCFDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCARGVMVKNISCFDYWGQGTLVTVSS");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*02");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testBadJ() {

		AbSequence sequence = records_->get( 3 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTA");

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG----ATGGTGTATGCTATA--------------");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"GCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTA" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVYAISCFD");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCARGVMVYAISCFD");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "N/A");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 92.308, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "N/A" );
	}

	void testNoJFound() {

		AbSequence sequence = records_->get( 4 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTGTATGCTATA" );

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG----ATGGTGTATGCTATA");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"N/A" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"N/A");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCARGVMVYAI");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "N/A");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), -1, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "N/A" );
	}

	void testNoDNoJ() {
		AbSequence sequence = records_->get( 5 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG" );

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"N/A" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"N/A");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVYYCAR");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "N/A");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), -1, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "N/A" );

	}

	void testLateStart() {

		AbSequence sequence = records_->get( 6 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"TCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTACTGTGCGAGAGG----ATGGTGTATGCTATA-----CTTTGACTACTGGGGCCAGGGAACCCTGGTCACCGTCTCCTCAG");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), 
			"GCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVYAISCFDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SLKLSSVTAADTAVYYCARGVMVYAISCFDYWGQGTLVTVSS");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*02");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testVOnly() {

		AbSequence sequence = records_->get( 7 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTA");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "TCAGTGGTTACTACTGGAGCTGGATCCGCCAGCCCCCAGGGAAGGGGCTGGAGTGGATTGGGGAAATCAATCATAGTGGAAGCACCAACTACAACCCGTCCCTCAAGAGTCGAGTCACCATATCAGTAGACACGTCCAAGAACCAGTTCTCCCTGAAGCTGAGCTCTGTGACCGCCGCGGACACGGCTGTGTATTA");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "TCAGTGGTTACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "SGYY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATCAATCATAGTGGAAGCACC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "INHSGST" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"N/A");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 3 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"SGYYWSWIRQPPGKGLEWIGEINHSGSTNYNPSLKSRVTISVDTSKNQFSLKLSSVTAADTAVY");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV4-34*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "N/A");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), -1, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), -1, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "N/A" );
	}

	void testBadV() {

		AbSequence sequence = records_->get( 8 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"N/A");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "N/A");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "GCGAGAGGCGTCATGGTGTATGCTATAAGCTGCTTTGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"ARGVMVYAISCFDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), -1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(), "N/A");

		TS_ASSERT_EQUALS( sequence.isGood(), 0 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.d_gene(), "IGHD2-8*01");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*02");

		TS_ASSERT_DELTA( sequence.v_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testIrrelevantSequence() {
		AbSequence sequence = records_->get( 9 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"N/A");
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "N/A");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "N/A" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "N/A" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(),"N/A");

		TS_ASSERT_EQUALS( sequence.translation_frame(), -1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(), "N/A");

		TS_ASSERT_EQUALS( sequence.isGood(), 0 );
		TS_ASSERT_EQUALS( sequence.chain(), "N/A" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "N/A");

		TS_ASSERT_DELTA( sequence.v_identity(), -1, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), -1, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), -1, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "N/A" );
	}


	void testLightChain() {

		AbSequence sequence = records_->get( 10 )->sequence();

		
		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"GATATTGTGATGACCCAGACTCCACTCTCTCTGTCCGTCACCCCTGGACAGCCGGCCTCCATCTCCTGCAAGTCTAGTCAGAGCCTCCTGCATGATGATGGAAAGACCTATTTGTATTGGTATTTGCAGAAGCCAGGCCAGTCTCCACAGCTCCTGATCTATGAGGTTTCCAACCGGTTCTCTGGAGTGCCAGATAGGTTCAGTGGCAGCGGGTCAGGGACAGATTTCACACTGAAAATCAGCCGGGTGGAGGCTGAGGATGTTGGGGTTTATTACTGCATGCGAAGTATACAGTTCGCGACTTTTGGCCAGGGGACCAAGCTGGAGATCAAAC");
		
		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "GATATTGTGATGACCCAGACTCCACTCTCTCTGTCCGTCACCCCTGGACAGCCGGCCTCCATCTCCTGCAAGTCTAGTCAGAGCCTCCTGCATAGTGATGGAAAGACCTATTTGTATTGGTACCTGCAGAAGCCAGGCCAGTCTCCACAGCTCCTGATCTATGAAGTTTCCAACCGGTTCTCTGGAGTGCCAGATAGGTTCAGTGGCAGCGGGTCAGGGACAGATTTCACACTGAAAATCAGCCGGGTGGAGGCTGAGGATGTTGGGGTTTATTACTGCATGCAAAGTATACAG------ACTTTTGGCCAGGGGACCAAGCTGGAGATCAAAC");
		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "CAGAGCCTCCTGCATGATGATGGAAAGACCTAT" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "QSLLHDDGKTY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "GAGGTTTCC" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "EVS" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "ATGCGAAGTATACAGTTCGCGACT" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(), "MRSIQFAT");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"DIVMTQTPLSLSVTPGQPASISCKSSQSLLHDDGKTYLYWYLQKPGQSPQLLIYEVSNRFSGVPDRFSGSGSGTDFTLKISRVEAEDVGVYYCMRSIQFATFGQGTKLEIK");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VK" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGKV2D-29*02");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGKJ2*01");

		TS_ASSERT_DELTA( sequence.v_identity(), 97.959, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), -1, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 100, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "97.96" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testMouse() {
		ErrorXOptions options( "testing/test_sequences.fastq", "fastq" );
		options.errorx_base( "../" );
		options.verbose( 0 );
		options.allow_nonproductive( 1 );
		options.species( "mouse" );

		options.validate();
		options.count_queries();
		options.fastq_to_fasta();

		IGBlastParser parser;
		parser.blast( options );
		SequenceRecordsPtr records = parser.parse_output( options );

		AbSequence sequence = records->get( 11 )->sequence();
		
		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"GAGGTTCAGCTGCAGCAATCTGGGGCAGAGCTTGTGAAGCCAGGGGCCTCAGTCAAGTTGTCCTGTACAGCTTCTGGCTTCAACATTAAAGACACCTATATACACTGGGTGAAGCAGAGGCCTGAACAGGGCCTGGAGTGGATTGGAAGGATTGATCCTGCGATTGGTAATACTAAATATGACCCGAAGTTCCAGGGCAAGGCCACTATAACAGCAGACACATCCTCCAACACAGCCTACCTGCAGCTCAGCAGCCTGACATCTGAGGACACTGCCGTCTATTACTGTTCTAGGGGGATTACCCCCTACTATGCTATAGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCA");

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "GAGGTTCAGCTGCAGCAGTCTGGGGCAGAGCTTGTGAAGCCAGGGGCCTCAGTCAAGTTGTCCTGCACAGCTTCTGGCTTCAACATTAAAGACACCTATATGCACTGGGTGAAGCAGAGGCCTGAACAGGGCCTGGAGTGGATTGGAAGGATTGATCCTGCGAATGGTAATACTAAATATGACCCGAAGTTCCAGGGCAAGGCCACTATAACAGCAGACACATCCTCCAACACAGCCTACCTGCAGCTCAGCAGCCTGACATCTGAGGACACTGCCGTCTATTACTGTGCTAG-------------TACTATGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCA");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "GGCTTCAACATTAAAGACACCTAT" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "GFNIKDTY" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "ATTGATCCTGCGATTGGTAATACT" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "IDPAIGNT" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "TCTAGGGGGATTACCCCCTACTATGCTATAGACTAC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(), "SRGITPYYAIDY");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"EVQLQQSGAELVKPGASVKLSCTASGFNIKDTYIHWVKQRPEQGLEWIGRIDPAIGNTKYDPKFQGKATITADTSSNTAYLQLSSLTSEDTAVYYCSRGITPYYAIDYWGQGTSVTVSS");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VH" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "IGHV14-3*02");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "IGHJ4*01");

		TS_ASSERT_DELTA( sequence.v_identity(), 98.294, 0.001 );
		TS_ASSERT_DELTA( sequence.d_identity(), 100, 0.001 );
		TS_ASSERT_DELTA( sequence.j_identity(), 98.039, 0.001 );

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "98.29" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "98.04" );
	}


	void testTCRA() {
		ErrorXOptions options( "testing/test_sequences.fastq", "fastq" );
		options.errorx_base( "../" );
		options.verbose( 0 );
		options.allow_nonproductive( 1 );
		options.species( "human" );
		options.igtype( "TCR" );

		options.validate();
		options.count_queries();
		options.fastq_to_fasta();

		IGBlastParser parser;
		parser.blast( options );
		SequenceRecordsPtr records = parser.parse_output( options );

		AbSequence sequence = records->get( 12 )->sequence();

		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"AAAAATGAAGTGGAGCAGAGTCCTCAGAACCTGACTGCCCAGGAAGGAGAATTTATCACAATCAACTGCAGTTACTCGGTAGGAATAAGTGCCTTACACTGGCTGCAACAGCATCCAGGAGGAGGCATTGTTTCCTTGTTTATGCTGAGCTCAGGGAAGAAGAAGCATGGAAGATTAATTGCCACAATAAACATACAGGAAAAGCACAGCTCCCTGCACATCACAGCCTCCCATCCCAGAGACTCTGCCGTCTACATCTGTGCTGTCCCTTACACCGACAAGCTCATCTTTGGGACTGGGACCAGATTACAAGTCTTTCCAA");

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "AAAAATGAAGTGGAGCAGAGTCCTCAGAACCTGACTGCCCAGGAAGGAGAATTTATCACAATCAACTGCAGTTACTCGGTAGGAATAAGTGCCTTACACTGGCTGCAACAGCATCCAGGAGGAGGCATTGTTTCCTTGTTTATGCTGAGCTCAGGGAAGAAGAAGCATGGAAGATTAATTGCCACAATAAACATACAGGAAAAGCACAGCTCCCTGCACATCACAGCCTCCCATCCCAGAGACTCTGCCGTCTACATCTGTGCTGTC----ACACCGACAAGCTCATCTTTGGGACTGGGACCAGATTACAAGTCTTTCCAA");

		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "GTAGGAATAAGTGCC" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "VGISA" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "CTGAGCTCAGGGAAG" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "LSSGK" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "GCTGTCCCTTACACCGACAAGCTCATC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(), "AVPYTDKLI");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"KNEVEQSPQNLTAQEGEFITINCSYSVGISALHWLQQHPGGGIVSLFMLSSGKKKHGRLIATINIQEKHSSLHITASHPRDSAVYICAVPYTDKLIFGTGTRLQVFP");

		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VA" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "TRAV41*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "TRAJ34*01");

		TS_ASSERT_EQUALS( sequence.v_identity(), 100);
		TS_ASSERT_EQUALS( sequence.d_identity(), -1);
		TS_ASSERT_EQUALS( sequence.j_identity(), 100);

		TS_ASSERT_EQUALS( sequence.v_identity_fmt(), "100.00" );
		TS_ASSERT_EQUALS( sequence.d_identity_fmt(), "N/A" );
		TS_ASSERT_EQUALS( sequence.j_identity_fmt(), "100.00" );
	}

	void testTCRB() {
		ErrorXOptions options( "testing/test_sequences.fastq", "fastq" );
		options.errorx_base( "../" );
		options.verbose( 0 );
		options.allow_nonproductive( 1 );
		options.species( "human" );
		options.igtype( "TCR" );

		options.validate();
		options.count_queries();
		options.fastq_to_fasta();

		IGBlastParser parser;
		parser.blast( options );
		SequenceRecordsPtr records = parser.parse_output( options );

		AbSequence sequence = records->get( 13 )->sequence();


		TS_ASSERT_EQUALS( sequence.full_nt_sequence(),"AAGGCTGGATTCACTCAAACTCCAAGATATCTGATCAAAACGAGAGGACAGCAAGTGACCCTGAGCTGCTCCCCTATCCCTGGGCATCGGAGGGTATCCTGGGACCAACAGACCCCAGGACAGGGCCTTCAGTTCCTCTTTGAATACTTCAGAGAGACACAGAGAAACAAAGGAAACTTCCCTGGTCGATTCTCAGGGCGCCAGTTCTCTAACTCTCGCTCTGAGATGAATGTGAGCACCTTGGAGCTGGGGGACTCGGCCCTTTATCTTTGCGCCAGCAGCGACGGGACCGGACAAAACTATGGCTACACCTTCGGTTCGGGGACCAGGTTAACCGTTGTAG");

		TS_ASSERT_EQUALS( sequence.full_gl_nt_sequence(), "AAGGCTGGAGTCACTCAAACTCCAAGATATCTGATCAAAACGAGAGGACAGCAAGTGACACTGAGCTGCTCCCCTATCTCTGGGCATAGGAGTGTATCCTGGTACCAACAGACCCCAGGACAGGGCCTTCAGTTCCTCTTTGAATACTTCAGTGAGACACAGAGAAACAAAGGAAACTTCCCTGGTCGATTCTCAGGGCGCCAGTTCTCTAACTCTCGCTCTGAGATGAATGTGAGCACCTTGGAGCTGGGGGACTCGGCCCTTTATCTTTGCGCCAGCAGC---------------AACTATGGCTACACCTTCGGTTCGGGGACCAGGTTAACCGTTGTAG");

		
		TS_ASSERT_EQUALS( sequence.cdr1_nt_sequence(), "CCTGGGCATCGGAGG" );
		TS_ASSERT_EQUALS( sequence.cdr1_aa_sequence(), "PGHRR" );

		TS_ASSERT_EQUALS( sequence.cdr2_nt_sequence(), "TACTTCAGAGAGACACAG" );
		TS_ASSERT_EQUALS( sequence.cdr2_aa_sequence(), "YFRETQ" );

		TS_ASSERT_EQUALS( sequence.cdr3_nt_sequence(), "GCCAGCAGCGACGGGACCGGACAAAACTATGGCTACACC" );
		TS_ASSERT_EQUALS( sequence.cdr3_aa_sequence(), "ASSDGTGQNYGYT");

		TS_ASSERT_EQUALS( sequence.translation_frame(), 1 );
		TS_ASSERT_EQUALS( sequence.full_aa_sequence(),"KAGFTQTPRYLIKTRGQQVTLSCSPIPGHRRVSWDQQTPGQGLQFLFEYFRETQRNKGNFPGRFSGRQFSNSRSEMNVSTLELGDSALYLCASSDGTGQNYGYTFGSGTRLTVV");


		TS_ASSERT_EQUALS( sequence.isGood(), 1 );
		TS_ASSERT_EQUALS( sequence.chain(), "VB" );
		TS_ASSERT_EQUALS( sequence.v_gene(), "TRBV5-1*01");
		TS_ASSERT_EQUALS( sequence.d_gene(), "N/A");
		TS_ASSERT_EQUALS( sequence.j_gene(), "TRBJ1-2*01");

		TS_ASSERT_EQUALS( sequence.v_identity(), 97.518);
		TS_ASSERT_EQUALS( sequence.d_identity(), 100);
		TS_ASSERT_EQUALS( sequence.j_identity(), 100);

	}

	void testQualityCorrect() {

		AbSequence sequence = records_->get( 14 )->sequence();
			
		TS_ASSERT_EQUALS( 
			sequence.quality_string_trimmed(),
			"########################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4"
			);



		TS_ASSERT_EQUALS(
			sequence.full_nt_sequence(),
			"AGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCA"
			);


		TS_ASSERT_EQUALS(
			sequence.full_gl_nt_sequence(),
			"AGGGCAGAGTCACGATTACCGCGGACAAATCCACGAGCACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAGGACACGGCCGTGTATTACTGTGCGAGAGA------------------------CTGGGGCCAAGGGACCACGGTCACCGTCTCCTCA"
			);
	}

	void testAdditionalFastq() {

		SequenceRecordPtr current = records_->get( 15 );
		TS_ASSERT( current->isGood() );

		AbSequence sequence = current->sequence();

		TS_ASSERT_EQUALS( 
			sequence.full_nt_sequence(),
			"GTGCAGCTGGTGGAGTCTGGGGGAGGCTTGGTGCAGCCTGGGGAGTCTCTGAGACTCTCCTGTGCAGCCTCTGGATTCACGTTCAGCCTGTACGACATGGCATGGGTCCGCCAGGCTCCAGGGAAGGGACTCGAGTGGGTCTCAGGTATGAGTG---GTGGTGGCCGTGAATACTATGTAGACTCCGTGAAGGGCCGATTCACCATCTCCAGAGACAACGCCAAGAACACGCTGTATCTGCAAATGAACAGCCTGAAAACTGAGGACACTGCCATTTATATCTGCGCCACAATGGTCTGGCACTATACCGACCCCGAGGTTTGGGGCCAGGGCACCCAGGTCACCGTCTCC"
			);

		TS_ASSERT_EQUALS(
			sequence.quality_string_trimmed(),
			"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG-\")\"2,'''-,'\",''\"'-\"-\"))96)3\"0,\"3-'\"\"0-)\"''\"\"''\"\"\"3))-\"\"-''3''\";066,\"6>*A9236<BA>'25'693?-3*-*3*>89<*3*\"05   DAB95>BAAD9?B;*?DB9DD\"DD?B6BDDDDDDDDBBDBABDDDDDDDD?DDDDDDDDDDDDDDDB8BDBADBBBB?BDDDDDDDB69B>ABDDDBBBD9D366B?D?DDB3BD?D?DDDDDBDA36>>?>A0?>DGGGGGGGGGGFGGGGGGGGGGGGGGGGGGGGGGGGGFGGGGGGGGGGGGGGGCCCCC" 
			);

		// Check that the PHRED is correctly calculated by ignoring those dashes
		SequenceFeatures f( *current, 153 );
		vector<int> quality_window = f.quality_window();

		vector<int> empirical_quality_window = {23, 24, 27, 9, 18, 9, 1, 15, 20, -1, -1, -1, 35, 32, 33, 24, 20};
		TS_ASSERT_EQUALS( quality_window, empirical_quality_window );
		
		TS_ASSERT_DELTA( util::phred_avg_realspace( empirical_quality_window ), 10.9304856, pow(10,-6));
		vector<double> vect = f.get_feature_vector();
		TS_ASSERT_DELTA( vect[ 3 ], 10.9304856/40, pow(10,-6) );

		current = records_->get( 16 );
		TS_ASSERT( current->isGood() );

		sequence = current->sequence();

		TS_ASSERT_EQUALS(
			sequence.full_nt_sequence(),
			"CATGTGCAGCTGGTGGAGTCTGGCGGAGGCTCGGTGCAGGCTTGAGGGTCTCTGCGACTCACTTTTGTAGCCTCA-----CAGC----GTGCCCATTGCATGGGCTTGGTCCGCCCGTCTCCATTTGTGGGGCTCGGGTGGTTCTCAGGTATTTCGACTGGTGCTGGTCACACATTCTAAGCATACTCCGCGACCGGCCGATTCACCATCTCCAGAGACAACGCCCACAACACACTATATCTCCAACTTACCAGCCTTAACACTCAGGCCACTGCCCTTTATTACT"
			);


		TS_ASSERT_EQUALS( 
			sequence.quality_string_untrimmed(),
			"@C-@BFGGGGGFFDFGGC8FCDDD@FEE9-86:+@@CD++B7@8,,6@,6,,,,86CEG,,++8@BFCC<,9,:,66,CC,C,,C+8,,C,,CE9C,C,,+9?,++:B?+>=+++84:A,,,,,<:+++48B++=F7+8:>B@93=>,:@D@,+@>8,@,,@,,@,7@,6@FCB@,,,>,,,7<D7*>******4=<*4<><FFFCC9B@+5>**202;585***/*1/2*;;+3++2<+3+8+9+++0+0*/9+0+*:6>**0**/**966546)2*17*19*1).7)0(0*-,(.)--"
			);

		TS_ASSERT_EQUALS(
			sequence.quality_string_trimmed(),
			"GGGGGFFDFGGC8FCDDD@FEE9-86:+@@CD++B7@8,,6@,6,,,,86CEG,,++8@BFCC<,9,:,66,CC,     C,,C    +8,,C,,CE9C,C,,+9?,++:B?+>=+++84:A,,,,,<:+++48B++=F7+8:>B@93=>,:@D@,+@>8,@,,@,,@,7@,6@FCB@,,,>,,,7<D7*>******4=<*4<><FFFCC9B@+5>**202;585***/*1/2*;;+3++2<+3+8+9+++0+0*/9+0+*:6>**0**/**966546)2*17*19"
			);
	}

	void testDuplicateTags() {

		TS_ASSERT_EQUALS( 
			records_->get( 17 )->sequence().sequenceID(), 
			"forward_sequence_1" 
			);

		TS_ASSERT_EQUALS( 
			records_->get( 18 )->sequence().sequenceID(), 
			"forward_sequence_2" 
			);

		TS_ASSERT_EQUALS( 
			records_->get( 19 )->sequence().sequenceID(), 
			"forward_sequence_3" 
			);
	}

	SequenceRecordsPtr records_;
};



#endif /* UNIT_TEST_TESTIGBLASTPARSER_HH_ */
