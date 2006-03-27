/* \file testDaqSource.cc
 *
 *  $Date: 2005/11/23 13:18:15 $
 *  $Revision: 1.4 $
 *  \author S. Argiro, N. Amapane - CERN
 */

#include <cppunit/extensions/HelperMacros.h>
#include <FWCore/Framework/interface/EventProcessor.h>
#include <FWCore/Utilities/interface/ProblemTracker.h>
#include <FWCore/Utilities/interface/Exception.h>
#include <iostream>
#include <cstdlib>

using namespace std;

string releasetop(getenv("CMSSW_BASE"));
string testfileLocation= releasetop + "/src/IORawData/DaqSource/test/";

class testDaqSource: public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(testDaqSource);

  // Test reading from a raw data file
//   CPPUNIT_TEST(testReadFile);
//   CPPUNIT_TEST(testReadFileWritePool);
//   CPPUNIT_TEST(testReadPool);

  // Test generating random FED blocks
  CPPUNIT_TEST(testGenerate);
  CPPUNIT_TEST(testGenerateWritePool);
  CPPUNIT_TEST(testReadPool);


  CPPUNIT_TEST_SUITE_END();

public:


  void setUp(){
    char * ret = getenv("CMSSW_BASE");
    if (!ret) {
      cerr<< "env variable SCRAMRT_LOCALRT not set, try eval `scramv1 runt -csh`"<< endl;
      exit(1);
    }
  }

  void tearDown(){}  

  void testReadFile();
  void testReadFileWritePool();

  void testGenerate();
  void testGenerateWritePool();

  void testReadPool();

  int  runIt(const std::string& config);
 
}; 


int testDaqSource::runIt(const std::string& config){
  edm::AssertHandler ah;
  int rc=0;
  try {
    edm::EventProcessor proc(config);
    proc.run();
  } catch (seal::Error& e){
    std::cerr << "Exception caught:  " 
	      << e.explainSelf()
	      << std::endl;
    rc=1;
  }
  return rc;
}


// Read raw data from a file
void testDaqSource::testReadFile(){
  cout << endl << endl << " ---- testDaqSource::testReadFile ---- "
       << endl << endl;

  const std::string config=
    "process TEST = { \n"
    "source = DaqSource{ string reader = \"DaqFileReader\"\n"
    "                    untracked int32 maxEvents = 1\n"
    "                    PSet pset = { string fileName = \"" + testfileLocation+ "rawdt.raw" +"\"}} \n"
    "module dummyunpacker = DummyUnpackingModule{ }\n"
    "path p = {dummyunpacker}\n"
    "}\n";
  
  int rc = runIt(config);  
  CPPUNIT_ASSERT(rc==0);
}


// Read raw data from a file and write out a pool DB
void testDaqSource::testReadFileWritePool(){
  cout << endl << endl << " ---- testDaqSource::testReadFileWritePool ---- "
       << endl << endl;

  const std::string config=
    "process TEST = { \n"
    "source = DaqSource{ string reader = \"DaqFileReader\"\n"
    "                    untracked int32 maxEvents = 1\n"
    "                    PSet pset = { string fileName = \"" + testfileLocation+ "rawdt.raw" +"\"}} \n"
    "module dummyunpacker = DummyUnpackingModule{ }\n"
    "module out = PoolOutputModule {\n"
    "                     string fileName =\"" + testfileLocation+ "rawdata.root" +"\"} \n"
    "path p = {dummyunpacker,out}\n"
    "}\n";
 int rc = runIt(config);
 CPPUNIT_ASSERT(rc==0);

}


// Re-read the pool DB
void testDaqSource::testReadPool(){
  cout << endl << endl << " ---- testDaqSource::testReadPool ---- "
       << endl << endl;

  const std::string config=
    "process TEST = { \n"
    " module dummyunpacker = DummyUnpackingModule{ }\n"
    " path p = {dummyunpacker}\n"
    " source = PoolSource{ untracked vstring fileNames ={\"file:" + testfileLocation+ "rawdata.root" +"\"}} \n"
    "}\n";

  int rc = runIt(config);
  CPPUNIT_ASSERT(rc==0);

  
}


// Read raw data from a file
void testDaqSource::testGenerate(){
  cout << endl << endl << " ---- testDaqSource::testGenerate ---- " 
       << endl << endl;

  const std::string config=
    "process TEST = { \n"
    "source = DaqSource{ string reader = \"DaqFakeReader\"\n"
    "                    untracked int32 maxEvents = 1\n"
    "                    PSet pset = { untracked int32 dummy = 0} }\n"
    "module dummyunpacker = DummyUnpackingModule{ }\n"
    "path p = {dummyunpacker}\n"
    "}\n";
  
  int rc = runIt(config);  
  CPPUNIT_ASSERT(rc==0);
}


// Read raw data from a file and write out a pool DB
void testDaqSource::testGenerateWritePool(){
  cout << endl << endl << " ---- testDaqSource::testGenerateWritePool ---- " 
       << endl << endl;

  const std::string config=
    "process TEST = { \n"
    "source = DaqSource{ string reader = \"DaqFakeReader\"\n"
    "                    untracked int32 maxEvents = 1\n"
    "                    PSet pset = { untracked int32 dummy = 0} }\n"
    "module dummyunpacker = DummyUnpackingModule{ }\n"
    "module out = PoolOutputModule {\n"
    "                     untracked string fileName =\"" + testfileLocation+ "rawdata.root" +"\"} \n"
    "path p = {dummyunpacker,out}\n"
    "}\n";
 int rc = runIt(config);
 CPPUNIT_ASSERT(rc==0);

}

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testDaqSource);

#include <Utilities/Testing/interface/CppUnit_testdriver.icpp>
