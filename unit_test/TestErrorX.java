import java.util.ArrayList; 
import errorx.ErrorX;

import org.junit.*;
import static org.junit.Assert.assertEquals;

import errorx.ErrorXOptions;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import java.io.IOException;

public class TestErrorX {

	@BeforeClass
	public static void setUp() throws Exception {
		sequence = "TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG";
		
		germline_sequence = "TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG";
		
		phred_score = "###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,";
		
		correctedSequence = "TACTCCCGTGGTACGCCCAAGGACNGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG";
    }

	@Test
	public void testRunProtocol() throws IOException {
		ErrorXOptions options = new ErrorXOptions( "testing/test.tsv", "tsv" );
		options.outfile( "testing/out.tsv" );
		options.species( "mouse" );
		
		ErrorX ex = new ErrorX();
		ex.runProtocol( options );

		List<String> lines = Files.readAllLines( Paths.get("testing/out.tsv" ));
		assertEquals( 
			lines.get(1), 
			"test		N/A	N/A		N/A	N/A		N/A	N/A		VH	False	N/A	N/A	AGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCA	AGGGCAGAGTCACGATTACCGCGGACAAATCCACGAGCACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAGGACACGGCCGTGTATTACTGTGCGAGAGA------------------------CTGGGGCCAAGGGACCACGGTCACCGTCTCCTCA	########################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4	N/A	AGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGNAGACTCTGCTGTCTGTTCCNGTGCGNGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTNTCACGGGCTCCTCA	N/A	6	"
			);

		options.infile( "testing/test.fastq" );
		options.format( "fastq" );
		options.species( "mouse" );
		ex.runProtocol( options );

		lines = Files.readAllLines( Paths.get("testing/out.tsv" ));
		assertEquals( 
			lines.get(1), 
			"SRR3175015.933	IGHV1-76*01	73.276000	1.12E-18	N/A	N/A	N/A	IGHJ2*01	94.872000	2.38E-14	-	VH	True	GCGGGAGAGGAGGCTTTGTCCTTCGTTTACTAC	AGEEALSFVYY	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS	TACTCCCGTGGTACGCCCAAGGACNGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	YSRGTPKDGTSADKSSSAACLSSLKAGDSAVCAGEEALSFVYYWGQGTTLTGSS	6	" 
			);

	}

	@Test
	public void testNumbers() {

		int n = 502;
		String[] sequences = new String[n];
		String[] germline_sequences = new String[n];
		String[] phred_scores = new String[n];

		for ( int ii = 0; ii < n; ++ii ) {
			sequences[ii] = sequence;
			germline_sequences[ii] = germline_sequence;
			phred_scores[ii] = phred_score;
		}
				


		String[] correctedSequences = new ErrorX().correctSequences( sequences, germline_sequences, phred_scores );

		assertEquals( correctedSequences.length, 502 );

		assertEquals( correctedSequences[0], correctedSequence );

	}

	@Test
	public void testCorrectSequence() {
		String[] sequences = new String[1];
		sequences[0] = sequence;

		String[] germline_sequences = new String[1];
		germline_sequences[0] = germline_sequence;

		String[] phred_scores = new String[1];
		phred_scores[0] = phred_score;

		String[] correctedSequences = new ErrorX().correctSequences( sequences, germline_sequences, phred_scores );

		assertEquals( correctedSequences.length, 1 );

		assertEquals( correctedSequences[0], correctedSequence );

	}


	@Test
	public void testPredictedError() {

		double[] errorPredictions = new ErrorX().getPredictedErrors( 
			sequence, 
			germline_sequence, 
			phred_score );

		assertEquals( errorPredictions[132], 0.486122388, 0.000000001 );
		assertEquals( errorPredictions[133], 0.348359625, 0.000000001 );
		assertEquals( errorPredictions[134], 0.143818731, 0.000000001 );
		assertEquals( errorPredictions[135], 0.458915133, 0.000000001 );
		assertEquals( errorPredictions[136], 0.306816917, 0.000000001 );
		assertEquals( errorPredictions[137], 0.022584986, 0.000000001 );
		assertEquals( errorPredictions[138], 0.513773250, 0.000000001 );

	}

	static String sequence;
	static String germline_sequence;
	static String phred_score;
	static String correctedSequence;
}
