# Installation

To install ErrorX, you need to request a download link from your Endeavor Bio representative. They will give you a link to the appropriate download based on your operating system. Once you have downloaded the zipped archive (this will have the extension `.tar.gz`), you are ready to unpack the folder and starting running.

Run the following command to extract the files from the archive:

	tar xvfz ErrorX-1.0_mac.tar.gz
	
You should replace `mac` in the command with whatever operating system you are running (`win`, `mac`, or `linux`). 

The ErrorX binary is ready to go from there - it is located at `Errorx-1.0/bin/errorx`

## Python installation

The Python packages are also located in the folder you just extracted. ErrorX supports Python versions 2.7 and 3.6. To install the Python 2.7 package simply run

	pip install python2_bindings/
	
and for version 3.6 run

	pip3 install python3_bindings
	
You are now ready to run ErrorX through the Python interface!

## Java installation

The ErrorX Java library is also located in the gzipped archive, in the folder `java_bindings`. To install you don't need to do anything special - all the functionality is contained in the `ErrorX.jar` archive within the `java_bindings` folder. When you are compiling your Java application simply add the flag 

`-cp 'ErrorX-1.0/java_bindings/ErrorX.jar'` 

to make sure the JAR is linked, and you are all set! 

**Note:** the JAR relies on data files contained in the `java_bindings` folder. For best results, leave the directory structure of `java_bindings` intact and only link the JAR. Alternatively, if you have to move the JAR file, make sure to move all the other directories in `java_bindings` along with it.
