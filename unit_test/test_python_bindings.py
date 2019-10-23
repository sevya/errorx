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
		cls.corrected_seq = 'TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG'

	def test_run_protocol(self):
		options = ex.ErrorXOptions('testing/test.fastq','fastq')
		options.outfile( 'testing/out.tsv' )
		options.species( 'mouse' )
		ex.run_protocol( options )

		output = '''
SRR3175015.933	IGHV1-76*01	73.28	1.12E-18	N/A	N/A	N/A	IGHJ2*01	94.87	2.38E-14	-	VH	True	N/A	N/A	N/A	N/A	GCGGGAGAGGAGGCTTTGTCCTTCGTTTACTAC	AGEEALSFVYY	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS	TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	YSXGTPKDGGTXSADKSSSAACLXXSSLKAGDSAVCXXAGEEALSFVYYWGQGTTLTGSS	6
'''.strip()
	
		with open( 'testing/out.tsv', 'r' ) as f:
			file_contents = f.readlines()[1].strip()
		
		for item1,item2 in zip(file_contents.split(),output.split()):
			self.assertEqual( item1,item2 )
	

		options = ex.ErrorXOptions('testing/test.tsv','tsv')
		options.outfile( 'testing/out.tsv' )
		options.species( 'mouse' )
		ex.run_protocol( options )

		output = '''	
SRR3175015.933	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	False	N/A	N/A	N/A	N/A	N/A	N/A	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	N/A	TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	N/A	6
		'''.strip()

		with open( 'testing/out.tsv', 'r' ) as f:
			file_contents = f.readlines()[1].strip()
		
		for item1,item2 in zip(file_contents.split(),output.split()):
			self.assertEqual( item1,item2 )

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

		for line in open('validation.txt'):
			position,value = line.split()
			self.assertAlmostEqual(result[int(position)], float(value), 3)


if __name__ == '__main__':
	unittest.main()
