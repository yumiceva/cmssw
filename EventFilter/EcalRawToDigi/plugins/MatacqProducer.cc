#include "EventFilter/EcalRawToDigi/interface/MatacqProducer.h"
#include "EventFilter/EcalRawToDigi/src/Majority.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/EcalDigi/interface/EcalMatacqDigi.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <memory>

#include <stdio.h>

#include <fstream>

#include "DataFormats/EcalDigi/interface/EcalMatacqDigi.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

using namespace std;
using namespace boost;
using namespace edm;

// #undef LogInfo
// #define LogInfo(a) cout << "INFO " << a << ": "
// #undef LogWarning
// #define LogWarning(a) cout << "WARN " << a << ": "
// #undef LogDebug
// #define LogDebug(a) cout << "DBG " << a << ": "


//verbose mode for matacq event retrieval debugging:
//static const bool searchDbg = false;

//laser freq is 1 every 112 orbit => >80 orbit
int MatacqProducer::orbitTolerance_ = 80; 

MatacqProducer::stats_t MatacqProducer::stats_init = {0,0,0};

MatacqProducer::MatacqProducer(const edm::ParameterSet& params):
  fileNames_(params.getParameter<std::vector<std::string> >("fileNames")),
  digiInstanceName_(params.getParameter<string>("digiInstanceName")),
  rawInstanceName_(params.getParameter<string>("rawInstanceName")),
  timing_(params.getUntrackedParameter<bool>("timing", false)),
  disabled_(params.getParameter<bool>("disabled")),
  verbosity_(params.getUntrackedParameter<int>("verbosity", 0)),
  produceDigis_(params.getParameter<bool>("produceDigis")),
  produceRaw_(params.getParameter<bool>("produceRaw")),
  inputRawCollection_(params.getParameter<InputTag>("inputRawCollection")),
  mergeRaw_(params.getParameter<bool>("mergeRaw")),
  ignoreTriggerType_(params.getParameter<bool>("ignoreTriggerType")),
  matacq_(0, 0),
  inFile_(0),
  data_(bufferSize),
  openedFileRunNumber_(0),
  lastOrb_(0),
  fastRetrievalThresh_(0),
  orbitOffsetFile_(params.getUntrackedParameter<std::string>("orbitOffsetFile",
							     "")),
  inFileName_(""),
  stats_(stats_init),
  logFileName_(params.getUntrackedParameter<std::string>("logFileName",
							 "matacqProducer.log")){
  if(verbosity_>=4) cout << "[Matacq] in MatacqProducer ctor"  << endl;
  
  posEstim_.verbosity(verbosity_);

  logFile_.open(logFileName_.c_str(), ios::app | ios::out);
  
  if(logFile_.bad()){
    throw cms::Exception("FileOpen") << "Failed to open file "
				     << logFileName_ << " for logging.\n";
  }

  if(produceDigis_){
    if(verbosity_>0) cout << "[Matacq] registering new "
                       "EcalMatacqDigiCollection product with instance name '"
                          << digiInstanceName_ << "'\n";
    produces<EcalMatacqDigiCollection>(digiInstanceName_);
  }
  
  if(produceRaw_){
    if(verbosity_>0) cout << "[Matacq] registering new FEDRawDataCollection "
                       "product with instance name '"
                          << rawInstanceName_ << "'\n";
    produces<FEDRawDataCollection>(rawInstanceName_);
  }
  
  startTime_.tv_sec = startTime_.tv_usec = 0;
  if(orbitOffsetFile_.size()>0){
    doOrbitOffset_ = true;
    loadOrbitOffset();
  } else{
    doOrbitOffset_ = false;
  }
  if(verbosity_>=4) cout << "[Matacq] exiting MatacqProducer ctor"  << endl;
}


void
MatacqProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup){
  if(verbosity_>=4) cout << "[Matacq] in MatacqProducer::produce"  << endl;
  if(startTime_.tv_sec==0) gettimeofday(&startTime_, 0);
  ++stats_.nEvents;  
  if(disabled_) return;
  addMatacqData(event);
}

