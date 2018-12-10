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
		options.outfile( "new/java_tsv_out.tsv" );
		options.species( "mouse" );
		options.verbose(0);
		ErrorX ex = new ErrorX();
		ex.runProtocol( options );

		options.infile( "input_files/test.fastq" );
		options.format( "fastq" );
		options.species( "mouse" );
		options.outfile( "new/java_fastq_out.tsv" );
		options.verbose(0);
		ex.runProtocol( options );
	}


}
