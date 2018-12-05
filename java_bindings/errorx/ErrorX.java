package errorx;

import java.util.ArrayList; 
import java.io.*;

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

	public native String[] correctSequences( String[] sequence_list, // ArrayList 
					 String[] germline_sequence_list, // ArrayList
					 String[] phred_score_list // ArrayList
					 );

	public native double[] getPredictedErrors( String sequence, // String 
					 String germline_sequence, // String
					 String phred_score // String
					 );

	public native void runProtocol( ErrorXOptions options );

}
