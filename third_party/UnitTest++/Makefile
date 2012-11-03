include ../../Makefile.$(shell uname -s | tr '[:upper:]' '[:lower:]')

INCLUDE = -I.
LIB_DIR = ../../libs

OBJECTS =\
		AssertException.o \
		Checks.o \
		CurrentTest.o \
		DeferredTestReporter.o \
		DeferredTestResult.o \
		MemoryOutStream.o \
		ReportAssert.o \
		Test.o \
		TestDetails.o \
		TestList.o \
		TestReporter.o \
		TestReporterStdout.o \
		TestResults.o \
		TestRunner.o \
		TimeConstraint.o \
		XmlTestReporter.o \
		Posix/SignalTranslator.o \
		Posix/TimeHelpers.o

LIBS = \
		$(LIB_DIR)/libUnitTest++.a \
		$(LIB_DIR)/libUnitTest++.$(SHARED_LIB_SUFFIX)

.PHONY: all clean

all: $(LIBS)

clean:
	-find . -name "*.o" -exec rm -f {} \;
	-rm -f $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB_DIR)/libUnitTest++.a: $(OBJECTS)
	ar rcs $@ $^

$(LIB_DIR)/libUnitTest++.$(SHARED_LIB_SUFFIX): $(OBJECTS)
	$(CXX) $(SHARED_LIB_LDFLAGS) $^ -o $@ $(LDFLAGS)
