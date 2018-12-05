import java.util.ArrayList; 
import errorx.ErrorX;

import errorx.ErrorXOptions;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import java.io.IOException;

public class TestErrorX {

	public static void main( String[] args ) {
		ErrorXOptions options = new ErrorXOptions( "input_files/test.tsv", "tsv" );
		options.outfile( "new/java_test_tsv.tsv" );
		
		ErrorX ex = new ErrorX();
		ex.runProtocol( options );

		options.infile( "input_files/test.fastq" );
		options.format( "fastq" );
		options.outfile( "new/java_test_fastq.tsv" );
		ex.runProtocol( options );
	}


}
