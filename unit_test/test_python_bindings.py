import unittest
import errorx as ex
from errorx import ErrorXOptions

class TestErrorXLibrary(unittest.TestCase):
	
	@classmethod
	def setUpClass(cls):
		cls.sequenceID = 'SRR3175015.933'
		cls.sequence = 'TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG' 
		cls.germline_sequence = 'TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG'
		cls.phred_score = '###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,'
		cls.corrected_seq = 'TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG'

	def test_run_protocol(self):
		options = ex.ErrorXOptions('testing/test.fastq','fastq')
		options.outfile( 'testing/out.tsv' )
		options.species( 'mouse' )
		ex.run_protocol( options )

		output = '''
SRR3175015.933	IGHV1-76*01	73.276000	1.12E-18	N/A	N/A	N/A	IGHJ2*01	94.872000	2.38E-14	-	VH	True	GCGGGAGAGGAGGCTTTGTCCTTCGTTTACTAC	AGEEALSFVYY	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	YSRGTPKDGGTXSADKSSSAACLXXSSLKAGDSAVCSXAGEEALSFVYYWGQGTTLTGSS	4
'''.strip()

		self.assertEqual(
			open( 'testing/out.tsv' ).readlines()[1].strip(),
			output 
			)
	

		options = ex.ErrorXOptions('testing/test.tsv','tsv')
		options.outfile( 'testing/out.tsv' )
		options.species( 'mouse' )
		ex.run_protocol( options )

		output = '''	
SRR3175015.933		N/A	N/A		N/A	N/A		N/A	N/A		VH	False	N/A	N/A	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	N/A	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	N/A	4
		'''.strip()

		self.assertEqual(
			open( 'testing/out.tsv' ).readlines()[1].strip(),
			output 
			)

	def test_correct_sequences(self):

		N = 501
		sequences = [self.sequence]*N

		germline_sequences = [self.germline_sequence]*N
		phred_scores = [self.phred_score]*N
		results = ex.correct_sequences(sequences, 
			   germline_sequences,
			   phred_scores )
		self.assertEqual(results[0], self.corrected_seq)

	def test_numpy(self):
		try:
			import numpy as np
			N = 10

			sequences = np.array([self.sequence]*N)
			germline_sequences = np.array([self.germline_sequence]*N)
			phred_scores = np.array([self.phred_score]*N)

			results = ex.correct_sequences(sequences, 
				   germline_sequences,
				   phred_scores )
			self.assertEqual(results[0], self.corrected_seq)
		except ImportError:
			pass

		try:
			import pandas as pd
			sequences = pd.Series([self.sequence]*N)
			germline_sequences = pd.Series([self.germline_sequence]*N)
			phred_scores = pd.Series([self.phred_score]*N)

			results = ex.correct_sequences(sequences, 
				   germline_sequences,
				   phred_scores )
			self.assertEqual(results[0], self.corrected_seq)
		except ImportError:
			pass


	def test_predicted_errors(self):

		result = ex.get_predicted_errors( self.sequence, 
			   self.germline_sequence,
			   self.phred_score )

		self.assertAlmostEqual(result[132], 0.486122388, 9)
		self.assertAlmostEqual(result[133], 0.348359625, 9)
		self.assertAlmostEqual(result[134], 0.143818731, 9)
		self.assertAlmostEqual(result[135], 0.458915133, 9)
		self.assertAlmostEqual(result[136], 0.306816917, 9)
		self.assertAlmostEqual(result[137], 0.022584986, 9)
		self.assertAlmostEqual(result[138], 0.513773250, 9)


if __name__ == '__main__':
	unittest.main()
