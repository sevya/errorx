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
bool suite_TestIGBlastParser_init = false;
#include "/Volumes/MyPassport/ErrorX/unit_test/TestIGBlastParser.hh"

static TestIGBlastParser suite_TestIGBlastParser;

static CxxTest::List Tests_TestIGBlastParser = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestIGBlastParser( "TestIGBlastParser.hh", 25, "TestIGBlastParser", suite_TestIGBlastParser, Tests_TestIGBlastParser );

static class TestDescription_suite_TestIGBlastParser_testFastq : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testFastq() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 30, "testFastq" ) {}
 void runTest() { suite_TestIGBlastParser.testFastq(); }
} testDescription_suite_TestIGBlastParser_testFastq;

static class TestDescription_suite_TestIGBlastParser_testParser : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testParser() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 79, "testParser" ) {}
 void runTest() { suite_TestIGBlastParser.testParser(); }
} testDescription_suite_TestIGBlastParser_testParser;

static class TestDescription_suite_TestIGBlastParser_testRecords : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testRecords() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 135, "testRecords" ) {}
 void runTest() { suite_TestIGBlastParser.testRecords(); }
} testDescription_suite_TestIGBlastParser_testRecords;

static class TestDescription_suite_TestIGBlastParser_testRecordsNoD : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testRecordsNoD() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 189, "testRecordsNoD" ) {}
 void runTest() { suite_TestIGBlastParser.testRecordsNoD(); }
} testDescription_suite_TestIGBlastParser_testRecordsNoD;

static class TestDescription_suite_TestIGBlastParser_testRecordsNoJ : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testRecordsNoJ() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 243, "testRecordsNoJ" ) {}
 void runTest() { suite_TestIGBlastParser.testRecordsNoJ(); }
} testDescription_suite_TestIGBlastParser_testRecordsNoJ;

static class TestDescription_suite_TestIGBlastParser_testRecordsNoDNoJ : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testRecordsNoDNoJ() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 298, "testRecordsNoDNoJ" ) {}
 void runTest() { suite_TestIGBlastParser.testRecordsNoDNoJ(); }
} testDescription_suite_TestIGBlastParser_testRecordsNoDNoJ;

static class TestDescription_suite_TestIGBlastParser_testLC : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testLC() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 352, "testLC" ) {}
 void runTest() { suite_TestIGBlastParser.testLC(); }
} testDescription_suite_TestIGBlastParser_testLC;

static class TestDescription_suite_TestIGBlastParser_testMouse : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestIGBlastParser_testMouse() : CxxTest::RealTestDescription( Tests_TestIGBlastParser, suiteDescription_TestIGBlastParser, 402, "testMouse" ) {}
 void runTest() { suite_TestIGBlastParser.testMouse(); }
} testDescription_suite_TestIGBlastParser_testMouse;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
