import errorx as ex
import pandas as pd

df = pd.read_table('~/sevy_2017/error_rate_estimation/corrected_seqs.tsv')
for uid,subdf in df.groupby('RID'):
	print list(subdf)
	exit()
	results = ex.correct_sequences(subdf['Uncorrected'], germline_sequences, phred_scores )