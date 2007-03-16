#ifndef GCTTEST_H_
#define GCTTEST_H_

//
// This massive class contains a bunch of test functionality
// migrated from standalone test programs, to reduce duplication
// of code and allow the tests to be done in an EDAnalyzer.
//
// Split into separate files to keep file sizes down.
//
// Greg Heath, March 2007
//

#include <vector>

class gctTestElectrons;
class gctTestEnergyAlgos;
class gctTestFirmware;
class gctTestHtAndJetCounts;

class L1GlobalCaloTrigger;

class gctTestFunctions
{
public:

  // structs and typedefs

  // Constructor and destructor
  gctTestFunctions();
  ~gctTestFunctions();

  /// Load another event into the gct. Overloaded for the various ways of doing this.
  void loadNextEvent(L1GlobalCaloTrigger* &gct, const bool simpleEvent);
  void loadNextEvent(L1GlobalCaloTrigger* &gct, const std::string fileName, bool &endOfFile);
  void loadNextEvent(L1GlobalCaloTrigger* &gct, const std::string fileName);

  /// Read the input electron data (after GCT processing).
  void fillElectronData(const L1GlobalCaloTrigger* gct);

  /// Read the firmware results from a file for the next event
  void fillJetsFromFirmware(const std::string &fileName);

  /// Read the input jet data from the jetfinders (after GCT processing).
  void fillRawJetData(const L1GlobalCaloTrigger* gct);

  /// Check the electron sorter
  bool checkElectrons(const L1GlobalCaloTrigger* gct) const;

  /// Check the jet finder against results from the firmware
  bool checkJetFinder(const L1GlobalCaloTrigger* gct) const;

  /// Check the energy sums algorithms
  bool checkEnergySums(const L1GlobalCaloTrigger* gct) const;

  /// Check the Ht summing algorithms
  bool checkHtSums(const L1GlobalCaloTrigger* gct) const;

  /// Check the jet counting algorithms
  bool checkJetCounts(const L1GlobalCaloTrigger* gct) const;

private:

  gctTestElectrons*      theElectronsTester;
  gctTestEnergyAlgos*    theEnergyAlgosTester;
  gctTestFirmware*       theFirmwareTester;
  gctTestHtAndJetCounts* theHtAndJetCountsTester;

};

#endif /*GCTTEST_H_*/
