.DEFAULT_GOAL := TestJava

TestJava: TestErrorX.java 
	javac -cp '.:../java_bindings/ErrorX.jar' TestErrorX.java
	java -cp .:../java_bindings/ErrorX.jar TestErrorX
