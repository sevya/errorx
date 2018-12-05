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
bool suite_TestUtil_init = false;
#include "/Volumes/MyPassport/ErrorX/unit_test/TestUtil.hh"

static TestUtil suite_TestUtil;

static CxxTest::List Tests_TestUtil = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestUtil( "TestUtil.hh", 22, "TestUtil", suite_TestUtil, Tests_TestUtil );

static class TestDescription_suite_TestUtil_testTranslation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testTranslation() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 26, "testTranslation" ) {}
 void runTest() { suite_TestUtil.testTranslation(); }
} testDescription_suite_TestUtil_testTranslation;

static class TestDescription_suite_TestUtil_testEncrypt : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testEncrypt() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 43, "testEncrypt" ) {}
 void runTest() { suite_TestUtil.testEncrypt(); }
} testDescription_suite_TestUtil_testEncrypt;

static class TestDescription_suite_TestUtil_testTokenize : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testTokenize() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 64, "testTokenize" ) {}
 void runTest() { suite_TestUtil.testTokenize(); }
} testDescription_suite_TestUtil_testTokenize;

static class TestDescription_suite_TestUtil_testReverse : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testReverse() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 81, "testReverse" ) {}
 void runTest() { suite_TestUtil.testReverse(); }
} testDescription_suite_TestUtil_testReverse;

static class TestDescription_suite_TestUtil_testCalculateMetrics : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testCalculateMetrics() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 86, "testCalculateMetrics" ) {}
 void runTest() { suite_TestUtil.testCalculateMetrics(); }
} testDescription_suite_TestUtil_testCalculateMetrics;

static class TestDescription_suite_TestUtil_testAvg : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testAvg() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 97, "testAvg" ) {}
 void runTest() { suite_TestUtil.testAvg(); }
} testDescription_suite_TestUtil_testAvg;

static class TestDescription_suite_TestUtil_testSplitVector : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testSplitVector() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 105, "testSplitVector" ) {}
 void runTest() { suite_TestUtil.testSplitVector(); }
} testDescription_suite_TestUtil_testSplitVector;

static class TestDescription_suite_TestUtil_testCountLines : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestUtil_testCountLines() : CxxTest::RealTestDescription( Tests_TestUtil, suiteDescription_TestUtil, 122, "testCountLines" ) {}
 void runTest() { suite_TestUtil.testCountLines(); }
} testDescription_suite_TestUtil_testCountLines;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
