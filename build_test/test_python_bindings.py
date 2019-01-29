import errorx as ex
from errorx import ErrorXOptions

def run_FASTQ_protocol():
	options = ex.ErrorXOptions('input_files/test.fastq','fastq')
	options.outfile( 'new/python_fastq_out.tsv' )
	options.species( 'mouse' )
	options.verbose( 0 )
	ex.run_protocol( options )
		
def run_TSV_protocol():
	options = ex.ErrorXOptions('input_files/test.tsv','tsv')
	options.outfile( 'new/python_tsv_out.tsv' )
	options.species( 'mouse' )
	options.verbose( 0 )
	
	ex.run_protocol( options )


if __name__ == '__main__':
	run_FASTQ_protocol()
	run_TSV_protocol()