void
MatacqProducer::addMatacqData(edm::Event& event){
  edm::Handle<FEDRawDataCollection> sourceColl;
  if(inputRawCollection_.label().size() == 0
     && inputRawCollection_.instance().size() == 0){
    event.getByType(sourceColl);
  } else{
    event.getByLabel(inputRawCollection_, sourceColl);
  }
  
  std::auto_ptr<FEDRawDataCollection> rawColl;
  if(produceRaw_){
    if(mergeRaw_){
      rawColl = auto_ptr<FEDRawDataCollection>(new FEDRawDataCollection(*sourceColl));
    } else{
      rawColl = auto_ptr<FEDRawDataCollection>(new FEDRawDataCollection());
    }
  }
  
  std::auto_ptr<EcalMatacqDigiCollection>
    digiColl(new EcalMatacqDigiCollection());

  if(sourceColl->FEDData(matacqFedId_).size()>4 && !produceRaw_){
    //input raw data collection already contains matacqData
    formatter_.interpretRawData(sourceColl->FEDData(matacqFedId_),
                                *digiColl);             
  } else{
    bool isLaserEvent = (getCalibTriggerType(event) == laserType);
    if(isLaserEvent || ignoreTriggerType_){

      const uint32_t runNumber = getRunNumber(event);
      const uint32_t orbitId   = getOrbitId(event);

      LogInfo("Matacq") << "Run " << runNumber << "\t Orbit " << orbitId << "\n";
    
      bool fileChange;
      uint32_t offset = 0;
      if(doOrbitOffset_){
        map<uint32_t,uint32_t>::iterator it = orbitOffset_.find(runNumber);
        if(it == orbitOffset_.end()){
          LogWarning("IncorrectLaserEvent") << "Orbit offset not found for run "
                               << runNumber
                               << ". No orbit correction will be applied.";
        } else{
          offset = it->second;
        }
      }    
      
      if(getMatacqFile(runNumber, orbitId, &fileChange)){
        //matacq file retrieval succeeded
        LogInfo("Matacq") << "Matacq data file found for "
                          << "run " << runNumber << " orbit " << orbitId;
        if(getMatacqEvent(runNumber, orbitId, fileChange)){
          if(produceDigis_){
            formatter_.interpretRawData(matacq_, *digiColl);
          }
          if(produceRaw_){
            uint32_t dataLen64 = matacq_.getParsedLen();
            if(dataLen64 > bufferSize*8 || matacq_.getDccLen()!= dataLen64){
              LogWarning("IncorrectLaserEvent") << " Error in Matacq event fragment length! "
                                   << "DCC len: " << matacq_.getDccLen()
                                   << "*8 Bytes, Parsed len: "
                                   << matacq_.getParsedLen() << "*8 Bytes.  "
                                   << "Matacq data will not be included for this event.\n";
            } else{
              rawColl->FEDData(matacqFedId_).resize(dataLen64*8);
              copy(data_.begin(), data_.begin() + dataLen64*8,
                   rawColl->FEDData(matacqFedId_).data());
            }
          }
          LogInfo("Matacq") << "Associating matacq data with orbit id "
                            << matacq_.getOrbitId()
                            << " to dcc event with orbit id "
                            << orbitId << std::endl;
	  if(isLaserEvent){
	    ++stats_.nLaserEventsWithMatacq;
	  } else{
	    ++stats_.nNonLaserEventsWithMatacq;
	  }
        } else{
          LogWarning("IncorrectLaserEvent") << "No matacq data found for "
                               << "run " << runNumber << " orbit " << orbitId;
        }
      } else{
        LogWarning("IncorrectLaserEvent") << "No matacq file found for event "
                             << event.id();
      }
    }
  }
  
  if(produceRaw_){
    if(verbosity_>1) cout << "[Matacq] "
                          << "Adding FEDRawDataCollection collection "
                          << " to event.\n";
    event.put(rawColl, rawInstanceName_);
  }

  if(produceDigis_){
    if(verbosity_>1) cout << "[Matacq] "
                          << "Adding EcalMatacqDigiCollection collection "
                          << " to event.\n";
    event.put(digiColl, digiInstanceName_);
  }
}

// #if 0
// bool
// MatacqProducer::getMatacqEvent(std::ifstream& f,
// 			       uint32_t runNumber,
// 			       uint32_t orbitId,
// 			       bool doWrap,
// 			       std::streamoff maxPos){
//   bool found = false;
//   streampos startPos = f.tellg();

