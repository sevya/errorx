package errorx;

import java.net.URLDecoder;
import java.io.UnsupportedEncodingException;
import java.io.File;

public class ErrorXOptions {	

	public ErrorXOptions( String infile, String format ) {
		infile_ = infile;
		format_ = format;

		outfile_ = "out.tsv";
		species_ = "human";
		base_path_ = get_base();

		verbose_ = 1;
		allow_nonproductive_ = false;
		error_threshold_ = 0.884311;
		correction_ = 'N';
		nthreads_ = -1;
	}

	public ErrorXOptions() {
		infile_ = "tmp";
		format_ = "tsv";

		outfile_ = "out.tsv";
		species_ = "human";
		base_path_ = get_base();

		verbose_ = 1;
		allow_nonproductive_ = false;
		error_threshold_ = 0.884311;
		correction_ = 'N';
		nthreads_ = -1;
	}

	private String get_base() {
		try { 
			// return URLDecoder.decode(ClassLoader.getSystemClassLoader().getResource(".").getPath(), "UTF-8");
			String path = ErrorXOptions.class.getProtectionDomain().getCodeSource().getLocation().getPath();
			String decodedPath = URLDecoder.decode(path, "UTF-8");
			return new File(decodedPath).getParentFile().getPath();

		} catch ( UnsupportedEncodingException exc ) {
			exc.printStackTrace();
			return "";
		}
	}

	public void infile( String infile ) { infile_ = infile; }
	public void format( String format ) { format_ = format; }
	public void outfile( String outfile ) { outfile_ = outfile; }
	public void species( String species ) { species_ = species; }
	public void base_path( String base_path ) { base_path_ = base_path; }

	public void verbose( int verbose ) { verbose_ = verbose; }
	public void allow_nonproductive( boolean allow_nonproductive )
		{ allow_nonproductive_ = allow_nonproductive; }

	public void error_threshold( double error_threshold ) 
		{ error_threshold_ = error_threshold; }
	public void correction( char correction ) { correction_ = correction; }
	public void nthreads( int nthreads ) { nthreads_ = nthreads; }


	private String infile_;
	private String format_;
	private String outfile_;
	private String species_;
	private String base_path_;

	private int verbose_;
	private boolean allow_nonproductive_;

	private double error_threshold_;
	private char correction_;
	private int nthreads_;

}
