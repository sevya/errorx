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
bool suite_TestErrorXTrial_init = false;
#include "/Volumes/MyPassport/ErrorX/unit_test/TestErrorXTrial.hh"

static TestErrorXTrial suite_TestErrorXTrial;

static CxxTest::List Tests_TestErrorXTrial = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestErrorXTrial( "TestErrorXTrial.hh", 20, "TestErrorXTrial", suite_TestErrorXTrial, Tests_TestErrorXTrial );

static class TestDescription_suite_TestErrorXTrial_testFromOptions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorXTrial_testFromOptions() : CxxTest::RealTestDescription( Tests_TestErrorXTrial, suiteDescription_TestErrorXTrial, 24, "testFromOptions" ) {}
 void runTest() { suite_TestErrorXTrial.testFromOptions(); }
} testDescription_suite_TestErrorXTrial_testFromOptions;

static class TestDescription_suite_TestErrorXTrial_testFromOptionsTSV : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorXTrial_testFromOptionsTSV() : CxxTest::RealTestDescription( Tests_TestErrorXTrial, suiteDescription_TestErrorXTrial, 76, "testFromOptionsTSV" ) {}
 void runTest() { suite_TestErrorXTrial.testFromOptionsTSV(); }
} testDescription_suite_TestErrorXTrial_testFromOptionsTSV;

static class TestDescription_suite_TestErrorXTrial_testFromVectors : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorXTrial_testFromVectors() : CxxTest::RealTestDescription( Tests_TestErrorXTrial, suiteDescription_TestErrorXTrial, 112, "testFromVectors" ) {}
 void runTest() { suite_TestErrorXTrial.testFromVectors(); }
} testDescription_suite_TestErrorXTrial_testFromVectors;

static class TestDescription_suite_TestErrorXTrial_testFromVectorsOptions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorXTrial_testFromVectorsOptions() : CxxTest::RealTestDescription( Tests_TestErrorXTrial, suiteDescription_TestErrorXTrial, 158, "testFromVectorsOptions" ) {}
 void runTest() { suite_TestErrorXTrial.testFromVectorsOptions(); }
} testDescription_suite_TestErrorXTrial_testFromVectorsOptions;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