//   while(!f.eof()
// 	&& !found
// 	&& (maxPos<0 || f.tellg()<=maxPos)){
//     const streamsize headerSize = 8*8;
//     f.read((char*)&data_[0], headerSize);
//     if(f.eof()) break;
//     int32_t orb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);
//     uint32_t len = MatacqRawEvent::getDccLen(&data_[0], headerSize);
//     uint32_t run = MatacqRawEvent::getRunNum(&data_[0], headerSize);
//     //     cout << "Matacq: orbit = " << orb
//     // 	 << " len = " << len
//     // 	 << " run = " << run << endl;
//     if((abs(orb-(int32_t)orbitId) < orbitTolerance_)
//        && (runNumber==0 || runNumber==run)){
//       found = true;
//       //reads the rest of the event:
//       if(data_.size() < len*8){
// 	throw cms::Exception("Matacq") << "Buffer overflow";
//       }
//       f.read((char*)&data_[0]+headerSize, len*8-headerSize);
//       matacq_ = MatacqRawEvent((unsigned char*)&data_[0], len*8);
//     } else{
//       //moves to next event:
//       f.seekg(len*8 - headerSize, ios::cur);
//     }
//   }
  
//   f.clear(); //clears eof error to allow seekg  
//   if(doWrap && !found){
//     f.seekg(0, ios::beg);
//     found =  getMatacqEvent(f, runNumber, orbitId, false, startPos);
//   }
//   return found;
// }
//#endif

