#include "EventFilter/GctRawToDigi/plugins/GctRawToDigi.h"

// System headers
#include <vector>
#include <sstream>
#include <iostream>

// Framework headers
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// Raw data collection headers
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

// GCT raw data format headers
#include "EventFilter/GctRawToDigi/src/GctBlockHeader.h"
//#include "EventFilter/GctRawToDigi/interface/L1GctInternalObject.h"

// GCT input data format headers
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

// GCT output data format headers
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

// Namespace resolution
using std::cout;
using std::endl;
using std::vector;

// Define class constants. 
const unsigned GctRawToDigi::MAX_BLOCKS = 128;


GctRawToDigi::GctRawToDigi(const edm::ParameterSet& iConfig) :
  inputLabel_(iConfig.getParameter<edm::InputTag>("inputLabel")),
  fedId_(iConfig.getParameter<int>("gctFedId")),
  verbose_(iConfig.getUntrackedParameter<bool>("verbose",false)),
  doEm_(iConfig.getUntrackedParameter<bool>("unpackEm",true)),
  doJets_(iConfig.getUntrackedParameter<bool>("unpackJets",true)),
  doEtSums_(iConfig.getUntrackedParameter<bool>("unpackEtSums",true)),
  doInternEm_(iConfig.getUntrackedParameter<bool>("unpackInternEm",true)),
  doFibres_(iConfig.getUntrackedParameter<bool>("unpackFibres",true)),
  blockUnpacker_()
{
  edm::LogInfo("GCT") << "GctRawToDigi will unpack FED Id " << fedId_ << endl;

  //register the products
  produces<L1CaloEmCollection>();
  produces<L1CaloRegionCollection>();
  produces<L1GctEmCandCollection>("isoEm");
  produces<L1GctEmCandCollection>("nonIsoEm");
  produces<L1GctInternEmCandCollection>();
  produces<L1GctJetCandCollection>("cenJets");
  produces<L1GctJetCandCollection>("forJets");
  produces<L1GctJetCandCollection>("tauJets");
  produces<L1GctEtTotal>();
  produces<L1GctEtHad>();
  produces<L1GctEtMiss>();
  produces<L1GctJetCounts>();
  produces<L1GctFibreCollection>();
}


GctRawToDigi::~GctRawToDigi()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GctRawToDigi::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // get raw data collection
   edm::Handle<FEDRawDataCollection> feds;
   iEvent.getByLabel(inputLabel_, feds);
   const FEDRawData& gctRcd = feds->FEDData(fedId_);
   
   edm::LogInfo("GCT") << "Upacking FEDRawData of size " << std::dec << gctRcd.size() << std::endl;

  // do a simple check of the raw data
  if (gctRcd.size()<16) {
      edm::LogWarning("Invalid Data") << "Empty/invalid GCT raw data, size = " << gctRcd.size();
      return;
  }
  else {
    unpack(gctRcd, iEvent);
  }

}


