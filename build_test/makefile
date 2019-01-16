.DEFAULT_GOAL := TestJava

ifeq ($(OS),Windows_NT)
	JAVAC="C:/Program Files/Java/jdk1.8.0_45/bin/javac.exe"
	JAVA="C:/Program Files/Java/jdk1.8.0_45/bin/java.exe"
	CP='.;../java_bindings/ErrorX.jar'
else
	JAVAC=javac
	JAVA=java
	CP='.:../java_bindings/ErrorX.jar'
endif

TestJava: TestErrorX.java 
	$(JAVAC) -cp $(CP) TestErrorX.java
	$(JAVA) -cp $(CP) TestErrorX

TestLinking: TestLinking.cc
	clang++ -std=c++11 -I../include/ -L../lib/ -lerrorx TestLinking.cc

