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
bool suite_TestErrorX_init = false;
#include "/Users/alexsevy/ErrorX/unit_test/TestErrorX.hh"

static TestErrorX suite_TestErrorX;

static CxxTest::List Tests_TestErrorX = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestErrorX( "TestErrorX.hh", 20, "TestErrorX", suite_TestErrorX, Tests_TestErrorX );

static class TestDescription_suite_TestErrorX_testFromOptions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorX_testFromOptions() : CxxTest::RealTestDescription( Tests_TestErrorX, suiteDescription_TestErrorX, 24, "testFromOptions" ) {}
 void runTest() { suite_TestErrorX.testFromOptions(); }
} testDescription_suite_TestErrorX_testFromOptions;

static class TestDescription_suite_TestErrorX_testFromOptionsTSV : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorX_testFromOptionsTSV() : CxxTest::RealTestDescription( Tests_TestErrorX, suiteDescription_TestErrorX, 75, "testFromOptionsTSV" ) {}
 void runTest() { suite_TestErrorX.testFromOptionsTSV(); }
} testDescription_suite_TestErrorX_testFromOptionsTSV;

static class TestDescription_suite_TestErrorX_testFromVectors : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorX_testFromVectors() : CxxTest::RealTestDescription( Tests_TestErrorX, suiteDescription_TestErrorX, 110, "testFromVectors" ) {}
 void runTest() { suite_TestErrorX.testFromVectors(); }
} testDescription_suite_TestErrorX_testFromVectors;

static class TestDescription_suite_TestErrorX_testFromVectorsOptions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestErrorX_testFromVectorsOptions() : CxxTest::RealTestDescription( Tests_TestErrorX, suiteDescription_TestErrorX, 158, "testFromVectorsOptions" ) {}
 void runTest() { suite_TestErrorX.testFromVectorsOptions(); }
} testDescription_suite_TestErrorX_testFromVectorsOptions;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
