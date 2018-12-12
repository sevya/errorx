.DEFAULT_GOAL := TestJava

TestJava: TestErrorX.java 
	javac -cp '.:../java_bindings/ErrorX.jar' TestErrorX.java
	java -cp .:../java_bindings/ErrorX.jar TestErrorX

TestLinking: TestLinking.cc
	clang++ -std=c++11 -I../include/ -L../lib/ -lerrorx TestLinking.cc

