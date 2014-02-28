include       ../../Makefile.$(shell uname -s | tr '[:upper:]' '[:lower:]')
TARGET_DIR = ../../libs

CPPFLAGS += -I. $(PLATFORM_CPPFLAGS)
CXXFLAGS += -std=c++11 -fPIC $(PLATFORM_CXXFLAGS)
LDFLAGS += $(PLATFORM_LDFLAGS)

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
		$(TARGET_DIR)/libUnitTest++.a \
		$(TARGET_DIR)/libUnitTest++.$(PLATFORM_SHARED_LIB_SUFFIX)

.PHONY: all clean

all: $(LIBS)

clean:
	-find . -name "*.o" -exec rm -f {} \;
	-rm -f $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_DIR)/libUnitTest++.a: $(OBJECTS)
	ar rcs $@ $^

$(TARGET_DIR)/libUnitTest++.$(PLATFORM_SHARED_LIB_SUFFIX): $(OBJECTS)
	$(CXX) $(PLATFORM_SHARED_LIB_LDFLAGS) $^ -o $@ $(LDFLAGS)