bool
MatacqProducer::getMatacqEvent(uint32_t runNumber,
			       int32_t orbitId,
			       bool fileChange){
  filepos_t startPos;
  if(!mtell(startPos)) return false;
  
  int32_t startOrb = -1;
  const size_t headerSize = 8*8;
  if(mread((char*)&data_[0], headerSize, "Reading matacq header", true)){
    startOrb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);
    if(startOrb<0) startOrb = 0;
  } else{
    if(verbosity_>2){
      cout << "[Matacq] Failed to read matacq header. Moved to start of "
	" the file.\n";
    }
    mrewind();
    if(mread((char*)&data_[0], headerSize, "Reading matacq header", true)){
      startPos = 0;
      startOrb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);
    } else{
      if(verbosity_>2) cout << "[Matacq] Looks like matacq file is empty"
                            << "\n";
      return false;
    }
  }
  
  if(verbosity_>2) cout << "[Matacq] Last read orbit: " << lastOrb_
                        << " looking for orbit " << orbitId
                        << ". Current file position: " << startPos
                        << " Orbit at current position: " << startOrb << "\n";
  
  //  f.clear();
  bool didCoarseMove = false;
  if(!posEstim_.invalid()
     && (abs(lastOrb_-orbitId) > fastRetrievalThresh_)){
    filepos_t pos = posEstim_.pos(orbitId);

    //    struct stat st;
    filepos_t fsize;
    //    if(0==stat(inFileName_.c_str(), &st)){
    if(msize(fsize)){
      //      const int64_t fsize = st.st_size;
      if(0!=posEstim_.eventLength() && pos > fsize){
	//estimated position is beyong end of file
	//-> move to beginning of last event:
	int64_t evtSize = posEstim_.eventLength()*sizeof(uint64_t);
	pos = ((int64_t)fsize/evtSize-1)*evtSize;
	if(verbosity_>2){
	  cout << "[Matacq] Estimated position was beyond end of file. "
	    "Changed to " << pos << "\n";
	}
      }
    } else{
      LogWarning("IncorrectConfiguration") << "Failed to access file " << inFileName_ << ".";
    }
    if(pos>=0){
      if(verbosity_>2) cout << "[Matacq] jumping to estimated position "
			    << pos << "\n";
      mseek(pos, SEEK_SET, "Jumping to estimated event position");
      if(mread((char*)&data_[0], headerSize, "Reading matacq header", true)){
	didCoarseMove = true;
      } else{
	//estimated position might have been beyond the end of the file,
	//try, with original position:
	didCoarseMove = false;
	if(!mread((char*)&data_[0], headerSize, "Reading event header", true)){
	  return false;
	}
      }
    } else{
      if(verbosity_) cout << "[Matacq] Event orbit outside of orbit range "
		       "of matacq data file events\n";
      return false;
    }
  }
  
  int32_t orb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);

  if(didCoarseMove){
    //autoadjustement of threshold for coarse move:
    if(abs(orb-orbitId) > fastRetrievalThresh_){
      if(verbosity_>2) cout << "[Matacq] Fast retrieval threshold increased from "
                            << fastRetrievalThresh_;
      fastRetrievalThresh_ = 2*abs(orb-orbitId);
      if(verbosity_>2) cout << " to " << fastRetrievalThresh_ << "\n";
    }

    //if coarse move did not improve situation, rolls back:
    if(startOrb > 0
       && (abs(orb-orbitId) > abs(startOrb-orbitId))){
      if(verbosity_>2) cout << "[Matacq] Estimation (-> orbit " << orb << ") "
                         "was worst than original position (-> orbit "
                            << startOrb
                            << "). Restoring position (" << startPos << ").\n";
      mseek(startPos, SEEK_SET);
      mread((char*)&data_[0], headerSize, "Reading event header", true);
      orb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);
    }
  }
  
  bool searchBackward = (orb>orbitId)?true:false;
  //BEWARE: len must be signed, because we are using latter in the code (-len)
  //expression
  int len = (int)MatacqRawEvent::getDccLen(&data_[0], headerSize);

  if(len==0){
    cout << "[Matacq] read DCC length is null! Cancels matacq event search "
	 << " and move matacq file pointer to beginning of the file. "
	 << "(" << __FILE__ << ":" << __LINE__ << ")."
	 << "\n";
    //rewind(f);
    mrewind();
    return false;
  }
  
  enum state_t { searching, found, failed } state = searching;
  
  while(state == searching){
    orb = MatacqRawEvent::getOrbitId(&data_[0], headerSize);
    len = (int)MatacqRawEvent::getDccLen(&data_[0], headerSize);
    uint32_t run = MatacqRawEvent::getRunNum(&data_[0], headerSize);
    if(verbosity_>3){
      filepos_t pos;
      mtell(pos);
      cout << "[Matacq] Header read at file position "
	   << pos
	   << ":  orbit = " << orb
	   << " len = " << len << "x8 Byte"
	   << " run = " << run << "\n";
    }
    if((abs(orb-orbitId) < orbitTolerance_)
       && (runNumber==0 || runNumber==run)){
      state = found;
      lastOrb_ = orb;
      //reads the rest of the event:
      if((int)data_.size() < len*8){
	throw cms::Exception("Matacq") << "Buffer overflow";
      }
      if(verbosity_>2) cout << "[Matacq] Event found. Reading "
                         " matacq event." << "\n";
      if(!mread((char*)&data_[0], len*8, "Reading matacq event")){
	if(verbosity_>2) cout << "[Matacq] Failed to read matacq event."
                              << "\n";
	state = failed;
      }
      matacq_ = MatacqRawEvent((unsigned char*)&data_[0], len*8);
    } else {
      if((searchBackward && (orb < orbitId))
	 || (!searchBackward) && (orb > orbitId)){ //search ended
	lastOrb_ = orb;      
	state = failed;
	if(verbosity_) cout << "[Matacq] Event search failed." << "\n";
      } else{
	off_t offset = (searchBackward?-len:len)*8;
	lastOrb_ = orb;	
	if(verbosity_>3){
	  cout << "[Matacq] In matacq file, moving "
	       << abs(offset) << " byte " << (offset>0?"forward":"backward")
	       << ".\n";
	}	

	if(mseek(offset, SEEK_CUR,
		 (searchBackward?"Moving to previous event":
		  "Moving to next event"))
	   && mread((char*)&data_[0], headerSize, "Reading event header",
		    true)){
	} else{
	  if(!searchBackward) mseek(-len*8, SEEK_CUR,
				    "Moving to start of last complete event");
	  state = failed;
	}
      }
    }
  }
  
  if(state==found){
    filepos_t pos;
    filepos_t fsize;
    mtell(pos);
    msize(fsize);
    if(pos==fsize-1){ //last byte.
      if(verbosity_>2){
	cout << "[Matacq] Event found was at the end of the file. Moving "
	  "stream position to beginning of this event."
	     << "\n";
      }
      mseek(-(int)len*8-1, SEEK_CUR,
	    "Moving to beginning of last matacq event");
    }
  }
  return (state==found);
}


