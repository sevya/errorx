import errorx as ex

def test_run_protocol():
	options = ex.ErrorXOptions('input_files/test.tsv','tsv')
	options.outfile( 'new/python_test_tsv.tsv' )
	ex.run_protocol( options )

	options = ex.ErrorXOptions('input_files/test.fastq','fastq')
	options.outfile( 'new/python_test_fastq.tsv' )
	ex.run_protocol( options )


if __name__ == '__main__':
	test_run_protocol()
