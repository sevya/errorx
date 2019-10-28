'''
@file ErrorXOptions.py
@brief Controls the options around running the ErrorX protocol
@details 
@author Alex Sevy (alex.sevy@gmail.com)
'''

class ErrorXOptions:
	
	'''
		Constructor. Only two arguments must be provided:
		infile, which is the name of the input file, and
		informat, which is the format of the input file
		(either fastq or tsv)
	'''
	def __init__( self, infile, informat, 
				  outfile='out.tsv', 
				  species='human',
				  nthreads=-1, 
				  verbose=1, 
				  error_threshold=0.556956,
				  allow_nonproductive=0, 
				  correction='N' ):

		self.infile_ = infile
		self.format_ = informat
		self.outfile_ = outfile
		self.species_ = species
		self.verbose_ = verbose
		self.error_threshold_ = error_threshold
		self.allow_nonproductive_ = allow_nonproductive
		self.correction_ = correction
		self.base_path_ = ''
		self.nthreads_ = nthreads

	'''
		Empty constructor with no arguments. When running
		directly from a list and not from a file, the infile
		and informat arguments are not necessary
	'''
	def __init__( self, 
				  infile="tmp",
				  informat="tsv", 
				  outfile='out.tsv', 
				  species='human',
				  nthreads=-1, 
				  verbose=1, 
				  error_threshold=0.556956,
				  allow_nonproductive=0, 
				  correction='N' ):

		self.infile_ = infile
		self.format_ = informat
		self.outfile_ = outfile
		self.species_ = species
		self.verbose_ = verbose
		self.error_threshold_ = error_threshold
		self.allow_nonproductive_ = allow_nonproductive
		self.correction_ = correction
		self.base_path_ = ''
		self.nthreads_ = nthreads

	'''
		Setter functions. The options that can be set by users are:
		
		infile: input file, either FASTQ or TSV, to use for prediction
		informat: format of infile, either fastq or tsv
		outfile: name of the output file to write the final ErrorX predictions. 
		Default out.tsv
		species: species to use for germline assignment. Currently supported
		are human or mouse. Default human
		verbose: should ErrorX output extra warnings and messages? Default no
		error_threshold: what probability is enough to call a base an error?
		Default 0.556956
		allow_nonproductive_: allow sequences through the protocol even if they
		are nonproductive. Default no
		correction: when ErrorX corrects a sequence it replaces the original base
		with a new character. What character should be used? Default N
	'''
	def infile( self, infile ):
		self.infile_ = infile

	def informat( self, informat ):
		self.format_ = informat

	def outfile( self, outfile ):
		self.outfile_ = outfile

	def species( self, species ):
		self.species_ = species

	def nthreads( self, nthreads ):
		self.nthreads_ = nthreads

	def verbose( self, verbose ):
		self.verbose_ = verbose

	def error_threshold( self, thresh ):
		self.error_threshold_ = thresh

	def allow_nonproductive( self, nonprod ):
		self.allow_nonproductive_ = nonprod

	def correction( self, corr ):
		self.correction_ = corr

	def base_path( self, base ):
		self.base_path_ = base

