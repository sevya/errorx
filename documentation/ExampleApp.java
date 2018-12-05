import errorx.ErrorX;
import errorx.ErrorXOptions;

public static void main( String[] args ) { 
	String[] sequences = new String[1];
	sequences[0] = "AGGGGCCACAGTCAAGTTGTCCTGCACAGCT";

	String[] germline_sequences = new String[1];
	germline_sequences[0] = "AGGGGCCACAGTCATGTTGTCCTGTACAGCT";

	String[] phred_scores = new String[1];
	phred_scores[0] = "GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG";

	String[] correctedSequences = new ErrorX().correctSequences( sequences, germline_sequences, phred_scores );


	String sequence = "AGGGGCCACAGTCAAGTTGTCCTGCACAGCT";

	String germline_sequence = "AGGGGCCACAGTCATGTTGTCCTGTACAGCT";

	String phred_score = "GGGGGGFGGGGGGGGGGGGGGGGGGGGGGGG";

	double[] errorPredictions = new ErrorX().getPredictedErrors( 
			sequence, 
			germline_sequence, 
			phred_score );



	ErrorXOptions options = new ErrorXOptions( "test.tsv", "tsv" );
	options.outfile( "out.tsv" );

	ErrorX ex = new ErrorX();
	ex.runProtocol( options );
}