void GctRawToDigi::unpack(const FEDRawData& d, edm::Event& e)
{
  std::vector<GctBlockHeader> bHdrs; // For storing block headers
  bHdrs.reserve(16);  // Reserve approx the right amount of space.

  // ** DON'T RESERVE SPACE IN VECTORS FOR DEBUG UNPACK ITEMS! **
  
  // Collections for storing GCT input data.  
  std::auto_ptr<L1CaloEmCollection> rctEm( new L1CaloEmCollection() ); // Input electrons.
  std::auto_ptr<L1CaloRegionCollection> rctRgn( new L1CaloRegionCollection() ); // Input regions.
  
  // GCT intermediate data
  std::auto_ptr<L1GctInternEmCandCollection> gctInternEm( new L1GctInternEmCandCollection() ); 

  // GCT output data
  std::auto_ptr<L1GctEmCandCollection>  gctIsoEm   ( new L1GctEmCandCollection() );  gctIsoEm->reserve(4);
  std::auto_ptr<L1GctEmCandCollection>  gctNonIsoEm( new L1GctEmCandCollection() );  gctNonIsoEm->reserve(4);
  std::auto_ptr<L1GctJetCandCollection> gctCenJets ( new L1GctJetCandCollection() ); gctCenJets->reserve(4);
  std::auto_ptr<L1GctJetCandCollection> gctForJets ( new L1GctJetCandCollection() ); gctForJets->reserve(4);
  std::auto_ptr<L1GctJetCandCollection> gctTauJets ( new L1GctJetCandCollection() ); gctTauJets->reserve(4);
  std::auto_ptr<L1GctJetCounts> jetCounts( new L1GctJetCounts() );
  std::auto_ptr<L1GctEtTotal> etTotResult( new L1GctEtTotal() );
  std::auto_ptr<L1GctEtHad> etHadResult( new L1GctEtHad() );
  std::auto_ptr<L1GctEtMiss> etMissResult( new L1GctEtMiss() );

  // Fibres
  std::auto_ptr<L1GctFibreCollection> gctFibres( new L1GctFibreCollection() );

  // Setup blockUnpacker
  blockUnpacker_.setRctEmCollection( rctEm.get() );
  blockUnpacker_.setIsoEmCollection( gctIsoEm.get() );
  blockUnpacker_.setNonIsoEmCollection( gctNonIsoEm.get() );
  blockUnpacker_.setInternEmCollection( gctInternEm.get() );
  blockUnpacker_.setFibreCollection( gctFibres.get() );
  blockUnpacker_.setCentralJetCollection( gctCenJets.get() );
  blockUnpacker_.setForwardJetCollection( gctForJets.get() );
  blockUnpacker_.setTauJetCollection( gctTauJets.get() );
  blockUnpacker_.setJetCounts( jetCounts.get() );
  blockUnpacker_.setEtTotal( etTotResult.get() );
  blockUnpacker_.setEtHad( etHadResult.get() );
  blockUnpacker_.setEtMiss( etMissResult.get() );

  // Unpacking variables
  const unsigned char * data = d.data();
  unsigned dEnd = d.size()-16; // bytes in payload
  unsigned dPtr = 8; // data pointer (starts at 8 as there is a 64-bit Slink header at start of packet).
  bool lost = false;

  // read blocks
  for (unsigned nb=0; !lost && dPtr<dEnd && nb<MAX_BLOCKS; ++nb)
  {
    // read block header
    GctBlockHeader blockHead(&data[dPtr]);

    // unpack the block
    blockUnpacker_.convertBlock(&data[dPtr+4], blockHead);  // dPtr+4 to get to the block data.

    // store the header
    bHdrs.push_back(blockHead);
    
    // advance pointer
    unsigned blockLen = blockHead.length();
    unsigned nSamples = blockHead.nSamples();
    dPtr += 4*(blockLen*nSamples+1); // *4 because blockLen is in 32-bit words, +1 for header
  }
  
  // dump summary in verbose mode
  if (verbose_)
  {
    std::ostringstream os;
    os << "Found " << bHdrs.size() << " GCT internal headers" << endl;
    for (unsigned i=0; i<bHdrs.size(); ++i) { os << bHdrs[i]<< endl; }
    os << "Read " << rctEm.get()->size() << " RCT EM candidates" << endl;
    os << "Read " << gctIsoEm.get()->size() << " GCT iso EM candidates" << endl;
    os << "Read " << gctNonIsoEm.get()->size() << " GCT non-iso EM candidates" << endl;
    os << "Read " << gctInternEm.get()->size() << " GCT intermediate EM candidates" << endl;
    os << "Read " << gctCenJets.get()->size() << " GCT central jet candidates" << endl;
    os << "Read " << gctForJets.get()->size() << " GCT forward jet candidates" << endl;
    os << "Read " << gctTauJets.get()->size() << " Gct tau jet candidates" << endl;
    
    edm::LogVerbatim("GCT") << os.str();
  }

  // put data into the event
  if (doEm_)
  {
    e.put(rctEm);
    e.put(gctIsoEm, "isoEm");
    e.put(gctNonIsoEm, "nonIsoEm");
  }
  if (doJets_)
  {
    e.put(rctRgn);
    e.put(gctCenJets,"cenJets");
    e.put(gctForJets,"forJets");
    e.put(gctTauJets,"tauJets");
    e.put(jetCounts);
  }
  if (doEtSums_)
  {
    e.put(etTotResult);
    e.put(etHadResult);
    e.put(etMissResult);
  }
  if (doInternEm_) { e.put(gctInternEm); }
  if (doFibres_)   { e.put(gctFibres); }

}


// ------------ method called once each job just before starting event loop  ------------
void 
GctRawToDigi::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GctRawToDigi::endJob() {
}



/// make this a plugin
DEFINE_FWK_MODULE(GctRawToDigi);