bool
MatacqProducer::getMatacqFile(uint32_t runNumber, uint32_t orbitId,
			      bool* fileChange){
  if(openedFileRunNumber_!=0
     && openedFileRunNumber_==runNumber){
    if(fileChange!=0) *fileChange = false;
    return misOpened();
  }

  if(fileNames_.size()==0) return 0;

  const string runNumberFormat = "%08d";
  string sRunNumber = str(boost::format(runNumberFormat) % runNumber);
  //cout << "Run number string: " << sRunNumber << "\n";
  bool found = false;
  string fname;
  for(unsigned i=0; i < fileNames_.size() && !found; ++i){
    fname = fileNames_[i];
    boost::algorithm::replace_all(fname, "%run_subdir%",
				  runSubDir(runNumber));
    boost::algorithm::replace_all(fname, "%run_number%", sRunNumber);

    if(verbosity_) cout << "[Matacq] Looking for a file with path "
			<< fname << "\n";
    
    if(mcheck(fname)){
      LogInfo("Matacq") << "Uses matacq data file: '" << fname << "'\n";
      found = true;
    }
  }
  if(!found){
    openedFileRunNumber_ = 0;
    if(fileChange!=0) *fileChange = false;
    return 0;
  }
  
  if(!mopen(fname)){
    LogWarning("IncorrectConfiguration") << "Failed to open file " << fname << "\n";
    openedFileRunNumber_ = 0;
    if(fileChange!=0) *fileChange = false;
    return false;
  } else{
    openedFileRunNumber_ = runNumber;
    lastOrb_ = 0;
    posEstim_.init(this);
    if(fileChange!=0) *fileChange = true;
    return true;
  }
}
 

uint32_t MatacqProducer::getRunNumber(edm::Event& ev) const{
  return ev.run();
}

uint32_t MatacqProducer::getOrbitId(edm::Event& ev) const{
  //on CVS HEAD (June 4, 08), class Event has a method orbitNumber()
  //we could use here. The code would be shorten to:
  //return ev.orbitNumber();
  //we have to deal with what we have in current CMSSW releases:
  edm::Handle<FEDRawDataCollection> rawdata;
  if(!(ev.getByType(rawdata) && rawdata.isValid())){
    throw cms::Exception("NotFound")
      << "No FED raw data collection found. ECAL raw data are "
      "required to retrieve the orbit ID";
  }
  
  int orbit = 0;
  for(int id=601; id<=654; ++id){
    if(!FEDNumbering::inRange(id)) continue;
    const FEDRawData& data = rawdata->FEDData(id);
    const int orbitIdOffset64 = 3;
    if(data.size()>=8*(orbitIdOffset64+1)){//orbit id is in 4th 64-bit word
      const unsigned char* pOrbit = data.data() + orbitIdOffset64*8;
      int thisOrbit = pOrbit[0]
	| (pOrbit[1] <<8)
	| (pOrbit[2] <<16)
	| (pOrbit[3] <<24);
      if(orbit!=0 && thisOrbit!=0 && abs(orbit-thisOrbit)>orbitTolerance_){
	//throw cms::Exception("EventCorruption")
	//  << "Orbit ID inconsitency in DCC headers";
	LogWarning("IncorrectEvent")
	  << "Orbit ID inconsitency in DCC headers";
	orbit = 0;
	break;
      }
      if(thisOrbit!=0) orbit = thisOrbit;
    }
  }
  
  if(orbit==0){
    //    throw cms::Exception("NotFound")
    //  << "Failed to retrieve orbit ID of event "<< ev.id();
    LogWarning("IncorrectEvent") << "Failed to retrieve orbit ID of event "
				<< ev.id();
  }
  return orbit;
}
 
