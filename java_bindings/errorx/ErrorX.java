package errorx;

import java.util.ArrayList; 
import java.io.*;

/**
	ErrorX bindings for Java. This file provides an interface
	for using the ErrorX dynamically linked library in a Java
	package
*/
public class ErrorX {

	static {
		try {
			System.loadLibrary("errorx_java"); // used for tests. This library in classpath only
		} catch (UnsatisfiedLinkError e) {
			try {
				NativeUtils.loadLibraryFromJar("/errorx/liberrorx_java.dylib"); // during runtime. .DLL within .JAR
			} catch (IOException e1) {
				throw new RuntimeException(e1);
			}
		}
	}	

	/**
		Error correct the provided sequences. All parameters
		are matched up elementwise, so they must have the same
		length.

		@param sequences String array of NT sequences to correct
		@param germline_sequences String array of inferred germline sequences
		corresponding to the entries in `sequences`
		@param phred_scores String array of PHRED quality scores straight
		from the sequencer, corresponding to the entries in `sequences`
		@param options ErrorXOptions object to control options. If 
		None, will set up default values internally

		@return String array of corrected NT sequences corresponding
		to the input param `sequences`
	*/
	public native String[] correctSequences( String[] sequence_list,
					 String[] germline_sequence_list,
					 String[] phred_score_list,
					 ErrorXOptions options
					 );

	/**
		Get the predicted likelihood that each base along a NT sequence
		is an error. 

		@param sequence A single NT sequence to correct
		@param germline_sequences Inferred germline sequence
		@param phred_scores PHRED quality scores
		@param options ErrorXOptions object to control options. If 
		None, will set up default values internally

		@return Array of doubles representing the probability of error for 
		each base. The probability of an error at position i is contained in
		index i of the array.
	*/
	public native double[] getPredictedErrors( String sequence,
					 String germline_sequence,
					 String phred_score,
					 ErrorXOptions options
					 );

	/**
		Wrapper for the native correctSequences method, using default options

		@param sequences String array of NT sequences to correct
		@param germline_sequences String array of inferred germline sequences
		corresponding to the entries in `sequences`
		@param phred_scores String array of PHRED quality scores straight
		from the sequencer, corresponding to the entries in `sequences`
		@param options ErrorXOptions object to control options. If 
		None, will set up default values internally

		@return String array of corrected NT sequences corresponding
		to the input param `sequences`
	*/
	public String[] correctSequences( String[] sequence_list,
					 String[] germline_sequence_list,
					 String[] phred_score_list ) {

		ErrorXOptions options = new ErrorXOptions();
		return correctSequences( sequence_list, germline_sequence_list, phred_score_list, options );
	}

	
	/**
		Wrapper for the native getPredictedErrors method, using default options

		@param sequences String array of NT sequences to correct
		@param germline_sequences String array of inferred germline sequences
		corresponding to the entries in `sequences`
		@param phred_scores String array of PHRED quality scores straight
		from the sequencer, corresponding to the entries in `sequences`
		@param options ErrorXOptions object to control options. If 
		None, will set up default values internally

		@return String array of corrected NT sequences corresponding
		to the input param `sequences`
	*/
	public double[] getPredictedErrors( String sequence,
					 String germline_sequence,
					 String phred_score ) {

		ErrorXOptions options = new ErrorXOptions();
		return getPredictedErrors( sequence, germline_sequence, phred_score, options );
	}

	/**
		Run the ErrorX protocol and write the results to a file

		@param options ErrorXOptions object with the processing details
	*/
	public native void runProtocol( ErrorXOptions options );

}
