/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestSequenceFeatures_init = false;
#include "/Volumes/MyPassport/ErrorX/unit_test/TestSequenceFeatures.hh"

static TestSequenceFeatures suite_TestSequenceFeatures;

static CxxTest::List Tests_TestSequenceFeatures = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSequenceFeatures( "TestSequenceFeatures.hh", 25, "TestSequenceFeatures", suite_TestSequenceFeatures, Tests_TestSequenceFeatures );

static class TestDescription_suite_TestSequenceFeatures_testCharEncoding : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testCharEncoding() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 28, "testCharEncoding" ) {}
 void runTest() { suite_TestSequenceFeatures.testCharEncoding(); }
} testDescription_suite_TestSequenceFeatures_testCharEncoding;

static class TestDescription_suite_TestSequenceFeatures_testSequenceEncoding : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testSequenceEncoding() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 60, "testSequenceEncoding" ) {}
 void runTest() { suite_TestSequenceFeatures.testSequenceEncoding(); }
} testDescription_suite_TestSequenceFeatures_testSequenceEncoding;

static class TestDescription_suite_TestSequenceFeatures_testSequenceWindow : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testSequenceWindow() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 83, "testSequenceWindow" ) {}
 void runTest() { suite_TestSequenceFeatures.testSequenceWindow(); }
} testDescription_suite_TestSequenceFeatures_testSequenceWindow;

static class TestDescription_suite_TestSequenceFeatures_testIntWindow : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testIntWindow() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 130, "testIntWindow" ) {}
 void runTest() { suite_TestSequenceFeatures.testIntWindow(); }
} testDescription_suite_TestSequenceFeatures_testIntWindow;

static class TestDescription_suite_TestSequenceFeatures_testFeatureVector : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testFeatureVector() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 179, "testFeatureVector" ) {}
 void runTest() { suite_TestSequenceFeatures.testFeatureVector(); }
} testDescription_suite_TestSequenceFeatures_testFeatureVector;

static class TestDescription_suite_TestSequenceFeatures_testFull : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testFull() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 192, "testFull" ) {}
 void runTest() { suite_TestSequenceFeatures.testFull(); }
} testDescription_suite_TestSequenceFeatures_testFull;

static class TestDescription_suite_TestSequenceFeatures_testErrorRate_singlethread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testErrorRate_singlethread() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 271, "testErrorRate_singlethread" ) {}
 void runTest() { suite_TestSequenceFeatures.testErrorRate_singlethread(); }
} testDescription_suite_TestSequenceFeatures_testErrorRate_singlethread;

static class TestDescription_suite_TestSequenceFeatures_testErrorRate_multithread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testErrorRate_multithread() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 312, "testErrorRate_multithread" ) {}
 void runTest() { suite_TestSequenceFeatures.testErrorRate_multithread(); }
} testDescription_suite_TestSequenceFeatures_testErrorRate_multithread;

static class TestDescription_suite_TestSequenceFeatures_testTSVInput_singlethread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testTSVInput_singlethread() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 353, "testTSVInput_singlethread" ) {}
 void runTest() { suite_TestSequenceFeatures.testTSVInput_singlethread(); }
} testDescription_suite_TestSequenceFeatures_testTSVInput_singlethread;

static class TestDescription_suite_TestSequenceFeatures_testTSVInput_multithread : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSequenceFeatures_testTSVInput_multithread() : CxxTest::RealTestDescription( Tests_TestSequenceFeatures, suiteDescription_TestSequenceFeatures, 367, "testTSVInput_multithread" ) {}
 void runTest() { suite_TestSequenceFeatures.testTSVInput_multithread(); }
} testDescription_suite_TestSequenceFeatures_testTSVInput_multithread;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