int MatacqProducer::getCalibTriggerType(edm::Event& ev) const{  
  edm::Handle<FEDRawDataCollection> rawdata;
  if(!(ev.getByType(rawdata) && rawdata.isValid())){
    throw cms::Exception("NotFound")
      << "No FED raw data collection found. ECAL raw data are "
      "required to retrieve the trigger type";
  }
  
  Majority<int> stat;
  for(int id=601; id<=654; ++id){
    if(!FEDNumbering::inRange(id)) continue;
    const FEDRawData& data = rawdata->FEDData(id);
    const int detailedTrigger32 = 5;
    if(data.size()>=4*(detailedTrigger32+1)){
      const unsigned char* pTType = data.data() + detailedTrigger32*4;
      int tType = pTType[1] & 0x7;
      stat.add(tType);
    }
  }
  double p;
  int tType = stat.result(&p);
  if(p<0){
    //throw cms::Exception("NotFound") << "No ECAL DCC data found\n";
    LogWarning("IncorrectEvent")  << "No ECAL DCC data found\n";
    tType = -1;
  }
  if(p<.8){
    //throw cms::Exception("EventCorruption") << "Inconsitency in detailed trigger type indicated in ECAL DCC data headers\n";
    LogWarning("IncorrectEvent") << "Inconsitency in detailed trigger type indicated in ECAL DCC data headers\n";
    tType = -1;
  }
  return tType;
}
  
void MatacqProducer::PosEstimator::init(MatacqProducer* mp){
  mp->mrewind();

  const size_t headerSize = 8*8;
  unsigned char data[headerSize];
  if(!mp->mread((char*)data, headerSize)){
    if(verbosity_) cout << "[Matacq] reached end of file!\n"; 
    firstOrbit_ = eventLength_ = orbitStepMean_ = 0;
    return;
  } else{
    firstOrbit_ = MatacqRawEvent::getOrbitId(data, headerSize);
    eventLength_ = MatacqRawEvent::getDccLen(data, headerSize);
    if(verbosity_>1) cout << "[Matacq] First event orbit: " << firstOrbit_
                          << " event length: " << eventLength_
                          << "*8 byte\n";
  }

  mp->mrewind();
  
  if(eventLength_==0){    
    if(verbosity_) cout << "[Matacq] event length is null!" << endl; 
    return;
  }

  filepos_t s;
  mp->msize(s);

  //number of complete events:
  const unsigned nEvents = s/eventLength_/8;

  if(nEvents==0){
    if(verbosity_) cout << "[Matacq] File is empty!" << endl;
    orbitStepMean_ = 0;
    return;
  }

  if(verbosity_>1) cout << "[Matacq] File size: " << s
                        << " Number of events: " << nEvents << endl;
  
  //position of last complete events:
  off_t last = (nEvents-1)*(off_t)eventLength_*8;
  mp->mseek(last, SEEK_SET, "Moving to beginning of last complete "
	    "matacq event");
  if(!mp->mread((char*) data, headerSize, "Reading matacq header", true)){
    LogWarning("IncorrectLaserEvent") << "Fast matacq event retrieval failure. "
      "Falling back to safe retrieval mode.";
    orbitStepMean_ = 0;
  }
  
  int32_t lastOrb = MatacqRawEvent::getOrbitId(data, headerSize);
  int32_t lastLen = MatacqRawEvent::getDccLen(data, headerSize);

  if(verbosity_>1) cout << "[Matacq] Last event orbit: " << lastOrb
                        << " last event length: " << lastLen << endl;
  
  //some consistency check
  if(lastLen!=eventLength_){
    LogWarning("IncorrectLaserEvent")
      << "Fast matacq event retrieval failure: it looks like "
      "the matacq file contains events of different sizes. Falling back to "
      "safe retrieval mode.";
    invalid_ = true;
    orbitStepMean_ = 0;
    return;
  }

  orbitStepMean_ = (lastOrb - firstOrbit_)/nEvents;
  
  if(verbosity_>1) cout << "[Matacq] Orbit step mean: " << orbitStepMean_
                        << "\n";

  invalid_ = false;
}

int64_t MatacqProducer::PosEstimator::pos(int orb) const{
  if(orb<firstOrbit_) return -1;
  uint64_t r = orbitStepMean_!=0?
    (((uint64_t)(orb-firstOrbit_))/orbitStepMean_)*eventLength_*8
    :0;
  if(verbosity_>2) cout << "[Matacq] Estimated Position for orbit  " << orb
                        << ": " << r << endl;
  return r;
}

