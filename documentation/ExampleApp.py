import errorx as ex

sequences = ['AGGGGCCACAGTCAAGTTGTCCTGCACAGCT']

germline_sequences = ['AGGGGCCACAGTCATGTTGTCCTGTACAGCT']

phred_scores = ['GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG']

results = ex.correct_sequences(sequences, 
		   germline_sequences,
		   phred_scores )

print results[0]

sequence = 'AGGGGCCACAGTCAAGTTGTCCTGCACAGCT'

germline_sequence = 'AGGGGCCACAGTCATGTTGTCCTGTACAGCT'

phred_score = 'GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG'

result = ex.get_predicted_errors(sequence, 
		   germline_sequence,
		   phred_score )

print result

options = ex.ErrorXOptions('example.fastq','fastq')
options.outfile( 'out.tsv' )
ex.run_protocol( options )