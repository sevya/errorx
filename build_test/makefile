.DEFAULT_GOAL := TestJava

TestJava: TestErrorX.java 
	"C:/Program Files/Java/jdk1.8.0_45/bin/javac.exe" -cp '../java_bindings/ErrorX.jar' TestErrorX.java
	"C:/Program Files/Java/jdk1.8.0_45/bin/java.exe" -cp '.;../java_bindings/ErrorX.jar' TestErrorX

TestLinking: TestLinking.cc
	clang++ -std=c++11 -I../include/ -L../lib/ -lerrorx TestLinking.cc

