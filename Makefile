MAIN_CLASS = App


.PHONY: all clean compile package run test wc

all: compile

clean:
	mvn clean

compile:
	mvn compile

package:
	mvn package

run: compile
	mvn exec:java -Dexec.mainClass=$(MAIN_CLASS)

test:
	mvn test

wc:
	find src/main/java/org/schwa/dr -name '*.java' | xargs wc -l
