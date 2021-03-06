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
			sequence = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";
		
			germline_sequence = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";
		
			phred_score = ";=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG";
		
			correctedSequence = "CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCNNTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG";
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
			String strTwo = "SRR3175015:::6:0:0:0:   N/A      N/A   N/A       N/A     N/A     N/A     N/A        N/A   N/A        N/A       N/A      False    N/A        N/A        N/A        N/A        N/A     N/A       CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   ;=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG   N/A    CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCNNTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   N/A    2";

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
			strTwo = "SRR3175015:::6:0:0:0:   IGHV9-4*02      97.96   1.08E-125       N/A     N/A     N/A     IGHJ4*01        97.87   8.66E-21        +       VH      True    GGGTATACCTTCACAACTGCTGGA        GYTFTTAG        ATAAACACCCACTCTGGAGTGCCA        INTHSGVP        GCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCC     ARGGAAFYRNDGGAMDS       CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCGTTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGCCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAAGCAACCTCAAAAATGAGGACACGGCTACGTATTTCTGTGCGAGA--------------------------------TGCTATGGACTACTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   ;=,,=;EE,<C,,8,CC,;;C-CEFGGGGGGGDFGGGGGGGGGGEGFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG@FGGGGGGGGFFFFGDFFGGFCEGGGGGGGGGGGGGEEGGGGDFGGGEEFCFCFGGCFFFGGGGGGGGGGGGGGF66DGGGGGCFGGGDG5DGFGFDCDBF9BA8@FFFFFFDAFGFF@?B@33>8;@B4C?CCFFEECE27;;@;@@E333:@CFFF6;DF>(4:1<A#######@@@6C:A;4)7/)CEEFGFFCFGGC7?:9ECDEDGD6GGCFA,DDFF=8EGECC8FF=9,GGGFDGE@E;GGGGGGGGGGGFGGGG   QIQLVQSGPELKKPGETVRISCKASGYTFTTAGMQWVQKMPGKGLKWIGWINTHSGVPKYAEDFKGRFVFSLETSASTAYLQITNLKNEDTATYFVARGGAAFYRNDGGAMDSWGQGTSVTVSS    CAGATCCAGTTGGTGCAGTCTGGACCTGAGCTGAAGAAGCCTGGAGAGACAGTCAGGATCTCCTGCAAGGCTTCTGGGTATACCTTCACAACTGCTGGAATGCAGTGGGTGCAAAAGATGCCAGGAAAGGGTTTGAAGTGGATTGGCTGGATAAACACCCACTCTGGAGTGCCAAAATATGCAGAAGACTTCAAGGGACGGTTTGTCTTCTCTTTGGAAACCTCTGCCAGCACTGCATATTTACAGATAACGAACCTCAAAAATGAGGACACGGCTACATATTTCNNTGCGAGAGGAGGGGCCGCCTTCTATAGAAACGACGGGGGTGCTATGGACTCCTGGGGTCAAGGAACCTCAGTCACCGTCTCCTCAG   QIQLVQSGPELKKPGETVRISCKASGYTFTTAGMQWVQKMPGKGLKWIGWINTHSGVPKYAEDFKGRFVFSLETSASTAYLQITNLKNEDTATYFXARGGAAFYRNDGGAMDSWGQGTSVTVSS    2";

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
