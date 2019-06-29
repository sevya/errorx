import java.util.ArrayList; 
import errorx.ErrorX;

import org.junit.*;
import static org.junit.Assert.*;

import errorx.ErrorXOptions;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import java.io.*; 

public class TestErrorX {

	@BeforeClass
	public static void setUp() {
		try {
			sequence = "TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG";
		
			germline_sequence = "TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG";
		
			phred_score = "###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,";
		
			correctedSequence = "TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG";
		} catch ( Exception exc ) {
			System.out.println(exc.getMessage());
			fail();
		}
	}

	@Test
	public void testRunProtocol() {
		try {
			ErrorXOptions options = new ErrorXOptions( "testing/test.tsv", "tsv" );
			options.outfile( "testing/out.tsv" );
			options.species( "mouse" );
		
			ErrorX ex = new ErrorX();
			ex.runProtocol( options );

			List<String> lines = Files.readAllLines( Paths.get("testing/out.tsv" ));
			String strOne = lines.get(1);
			String strTwo = "SRR3175015.933	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	N/A	False	N/A	N/A	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	N/A	TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	N/A	6	";

			String[] tokensOne = strOne.split("\\s+");
			String[] tokensTwo = strTwo.split("\\s+");

			for ( int ii = 0; ii < tokensOne.length; ++ii ) {
				assertEquals( tokensOne[ii], tokensTwo[ii] );

			}

			options.infile( "testing/test.fastq" );
			options.format( "fastq" );
			options.species( "mouse" );
			ex.runProtocol( options );

			lines = Files.readAllLines( Paths.get("testing/out.tsv" ));

			strOne = lines.get(1);
			strTwo = "SRR3175015.933	IGHV1-76*01	73.28	1.12E-18	N/A	N/A	N/A	IGHJ2*01	94.87	2.38E-14	-	VH	True	GCGGGAGAGGAGGCTTTGTCCTTCGTTTACTAC	AGEEALSFVYY	TACTCCCGTGGTACGCCCAAGGACGGAGGCACACGGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGCGCGCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTCCGGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	TACTACAATGAGAAGTTCAAGGGCAAGGCCACACTGACTGCAGAAAAATCCTCCAGCACTGCCTACATGCAGCTCAGCAGCCTGACATCTGAGGACTCTGCTGTCTATTTCTGTGC--------------------------ACTACTGGGGCCAAGGCACCACTCTCACAGTCTCCTCAG	###########################################################################################################################################C:=9@7+C6++8,E>7,8>@,7B>8,++C@64+8>88@,@4,	YSRGTPKDGGTRSADKSSSAACLARSSLKAGDSAVCSGAGEEALSFVYYWGQGTTLTGSS	TACTCCNGTGGTACGCCCAAGGACGGAGGCACACNGAGTGCAGACAAGTCCTCCAGCGCGGCCTGCCTGGNGCNCAGCAGCCTGAAAGCTGGAGACTCTGCTGTCTGTTNCNGTGCGGGAGAGGAGGCTTTGTCCTTCGTTTACTACTGGGGCCAAGGCACCACTCTCACGGGCTCCTCAG	YSXGTPKDGGTXSADKSSSAACLXXSSLKAGDSAVCXXAGEEALSFVYYWGQGTTLTGSS	6	";

			tokensOne = strOne.split("\\s+");
			tokensTwo = strTwo.split("\\s+");

			for ( int ii = 0; ii < tokensOne.length; ++ii ) {
				assertEquals( tokensOne[ii], tokensTwo[ii] );

			}
		} catch ( Exception exc ) {
			System.out.println(exc.getMessage());
			fail();
		}
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
		try {
			double[] errorPredictions = new ErrorX().getPredictedErrors( 
				sequence, 
				germline_sequence, 
				phred_score );
			File file = new File("validation.txt"); 
  
			BufferedReader br = new BufferedReader(new FileReader(file)); 
	
			String st; 
			while ((st = br.readLine()) != null) {
				String[] tokens = st.split("\t");
				Integer position = Integer.valueOf(tokens[0]);
				Double value = Double.valueOf(tokens[1]);
				assertEquals( errorPredictions[position], value, 0.001 );
	  		} 
		} catch ( Exception exc ) {
			System.out.println(exc.getMessage());
			fail();
		}
	}

	static String sequence;
	static String germline_sequence;
	static String phred_score;
	static String correctedSequence;
}
