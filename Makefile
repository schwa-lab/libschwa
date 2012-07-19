MAIN_CLASS = org.schwa.dr.App


.PHONY: all clean compile package run test

all: compile

clean:
	mvn clean

compile:
	mvn compile

package:
	mvn package

run:
	mvn exec:java -Dexec.mainClass=$(MAIN_CLASS)

test:
	mvn test