MatacqProducer::~MatacqProducer(){
  mclose();
  timeval t;
  gettimeofday(&t, 0);
  if(timing_ && startTime_.tv_sec!=0){
    //not using logger, to allow timing with different logging options
    cout << "[Matacq] Time elapsed between first event and "
      "destruction of MatacqProducer: "
	 << ((t.tv_sec-startTime_.tv_sec)*1.
	     + (t.tv_usec-startTime_.tv_usec)*1.e-6) << "s\n";
  }
  logFile_ << "Event count: "
	   << "total: " << stats_.nEvents << ", "
	   << "Laser event with Matacq data: "
	   << stats_.nLaserEventsWithMatacq << ", "
	   << "Non laser event (according to DCC header) with Matacq data: "
	   << stats_.nNonLaserEventsWithMatacq << "\n";
}

void MatacqProducer::loadOrbitOffset(){
  ifstream f(orbitOffsetFile_.c_str());
  if(f.bad()){
    throw cms::Exception("Matacq")
      << "Failed to open orbit ID correction file '"
      << orbitOffsetFile_ << "'\n";
  }

  cout << "[Matacq] "
       << "Offset to substract to Matacq events Orbit ID: \n"
       << "#Run Number\t Offset\n";

  int iline = 0;
  string s;
  stringstream buf;
  while(f.eof()){
    getline(f, s);
    ++iline;
    if(s[0]=='#'){//comment
      //skip line:
      f.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }
    buf.str("");
    buf << s;
    int run;
    int orbit;
    buf >> run;
    buf >> orbit;
    if(buf.bad()){
      throw cms::Exception("Matacq")
	<< "Syntax error in Orbit offset file '"
	<< orbitOffsetFile_ << "'";
    }
    cout << run << "\t" << orbit << "\n";
    orbitOffset_.insert(pair<int, int>(run, orbit));
  }
}

#ifdef USE_STORAGE_MANAGER
bool MatacqProducer::mseek(filepos_t offset, int whence, const char* mess){
  if(0==inFile_.get()) return false;
  try{
    Storage::Relative wh;
    if(whence==SEEK_SET) wh = Storage::SET;
    else if(whence==SEEK_CUR) wh = Storage::CURRENT;
    else if(whence==SEEK_END) wh = Storage::END;
    else throw cms::Exception("Bug") << "Bug found in "
                                     << __FILE__ << ": "<< __LINE__ << "\n";
                   
    inFile_->position(offset, wh);
  } catch(cms::Exception& e){
    if(verbosity_){
      cout << "[Matacq] ";
      if(mess) cout << mess << ". ";
      cout << "Random access error on input matacq file. "
        "Reopening file. " << e.what() << "\n";
      mopen(inFileName_);
      return false;
    }
  }
  return true;
}

bool MatacqProducer::mtell(filepos_t& pos){
  if(0==inFile_.get()) return false;
  pos = inFile_->position();
  return true;
}

bool MatacqProducer::mread(char* buf, size_t n, const char* mess, bool peek){
  if(0==inFile_.get()) return false;
  
  filepos_t pos;
  if(!mtell(pos)) return false;

  bool rc = false;
  try{
    rc =  (n==inFile_->xread(buf, n));
  } catch(cms::Exception& e){
    if(verbosity_){
      cout << "[Matacq] ";
      if(mess) cout << mess << ". ";
      cout << "Read failure from input matacq file: "
           << e.what() << "\n";
    }
    //recovering from error:
    mopen(inFileName_);
    mseek(pos);
    return false;
  }
  if(peek){//asked to restore original file position
    mseek(pos);
  }
  return rc;
}

bool MatacqProducer::msize(filepos_t& s){
  if(inFile_.get()==0) return false;
  s = inFile_.get()->size();
  return true;
}

bool MatacqProducer::mrewind(){
  Storage* file = inFile_.get();
  if(file==0) return false;
  try{
    file->rewind();
  } catch(cms::Exception e){
    if(verbosity_) cout << "Exception cautgh while rewinding file "
                        << inFileName_ << ": " << e.what() << ". "
                        << "File will be reopened.";
    return mopen(inFileName_);
  }
  return true;
}

bool MatacqProducer::mcheck(const std::string& name){
  return StorageFactory::get()->check(name);
}

bool MatacqProducer::mopen(const std::string& name){
  //close already opened file if any:
  mclose();
  
  try{
    inFile_
      = auto_ptr<Storage>(StorageFactory::get()->open(name,
                                                      IOFlags::OpenRead));
    inFileName_ = name;
  } catch(cms::Exception& e){
    LogWarning("IncorrectConfiguration") << e.what();
    inFile_.reset();
    inFileName_ = "";
    return false;
  }
  return true;
}

void MatacqProducer::mclose(){
  if(inFile_.get()!=0){
    inFile_->close();
    inFile_.reset();
  }
}

bool MatacqProducer::misOpened(){
  return inFile_.get()!=0;
}

bool MatacqProducer::meof(){
  if(inFile_.get()==0) return true;
  return inFile_->eof();
}

#else //USE_STORAGE_MANAGER not defined
bool MatacqProducer::mseek(off_t offset, int whence, const char* mess){
  if(0==inFile_) return false;
  const int rc = fseeko(inFile_, offset, whence);
  if(rc!=0 && verbosity_){
    cout << "[Matacq] ";
    if(mess) cout << mess << ". ";
    cout << "Random access error on input matacq file. "
      "Rewind file.\n";
    mrewind();
  }
  return rc==0;
}

bool MatacqProducer::mtell(filepos_t& pos){
  if(0==inFile_) return false;
  pos = ftello(inFile_);
  return pos != -1;
    
}

bool MatacqProducer::mread(char* buf, size_t n, const char* mess, bool peek){
  if(0==inFile_) return false;
  off_t pos = ftello(inFile_);
  bool rc = (pos!=-1) && (1==fread(buf, n, 1, inFile_));
  if(!rc){
    if(verbosity_){
      cout << "[Matacq] ";
      if(mess) cout << mess << ". ";
      cout << "Read failure from input matacq file.\n";
    }
    clearerr(inFile_);
  }
  if(peek || !rc){//need to restore file position
    if(0!=fseeko(inFile_, pos, SEEK_SET)){
      if(verbosity_){
	cout << "[Matacq] ";
	if(mess) cout << mess << ". ";
	cout << "Failed to restore file position of "
	  "before read error. Rewind file.\n";
      }
      //rewind(inFile_.get());
      mrewind();
      lastOrb_ = 0;
    }
  }
  return rc;
}

bool MatacqProducer::msize(filepos_t& s){
  if(0==inFile_) return false;
  struct stat buf;
  if(0!=fstat(fileno(inFile_), &buf)){
    s = 0;
    return false;
  } else{
    s = buf.st_size;
    return true;
  }
}

bool MatacqProducer::mrewind(){
  if(0==inFile_) return false;
  clearerr(inFile_);
  return fseeko(inFile_, 0, SEEK_SET)!=0; 
}

bool MatacqProducer::mcheck(const std::string& name){
  struct stat dummy;
  return 0==stat(name.c_str(), &dummy);
}

bool MatacqProducer::mopen(const std::string& name){
  if(inFile_!=0) mclose();
  inFile_ = fopen(name.c_str(), "r");
  if(inFile_!=0){
    inFileName_ = name;
    return true;
  } else{
    inFileName_ = "";
    return false;
  }
}

void MatacqProducer::mclose(){
  if(inFile_!=0) fclose(inFile_);
  inFile_ = 0;
}

bool MatacqProducer::misOpened(){
  return inFile_!=0;
}

bool MatacqProducer::meof(){
  if(0==inFile_) return true;
  return feof(inFile_)==0;
}

#endif //USE_STORAGE_MANAGER defined

std::string MatacqProducer::runSubDir(uint32_t runNumber){
  int millions = runNumber / (1000*1000);
  int thousands = (runNumber-millions*1000*1000) / 1000;
  int units = runNumber-millions*1000*1000 - thousands*1000;
  return str(boost::format("%03d/%03d/%03d") % millions % thousands % units);
}
