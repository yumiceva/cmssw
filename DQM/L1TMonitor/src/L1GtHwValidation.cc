/**
 * \class L1GtHwValidation
 * 
 * 
 * Description: compare hardware records with emulator records for L1 GT records.
 *
 * Implementation:
 *    Get the L1 GT records from data and from emulator.   
 *    Compare every board between data and emulator.
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 * $Date$
 * $Revision$
 *
 */

// this class header
#include "DQM/L1TMonitor/interface/L1GtHwValidation.h"

// system include files
#include <memory>
#include <iostream>
#include <iomanip>

// user include files
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Run.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

// constructor(s)
L1GtHwValidation::L1GtHwValidation(const edm::ParameterSet& paramSet) :
            //
            // input tag for the L1 GT hardware DAQ record
            m_l1GtDataDaqInputTag(paramSet.getParameter<edm::InputTag>(
                    "L1GtDataDaqInputTag")),
            // input tag for the L1 GT hardware EVM record
            m_l1GtDataEvmInputTag(paramSet.getParameter<edm::InputTag>(
                    "L1GtDataEvmInputTag")),
            // input tag for the L1 GT emulator DAQ record
            m_l1GtEmulDaqInputTag(paramSet.getParameter<edm::InputTag>(
                    "L1GtEmulDaqInputTag")),
            // input tag for the L1 GT emulator EVM record
            m_l1GtEmulEvmInputTag(paramSet.getParameter<edm::InputTag>(
                    "L1GtEmulEvmInputTag")),
            // input tag for the L1 GCT hardware record
            m_l1GctDataInputTag(paramSet.getParameter<edm::InputTag>(
                    "L1GctDataInputTag")),
            //
            m_dirName(paramSet.getUntrackedParameter("DirName", std::string(
                    "L1Trigger/L1ExtraDQM/source"))),
            // initialize counters
            m_nrDataEventError(0),
            m_nrEmulEventError(0),
            // cache
            m_l1GtMenuCacheID(0ULL), m_l1GtPfAlgoCacheID(0ULL),
            m_l1GtPfTechCacheID(0ULL), m_l1GtTmAlgoCacheID(0ULL),
            m_l1GtTmTechCacheID(0ULL),
            //
            m_dbe(0),
            //
            m_nrEvJob(0), m_nrEvRun(0) {

    LogDebug("L1GtHwValidation")
            << "\nInput tag for the L1 GT DAQ hardware record:       "
            << m_l1GtDataDaqInputTag
            << "\nInput tag for the L1 GT EVM hardware record:       "
            << m_l1GtDataEvmInputTag
            << "\nInput tag for the L1 GT DAQ emulator records:          "
            << m_l1GtEmulDaqInputTag
            << "\nInput tag for the L1 GT EVM emulator records:          "
            << m_l1GtEmulEvmInputTag
            << "\nInput tag for the L1 GCT hardware record:          "
            << m_l1GctDataInputTag << std::endl;

    m_dbe = edm::Service<DQMStore>().operator->();
    if (m_dbe == 0) {
        edm::LogInfo("L1GtHwValidation")
                << "\n Unable to get DQMStore service.";
    } else {

        if (paramSet.getUntrackedParameter<bool>("DQMStore", false)) {
            m_dbe->setVerbose(0);
        }

        m_dbe->setCurrentFolder(m_dirName);

    }

}

// destructor
L1GtHwValidation::~L1GtHwValidation() {
    // empty   
}

// member functions

// method called once each job just before starting event loop
void L1GtHwValidation::beginJob() {

    DQMStore* dbe = 0;
    dbe = edm::Service<DQMStore>().operator->();

    // clean up directory
    if (dbe) {
        dbe->setCurrentFolder(m_dirName);
        if (dbe->dirExists(m_dirName)) {
            dbe->rmdir(m_dirName);
        }
        dbe->setCurrentFolder(m_dirName);
    }

    // book histograms
    bookHistograms();

}

void L1GtHwValidation::beginRun(const edm::Run& iRun,
        const edm::EventSetup& evSetup) {

    m_nrEvRun = 0;

    // get / update the trigger menu from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtMenuCacheID =
            evSetup.get<L1GtTriggerMenuRcd>().cacheIdentifier();

    if (m_l1GtMenuCacheID != l1GtMenuCacheID) {

        edm::ESHandle<L1GtTriggerMenu> l1GtMenu;
        evSetup.get<L1GtTriggerMenuRcd>().get(l1GtMenu);
        m_l1GtMenu = l1GtMenu.product();

        m_l1GtMenuCacheID = l1GtMenuCacheID;

    }

    // FIXME when the menu changes, make a copy of histograms, and clear the old one
    //       otherwise the labels are wrong
    const AlgorithmMap& algorithmMap = m_l1GtMenu->gtAlgorithmMap();

    for (CItAlgo itAlgo = algorithmMap.begin(); itAlgo != algorithmMap.end(); itAlgo++) {

        std::string aName = itAlgo->first;
        const char* algName = aName.c_str();
        int algBitNumber = (itAlgo->second).algoBitNumber();

        for (int iRec = 0; iRec < NumberOfGtRecords; ++iRec) {
            for (int iBxInEvent = 0; iBxInEvent < TotalBxInEvent; ++iBxInEvent) {
                m_fdlDataAlgoDecision[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataAlgoDecisionPrescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataAlgoDecisionUnprescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataAlgoDecisionMask[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataAlgoDecision_NoMatch[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataAlgoDecision_Err[iRec]->setBinLabel(algBitNumber + 1,
                        algName, 1);

                m_fdlEmulAlgoDecision[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlEmulAlgoDecisionPrescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlEmulAlgoDecisionUnprescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlEmulAlgoDecisionMask[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlEmulAlgoDecision_NoMatch[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlEmulAlgoDecision_Err[iRec]->setBinLabel(algBitNumber + 1,
                        algName, 1);

                m_fdlDataEmulAlgoDecision[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataEmulAlgoDecisionPrescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataEmulAlgoDecisionUnprescaled[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataEmulAlgoDecisionMask[iBxInEvent][iRec]->setBinLabel(
                        algBitNumber + 1, algName, 1);
                m_fdlDataEmulAlgoDecision_Err[iRec]->setBinLabel(algBitNumber
                        + 1, algName, 1);
            }
        }

    }

    // get / update the prescale factors from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtPfAlgoCacheID = evSetup.get<
            L1GtPrescaleFactorsAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtPfAlgoCacheID != l1GtPfAlgoCacheID) {

        edm::ESHandle<L1GtPrescaleFactors> l1GtPfAlgo;
        evSetup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(l1GtPfAlgo);
        m_l1GtPfAlgo = l1GtPfAlgo.product();

        m_prescaleFactorsAlgoTrig = &(m_l1GtPfAlgo->gtPrescaleFactors());

        m_l1GtPfAlgoCacheID = l1GtPfAlgoCacheID;

    }

    unsigned long long l1GtPfTechCacheID = evSetup.get<
            L1GtPrescaleFactorsTechTrigRcd>().cacheIdentifier();

    if (m_l1GtPfTechCacheID != l1GtPfTechCacheID) {

        edm::ESHandle<L1GtPrescaleFactors> l1GtPfTech;
        evSetup.get<L1GtPrescaleFactorsTechTrigRcd>().get(l1GtPfTech);
        m_l1GtPfTech = l1GtPfTech.product();

        m_prescaleFactorsTechTrig = &(m_l1GtPfTech->gtPrescaleFactors());

        m_l1GtPfTechCacheID = l1GtPfTechCacheID;

    }

    // get / update the trigger mask from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtTmAlgoCacheID = evSetup.get<
            L1GtTriggerMaskAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtTmAlgoCacheID != l1GtTmAlgoCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmAlgo;
        evSetup.get<L1GtTriggerMaskAlgoTrigRcd>().get(l1GtTmAlgo);
        m_l1GtTmAlgo = l1GtTmAlgo.product();

        m_triggerMaskAlgoTrig = m_l1GtTmAlgo->gtTriggerMask();

        m_l1GtTmAlgoCacheID = l1GtTmAlgoCacheID;

    }

    unsigned long long l1GtTmTechCacheID = evSetup.get<
            L1GtTriggerMaskTechTrigRcd>().cacheIdentifier();

    if (m_l1GtTmTechCacheID != l1GtTmTechCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmTech;
        evSetup.get<L1GtTriggerMaskTechTrigRcd>().get(l1GtTmTech);
        m_l1GtTmTech = l1GtTmTech.product();

        m_triggerMaskTechTrig = m_l1GtTmTech->gtTriggerMask();

        m_l1GtTmTechCacheID = l1GtTmTechCacheID;

    }

}

//compare the GTFE board
void L1GtHwValidation::compareGTFE(const edm::Event& iEvent,
        const edm::EventSetup& evSetup, const L1GtfeWord& gtfeBlockData,
        const L1GtfeWord& gtfeBlockEmul, const int iRec) {

    //
    std::string recString;
    if (iRec == 0) {
        recString = "DAQ";
    } else {
        recString = "EVM";
    }

    if (gtfeBlockData == gtfeBlockEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE blocks: identical.\n";
        gtfeBlockData.print(m_myCoutStream);
    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE blocks: different.\n";

        m_myCoutStream << "\nData: GTFE block\n";
        gtfeBlockData.print(m_myCoutStream);

        m_myCoutStream << "\nEmul: GTFE block\n";
        gtfeBlockEmul.print(m_myCoutStream);

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get BoardId value
    const boost::uint16_t boardIdData = gtfeBlockData.boardId();
    const boost::uint16_t boardIdEmul = gtfeBlockEmul.boardId();

    if (boardIdData == boardIdEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE boardId identical.";
        m_myCoutStream << "\n boardId() = " << std::hex << "0x" << std::setw(4)
                << std::setfill('0') << boardIdData << std::setfill(' ')
                << std::dec;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE boardId different.";
        m_myCoutStream << "\n Data: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n Emul: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdEmul
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(0);

    }

    /// get record length for alternative 1
    const boost::uint16_t recordLength1Data = gtfeBlockData.recordLength1();
    const boost::uint16_t recordLength1Emul = gtfeBlockEmul.recordLength1();

    if (recordLength1Data == recordLength1Emul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE recordLength for alternative 1 identical.";
        m_myCoutStream << "\n recordLength1() = " << recordLength1Data;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE recordLength for alternative 1 different.";
        m_myCoutStream << "\n Data: recordLength1() = " << recordLength1Data;
        m_myCoutStream << "\n Emul: recordLength1() = " << recordLength1Emul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(1);

    }

    /// get record length for alternative 0
    const boost::uint16_t recordLengthData = gtfeBlockData.recordLength();
    const boost::uint16_t recordLengthEmul = gtfeBlockEmul.recordLength();

    if (recordLengthData == recordLengthEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE recordLength for alternative 0 identical.";
        m_myCoutStream << "\n recordLength() = " << recordLengthData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE recordLength for alternative 1 different.";
        m_myCoutStream << "\n Data: recordLength() = " << recordLengthData;
        m_myCoutStream << "\n Emul: recordLength() = " << recordLengthEmul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(2);

    }

    /// get bunch cross number as counted in the GTFE board
    const boost::uint16_t bxNrData = gtfeBlockData.bxNr();
    const boost::uint16_t bxNrEmul = gtfeBlockEmul.bxNr();

    if (bxNrData == bxNrEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE bxNr identical.";
        m_myCoutStream << "\n bxNr() = " << bxNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE bxNr different.";
        m_myCoutStream << "\n Data: bxNr() = " << bxNrData;
        m_myCoutStream << "\n Emul: bxNr() = " << bxNrEmul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(3);

    }

    /// get setup version
    const boost::uint32_t setupVersionData = gtfeBlockData.setupVersion();
    const boost::uint32_t setupVersionEmul = gtfeBlockEmul.setupVersion();

    if (setupVersionData == setupVersionEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE setupVersion identical.";
        m_myCoutStream << "\n setupVersion() = " << setupVersionData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE setupVersion different.";
        m_myCoutStream << "\n Data: setupVersion() = " << setupVersionData;
        m_myCoutStream << "\n Emul: setupVersion() = " << setupVersionEmul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(4);

    }

    /// get boards contributing to EVM respectively DAQ record
    const boost::uint16_t activeBoardsData = gtfeBlockData.activeBoards();
    const boost::uint16_t activeBoardsEmul = gtfeBlockEmul.activeBoards();

    if (activeBoardsData == activeBoardsEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE activeBoards identical.";
        m_myCoutStream << "\n activeBoards() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << activeBoardsData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE activeBoards different.";
        m_myCoutStream << "\n Data: activeBoards() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << activeBoardsData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n Emul: activeBoards() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << activeBoardsEmul
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(5);

    }
    /// alternative for number of BX per board
    ///     correlated with active boards
    ///     bit value is 0: take alternative 0
    ///     bit value is 1: take alternative 1
    const boost::uint16_t altNrBxBoardData = gtfeBlockData.altNrBxBoard();
    const boost::uint16_t altNrBxBoardEmul = gtfeBlockEmul.altNrBxBoard();

    if (altNrBxBoardData == altNrBxBoardEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE altNrBxBoard identical.";
        m_myCoutStream << "\n altNrBxBoard() = " << altNrBxBoardData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE altNrBxBoard different.";
        m_myCoutStream << "\n Data: altNrBxBoard() = " << altNrBxBoardData;
        m_myCoutStream << "\n Emul: altNrBxBoard() = " << altNrBxBoardEmul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(6);

    }

    /// get total number of L1A sent since start of run
    const boost::uint32_t totalTriggerNrData = gtfeBlockData.totalTriggerNr();
    const boost::uint32_t totalTriggerNrEmul = gtfeBlockEmul.totalTriggerNr();

    if (totalTriggerNrData == totalTriggerNrEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE totalTriggerNr identical.";
        m_myCoutStream << "\n totalTriggerNr() = " << totalTriggerNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated GTFE totalTriggerNr different.";
        m_myCoutStream << "\n Data: totalTriggerNr() = " << totalTriggerNrData;
        m_myCoutStream << "\n Emul: totalTriggerNr() = " << totalTriggerNrEmul;
        m_myCoutStream << "\n";
        m_gtfeDataEmul[iRec]->Fill(7);

    }

    edm::LogInfo("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

}

//compare the FDL board
void L1GtHwValidation::compareFDL(const edm::Event& iEvent,
        const edm::EventSetup& evSetup, const L1GtFdlWord& fdlBlockData,
        const L1GtFdlWord& fdlBlockEmul, const int iRec) {

    // index of physics partition
    int PhysicsPartition = 0;

    // 
    std::string recString;
    if (iRec == 0) {
        recString = "DAQ";
    } else {
        recString = "EVM";
    }

    if (fdlBlockData == fdlBlockEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL blocks: identical.\n";
        fdlBlockData.print(m_myCoutStream);

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL blocks: different.\n";

        m_myCoutStream << "\nData: FDL block\n";
        fdlBlockData.print(m_myCoutStream);

        m_myCoutStream << "\nEmul: FDL block\n";
        fdlBlockEmul.print(m_myCoutStream);

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get bunch cross in the GT event record - 
    // move it first as histograms are BxInEvent dependent
    const int bxInEventData = fdlBlockData.bxInEvent();
    const int bxInEventEmul = fdlBlockEmul.bxInEvent();

    bool matchBxInEvent = false;

    if (bxInEventData == bxInEventEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL bxInEvent identical.";
        m_myCoutStream << "\n bxInEvent() = " << bxInEventData;
        m_myCoutStream << "\n";
        matchBxInEvent = true;

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL bxInEvent different.";
        m_myCoutStream << "\n Data: bxInEvent() = " << bxInEventData;
        m_myCoutStream << "\n Emul: bxInEvent() = " << bxInEventEmul;
        m_myCoutStream << "\n";

        m_fdlDataEmul_Err[iRec]->Fill(1);
    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // symmetrize 
    bool validBxInEvent = false;
    int histIndex = bxInEventData + (TotalBxInEvent + 1) / 2 - 1;
    LogDebug("L1GtHwValidation") << "\n Convert bxInEvent = " << bxInEventData
            << " to histIndex = " << histIndex << std::endl;
    if ((histIndex <= TotalBxInEvent) && (histIndex >= 0)) {
        validBxInEvent = true;
    }

    // loop over algorithms and increase the corresponding counters

    // get BoardId value
    const boost::uint16_t boardIdData = fdlBlockData.boardId();
    const boost::uint16_t boardIdEmul = fdlBlockEmul.boardId();

    if (boardIdData == boardIdEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL boardId identical.";
        m_myCoutStream << "\n boardId() = " << std::hex << "0x" << std::setw(4)
                << std::setfill('0') << boardIdData << std::setfill(' ')
                << std::dec;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL boardId different.";
        m_myCoutStream << "\n Data: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n Emul: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdEmul
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(0);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(0);
        }

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get BxNr - bunch cross number of the actual bx
    const boost::uint16_t bxNrData = fdlBlockData.bxNr();
    const boost::uint16_t bxNrEmul = fdlBlockEmul.bxNr();

    if (bxNrData == bxNrEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL bxNr identical.";
        m_myCoutStream << "\n bxNr() = " << bxNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL bxNr different.";
        m_myCoutStream << "\n Data: bxNr() = " << bxNrData;
        m_myCoutStream << "\n Emul: bxNr() = " << bxNrEmul;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(2);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(2);
        }
    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get event number since last L1 reset generated in FDL
    const boost::uint32_t eventNrData = fdlBlockData.eventNr();
    const boost::uint32_t eventNrEmul = fdlBlockEmul.eventNr();

    if (eventNrData == eventNrEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL eventNr identical.";
        m_myCoutStream << "\n eventNr() = " << eventNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL eventNr different.";
        m_myCoutStream << "\n Data: eventNr() = " << eventNrData;
        m_myCoutStream << "\n Emul: eventNr() = " << eventNrEmul;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(3);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(3);
        }

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get  technical trigger bits
    const TechnicalTriggerWord& gtTechnicalTriggerWordData =
            fdlBlockData.gtTechnicalTriggerWord();
    const TechnicalTriggerWord& gtTechnicalTriggerWordEmul =
            fdlBlockEmul.gtTechnicalTriggerWord();

    int nTechBits = gtTechnicalTriggerWordData.size();

    TechnicalTriggerWord gtTechnicalTriggerWordDataMask(nTechBits);
    TechnicalTriggerWord gtTechnicalTriggerWordEmulMask(nTechBits);

    unsigned int bitValue = 0;

    if (matchBxInEvent && validBxInEvent) {
        for (int iBit = 0; iBit < nTechBits; ++iBit) {

            unsigned int triggerMask = (m_triggerMaskTechTrig.at(iBit)) & (1
                    << PhysicsPartition);

            if (gtTechnicalTriggerWordData[iBit]) {
                m_fdlDataTechDecision[histIndex][iRec]->Fill(iBit);

                bitValue = (triggerMask) ? 0 : 1;
                gtTechnicalTriggerWordDataMask[iBit] = bitValue;
                if (bitValue) {
                    m_fdlDataTechDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }

            if (gtTechnicalTriggerWordEmul.at(iBit)) {
                m_fdlEmulTechDecision[histIndex][iRec]->Fill(iBit);

                bitValue = (triggerMask) ? 0 : 1;
                gtTechnicalTriggerWordEmulMask[iBit] = bitValue;
                if (bitValue) {
                    m_fdlEmulTechDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }
        }
    } else {
        for (int iBit = 0; iBit < nTechBits; ++iBit) {

            if (gtTechnicalTriggerWordData[iBit]) {
                m_fdlDataTechDecision_Err[iRec]->Fill(iBit);
            }

            if (gtTechnicalTriggerWordEmul.at(iBit)) {
                m_fdlEmulTechDecision_Err[iRec]->Fill(iBit);
            }
        }
    }

    if (gtTechnicalTriggerWordData == gtTechnicalTriggerWordEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtTechnicalTriggerWord identical.\n";
        fdlBlockData.printGtTechnicalTriggerWord(m_myCoutStream);
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtTechnicalTriggerWord different.";
        m_myCoutStream << "\n Data: ";
        fdlBlockData.printGtTechnicalTriggerWord(m_myCoutStream);
        m_myCoutStream << "\n Emul: ";
        fdlBlockEmul.printGtTechnicalTriggerWord(m_myCoutStream);
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(4);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(4);
        }

        if (matchBxInEvent && validBxInEvent) {
            for (int iBit = 0; iBit < nTechBits; ++iBit) {
                if (gtTechnicalTriggerWordData[iBit]
                        != gtTechnicalTriggerWordEmul.at(iBit)) {
                    m_fdlDataEmulTechDecision[histIndex][iRec]->Fill(iBit);
                }
            }
        } else {
            for (int iBit = 0; iBit < nTechBits; ++iBit) {
                if (gtTechnicalTriggerWordData[iBit]
                        != gtTechnicalTriggerWordEmul.at(iBit)) {
                    m_fdlDataEmulTechDecision_Err[iRec]->Fill(iBit);
                }
            }
        }
    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    if (gtTechnicalTriggerWordDataMask == gtTechnicalTriggerWordEmulMask) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtTechnicalTriggerWord after mask identical.\n";
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtTechnicalTriggerWord after mask different.";
        m_myCoutStream << "\n Data: ";
        m_myCoutStream << "\n Emul: ";
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(5);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(5);
        }

        if (matchBxInEvent && validBxInEvent) {
            for (int iBit = 0; iBit < nTechBits; ++iBit) {
                if (gtTechnicalTriggerWordData[iBit]
                        != gtTechnicalTriggerWordEmul.at(iBit)) {
                    m_fdlDataEmulTechDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }
        }
    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get algorithms bits (decision word)
    const DecisionWord& gtDecisionWordData = fdlBlockData.gtDecisionWord();
    const DecisionWord& gtDecisionWordEmul = fdlBlockEmul.gtDecisionWord();

    int nAlgoBits = gtDecisionWordData.size();

    DecisionWord gtDecisionWordDataMask(nAlgoBits);
    DecisionWord gtDecisionWordEmulMask(nAlgoBits);

    // get the index of the prescale factor set from data
    int iPfSet = fdlBlockData.gtPrescaleFactorIndexAlgo();
    const std::vector<int>& prescaleFactorsAlgoTrig =
            (*m_prescaleFactorsAlgoTrig).at(iPfSet);


    if (matchBxInEvent && validBxInEvent) {

        for (int iBit = 0; iBit < nAlgoBits; ++iBit) {

            unsigned int triggerMask = (m_triggerMaskAlgoTrig.at(iBit)) & (1
                    << PhysicsPartition);

            int prescaleFactor = prescaleFactorsAlgoTrig.at(iBit);

            LogTrace("L1GtHwValidation") << "Bit " << iBit
                    << ": prescale factor = " << prescaleFactor
                    << " trigger mask = " << triggerMask << std::endl;

            if (gtDecisionWordData[iBit]) {
                m_fdlDataAlgoDecision[histIndex][iRec]->Fill(iBit);

                if (prescaleFactor == 1) {
                    m_fdlDataAlgoDecisionUnprescaled[histIndex][iRec]->Fill(
                            iBit);
                } else {
                    m_fdlDataAlgoDecisionPrescaled[histIndex][iRec]->Fill(iBit);
                }

                bitValue = (triggerMask) ? 0 : 1;
                gtDecisionWordDataMask[iBit] = bitValue;
                if (bitValue) {
                    m_fdlDataAlgoDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }

            if (gtDecisionWordEmul.at(iBit)) {
                m_fdlEmulAlgoDecision[histIndex][iRec]->Fill(iBit);

                bitValue = (triggerMask) ? 0 : 1;
                gtDecisionWordEmulMask[iBit] = bitValue;
                if (bitValue) {
                    m_fdlEmulAlgoDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }
        }
    } else {
        for (int iBit = 0; iBit < nAlgoBits; ++iBit) {
            if (gtDecisionWordData[iBit]) {
                m_fdlDataAlgoDecision_Err[iRec]->Fill(iBit);
            }
        }

        for (int iBit = 0; iBit < nAlgoBits; ++iBit) {
            if (gtDecisionWordEmul.at(iBit)) {
                m_fdlEmulAlgoDecision_Err[iRec]->Fill(iBit);
            }
        }
    }

    if (gtDecisionWordData == gtDecisionWordEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWord identical.";
        fdlBlockData.printGtDecisionWord(m_myCoutStream);
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWord different.";
        m_myCoutStream << "\n Data: ";
        fdlBlockData.printGtDecisionWord(m_myCoutStream);
        m_myCoutStream << "\n Emul: ";
        fdlBlockEmul.printGtDecisionWord(m_myCoutStream);
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(6);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(6);
        }

        if (matchBxInEvent && validBxInEvent) {
            for (int iBit = 0; iBit < nAlgoBits; ++iBit) {

                int prescaleFactor = prescaleFactorsAlgoTrig.at(iBit);

                if (gtDecisionWordData[iBit] != gtDecisionWordEmul.at(iBit)) {

                    m_fdlDataEmulAlgoDecision[histIndex][iRec]->Fill(iBit);

                    if (prescaleFactor == 1) {
                        m_fdlDataEmulAlgoDecisionUnprescaled[histIndex][iRec]->Fill(
                                iBit);
                    } else {
                        m_fdlDataEmulAlgoDecisionPrescaled[histIndex][iRec]->Fill(
                                iBit);
                    }

                    if (gtDecisionWordData[iBit]) {
                        m_fdlDataAlgoDecision_NoMatch[histIndex][iRec]->Fill(
                                iBit);

                        if (prescaleFactor == 1) {
                            m_fdlDataAlgoDecisionUnprescaled_NoMatch[histIndex][iRec]->Fill(
                                    iBit);
                        } else {
                            m_fdlDataAlgoDecisionPrescaled_NoMatch[histIndex][iRec]->Fill(
                                    iBit);
                        }

                    } else {
                        m_fdlEmulAlgoDecision_NoMatch[histIndex][iRec]->Fill(
                                iBit);

                        if (prescaleFactor == 1) {
                            m_fdlEmulAlgoDecisionUnprescaled_NoMatch[histIndex][iRec]->Fill(
                                    iBit);
                        } else {
                            m_fdlEmulAlgoDecisionPrescaled_NoMatch[histIndex][iRec]->Fill(
                                    iBit);
                        }
                    }
                }
            }
        } else {
            for (int iBit = 0; iBit < nAlgoBits; ++iBit) {
                if (gtDecisionWordData[iBit] != gtDecisionWordEmul.at(iBit)) {
                    m_fdlDataEmulAlgoDecision_Err[iRec]->Fill(iBit);
                }
            }
        }

    }

    if (gtDecisionWordDataMask == gtDecisionWordEmulMask) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWord after mask identical.";
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWord after mask different.";
        m_myCoutStream << "\n Data: ";
        m_myCoutStream << "\n Emul: ";
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(7);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(7);
        }

        if (matchBxInEvent && validBxInEvent) {
            for (int iBit = 0; iBit < nAlgoBits; ++iBit) {
                if (gtDecisionWordDataMask[iBit] != gtDecisionWordEmulMask.at(
                        iBit)) {
                    m_fdlDataEmulAlgoDecisionMask[histIndex][iRec]->Fill(iBit);
                }
            }
        }

    }

    // get  extended algorithms bits (extended decision word)
    const DecisionWordExtended gtDecisionWordExtendedData =
            fdlBlockData.gtDecisionWordExtended();
    const DecisionWordExtended gtDecisionWordExtendedEmul =
            fdlBlockEmul.gtDecisionWordExtended();

    if (gtDecisionWordExtendedData == gtDecisionWordExtendedEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWordExtended identical.\n";
        fdlBlockData.printGtDecisionWordExtended(m_myCoutStream);
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL gtDecisionWordExtended different.\n";
        m_myCoutStream << "\n Data: ";
        fdlBlockData.printGtDecisionWordExtended(m_myCoutStream);
        m_myCoutStream << "\n Emul: ";
        fdlBlockEmul.printGtDecisionWordExtended(m_myCoutStream);
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(8);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(8);
        }

    }

    // get  NoAlgo
    const boost::uint16_t noAlgoData = fdlBlockData.noAlgo();
    const boost::uint16_t noAlgoEmul = fdlBlockEmul.noAlgo();

    if (noAlgoData == noAlgoEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL noAlgo identical.";
        m_myCoutStream << "\n noAlgo() = " << noAlgoData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL noAlgo different.";
        m_myCoutStream << "\n Data: noAlgo() = " << noAlgoData;
        m_myCoutStream << "\n Emul: noAlgo() = " << noAlgoEmul;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(9);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(9);
        }

    }

    // get  "Final OR" bits
    const boost::uint16_t finalORData = fdlBlockData.finalOR();
    const boost::uint16_t finalOREmul = fdlBlockEmul.finalOR();

    if (finalORData == finalOREmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL finalOR identical.";
        m_myCoutStream << "\n finalOR() = " << std::hex << "0x" << std::setw(2)
                << std::setfill('0') << finalORData << std::setfill(' ')
                << std::dec;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL finalOR different.";
        m_myCoutStream << "\n Data: finalOR() = " << std::hex << "0x"
                << std::setw(2) << std::setfill('0') << finalORData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n Emul: finalOR() = " << std::hex << "0x"
                << std::setw(2) << std::setfill('0') << finalOREmul
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(10);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(10);
        }

    }

    // get  "Final OR" for physics partition
    const int finalORPhysData = finalORData & (1 << PhysicsPartition);
    const int finalORPhysEmul = finalOREmul & (1 << PhysicsPartition);

    if (finalORPhysData == finalORPhysEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL finalOR for the physics partition identical.";
        m_myCoutStream << "\n finalOR() = " << finalORPhysData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL finalOR for the physics partition  different.";
        m_myCoutStream << "\n Data: finalOR() = " << finalORPhysData;
        m_myCoutStream << "\n Emul: finalOR() = " << finalORPhysEmul;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(11);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(11);
        }

    }

    // get  local bunch cross number of the actual bx
    const boost::uint16_t localBxNrData = fdlBlockData.localBxNr();
    const boost::uint16_t localBxNrEmul = fdlBlockEmul.localBxNr();

    if (localBxNrData == localBxNrEmul) {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL localBxNr identical.";
        m_myCoutStream << "\n localBxNr() = " << localBxNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\n" << recString
                << " Data and emulated FDL localBxNr different.";
        m_myCoutStream << "\n Data: localBxNr() = " << localBxNrData;
        m_myCoutStream << "\n Emul: localBxNr() = " << localBxNrEmul;
        m_myCoutStream << "\n";

        if (matchBxInEvent && validBxInEvent) {
            m_fdlDataEmul[histIndex][iRec]->Fill(12);
        } else {
            m_fdlDataEmul_Err[iRec]->Fill(12);
        }

    }

    edm::LogInfo("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

}

//compare the PSB board
void L1GtHwValidation::comparePSB(const edm::Event& iEvent,
        const edm::EventSetup& evSetup, const L1GtPsbWord& psbBlockData,
        const L1GtPsbWord& psbBlockEmul) {

    if (psbBlockData == psbBlockEmul) {
        m_myCoutStream << "\nData and emulated PSB blocks: identical.\n";
        psbBlockData.print(m_myCoutStream);

    } else {
        m_myCoutStream << "\nData and emulated PSB blocks: different.\n";

        m_myCoutStream << "\nData: PSB block\n";
        psbBlockData.print(m_myCoutStream);

        m_myCoutStream << "\nEmul: PSB block\n";
        psbBlockEmul.print(m_myCoutStream);

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // get BoardId value
    const boost::uint16_t boardIdData = psbBlockData.boardId();
    const boost::uint16_t boardIdEmul = psbBlockEmul.boardId();

    if (boardIdData == boardIdEmul) {
        m_myCoutStream << "\nData and emulated PSB boardId identical.";
        m_myCoutStream << "\n boardId() = " << std::hex << "0x" << std::setw(4)
                << std::setfill('0') << boardIdData << std::setfill(' ')
                << std::dec;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\nData and emulated PSB boardId different.";
        m_myCoutStream << "\n Data: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdData
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n Emul: boardId() = " << std::hex << "0x"
                << std::setw(4) << std::setfill('0') << boardIdEmul
                << std::setfill(' ') << std::dec;
        m_myCoutStream << "\n";

    }

    // get bunch cross in the GT event record
    const int bxInEventData = psbBlockData.bxInEvent();
    const int bxInEventEmul = psbBlockEmul.bxInEvent();

    if (bxInEventData == bxInEventEmul) {
        m_myCoutStream << "\nData and emulated PSB bxInEvent identical.";
        m_myCoutStream << "\n bxInEvent() = " << bxInEventData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\nData and emulated PSB bxInEvent different.";
        m_myCoutStream << "\n Data: bxInEvent() = " << bxInEventData;
        m_myCoutStream << "\n Emul: bxInEvent() = " << bxInEventEmul;
        m_myCoutStream << "\n";

    }

    // get BxNr - bunch cross number of the actual bx
    const boost::uint16_t bxNrData = psbBlockData.bxNr();
    const boost::uint16_t bxNrEmul = psbBlockEmul.bxNr();

    if (bxNrData == bxNrEmul) {
        m_myCoutStream << "\nData and emulated PSB bxNr identical.";
        m_myCoutStream << "\n bxNr() = " << bxNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\nData and emulated PSB bxNr different.";
        m_myCoutStream << "\n Data: bxNr() = " << bxNrData;
        m_myCoutStream << "\n Emul: bxNr() = " << bxNrEmul;
        m_myCoutStream << "\n";

    }

    // get event number since last L1 reset generated in FDL
    const boost::uint32_t eventNrData = psbBlockData.eventNr();
    const boost::uint32_t eventNrEmul = psbBlockEmul.eventNr();

    if (eventNrData == eventNrEmul) {
        m_myCoutStream << "\nData and emulated PSB eventNr identical.";
        m_myCoutStream << "\n eventNr() = " << eventNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\nData and emulated PSB eventNr different.";
        m_myCoutStream << "\n Data: eventNr() = " << eventNrData;
        m_myCoutStream << "\n Emul: eventNr() = " << eventNrEmul;
        m_myCoutStream << "\n";

    }

    /// get/set A_DATA_CH_IA
    boost::uint16_t valData;
    boost::uint16_t valEmul;

    for (int iA = 0; iA < psbBlockData.NumberAData; ++iA) {
        valData = psbBlockData.aData(iA);
        valEmul = psbBlockEmul.aData(iA);

        if (valData == valEmul) {
            m_myCoutStream << "\nData and emulated PSB aData(" << iA
                    << ") identical.";
            m_myCoutStream << "\n aData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valData
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n";

        } else {
            m_myCoutStream << "\nData and emulated PSB aData(" << iA
                    << ") different.";
            m_myCoutStream << "\n Data: aData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valData
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n Emul: aData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valEmul
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n";

        }

    }

    /// get/set B_DATA_CH_IB
    for (int iB = 0; iB < psbBlockData.NumberBData; ++iB) {
        valData = psbBlockData.bData(iB);
        valEmul = psbBlockEmul.bData(iB);

        if (valData == valEmul) {
            m_myCoutStream << "\nData and emulated PSB bData(" << iB
                    << ") identical.";
            m_myCoutStream << "\n bData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valData
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n";

        } else {
            m_myCoutStream << "\nData and emulated PSB bData(" << iB
                    << ") different.";
            m_myCoutStream << "\n Data: bData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valData
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n Emul: bData(iA) = " << std::hex << "0x"
                    << std::setw(4) << std::setfill('0') << valEmul
                    << std::setfill(' ') << std::dec;
            m_myCoutStream << "\n";

        }

    }

    // get  local bunch cross number of the actual bx
    const boost::uint16_t localBxNrData = psbBlockData.localBxNr();
    const boost::uint16_t localBxNrEmul = psbBlockEmul.localBxNr();

    if (localBxNrData == localBxNrEmul) {
        m_myCoutStream << "\nData and emulated PSB localBxNr identical.";
        m_myCoutStream << "\n localBxNr() = " << localBxNrData;
        m_myCoutStream << "\n";

    } else {
        m_myCoutStream << "\nData and emulated PSB localBxNr different.";
        m_myCoutStream << "\n Data: localBxNr() = " << localBxNrData;
        m_myCoutStream << "\n Emul: localBxNr() = " << localBxNrEmul;
        m_myCoutStream << "\n";

    }

    edm::LogInfo("L1GtHwValidation") << m_myCoutStream.str() << std::endl;
    m_myCoutStream.str("");
    m_myCoutStream.clear();

}

//compare the TCS board
void L1GtHwValidation::compareTCS(const edm::Event& iEvent,
        const edm::EventSetup& evSetup, const L1TcsWord&, const L1TcsWord&) {
    // empty
}

//L1 GT DAQ record comparison
void L1GtHwValidation::compareDaqRecord(const edm::Event& iEvent,
        const edm::EventSetup& evSetup) {

    // formal index for DAQ record 
    int iRec = 0;

    // get the L1 GT hardware DAQ record L1GlobalTriggerReadoutRecord
    edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecordData;
    iEvent.getByLabel(m_l1GtDataDaqInputTag, gtReadoutRecordData);

    bool validData = false;

    if (!gtReadoutRecordData.isValid()) {
        m_nrDataEventError++;
    } else {
        validData = true;
    }

    // get the L1 GT emulator DAQ record L1GlobalTriggerReadoutRecord
    edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecordEmul;
    iEvent.getByLabel(m_l1GtEmulDaqInputTag, gtReadoutRecordEmul);

    bool validEmul = false;

    if (!gtReadoutRecordEmul.isValid()) {
        m_nrEmulEventError++;
    } else {
        validEmul = true;
    }

    if ((!validData) || (!validEmul)) {
        edm::LogWarning("L1GtHwValidation")
                << "\n No valid product found: DAQ L1GlobalTriggerReadoutRecord"
                << "\n     Data validity [1 = true; 0 = false]: " << validData
                << "\n     Emulator validity: [1 = true; 0 = false]: "
                << validEmul << "\n DAQ histograms will not be filled.\n"
                << std::endl;

        return;
    }

    // compare GTFE
    const L1GtfeWord& gtfeBlockData = gtReadoutRecordData->gtfeWord();
    const L1GtfeWord& gtfeBlockEmul = gtReadoutRecordEmul->gtfeWord();

    compareGTFE(iEvent, evSetup, gtfeBlockData, gtfeBlockEmul, iRec);

    // FDL comparison
    const std::vector<L1GtFdlWord>& gtFdlVectorData =
            gtReadoutRecordData->gtFdlVector();
    const std::vector<L1GtFdlWord>& gtFdlVectorEmul =
            gtReadoutRecordEmul->gtFdlVector();

    int gtFdlVectorDataSize = gtFdlVectorData.size();
    int gtFdlVectorEmulSize = gtFdlVectorEmul.size();

    if (gtFdlVectorDataSize == gtFdlVectorEmulSize) {
        m_myCoutStream << "\nData and emulated FDL vector size: identical.\n";
        m_myCoutStream << "  Size: " << gtFdlVectorDataSize << std::endl;

        for (int iFdl = 0; iFdl < gtFdlVectorDataSize; ++iFdl) {

            const L1GtFdlWord& fdlBlockData = gtFdlVectorData[iFdl];
            const L1GtFdlWord& fdlBlockEmul = gtFdlVectorEmul[iFdl];

            compareFDL(iEvent, evSetup, fdlBlockData, fdlBlockEmul, iRec);
        }
    } else {
        m_myCoutStream << "\nData and emulated FDL vector size: different.\n";
        m_myCoutStream << "  Data: size = " << gtFdlVectorDataSize << std::endl;
        m_myCoutStream << "  Emul: size = " << gtFdlVectorEmulSize << std::endl;

    }

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

    // PSB comparison
    const std::vector<L1GtPsbWord>& gtPsbVectorData =
            gtReadoutRecordData->gtPsbVector();
    const std::vector<L1GtPsbWord>& gtPsbVectorEmul =
            gtReadoutRecordEmul->gtPsbVector();

    int gtPsbVectorDataSize = gtPsbVectorData.size();
    int gtPsbVectorEmulSize = gtPsbVectorEmul.size();

    if (gtPsbVectorDataSize == gtPsbVectorEmulSize) {
        m_myCoutStream << "\nData and emulated PSB vector size: identical.\n";
        m_myCoutStream << "  Size: " << gtPsbVectorDataSize << std::endl;
    } else {
        m_myCoutStream << "\nData and emulated PSB vector size: different.\n";
        m_myCoutStream << "  Data: size = " << gtPsbVectorDataSize << std::endl;
        m_myCoutStream << "  Emul: size = " << gtPsbVectorEmulSize << std::endl;

    }

    // the order of PSB block in the gtPsbVector is different in emulator and in data
    // in emulator: all active PSB in one BxInEvent, ordered L1A-1, L1A, L1A+1
    // in unpacker: every PSB in all BxInEvent
    for (int iPsb = 0; iPsb < gtPsbVectorDataSize; ++iPsb) {

        const L1GtPsbWord& psbBlockData = gtPsbVectorData[iPsb];
        const boost::uint16_t boardIdData = psbBlockData.boardId();
        const int bxInEventData = psbBlockData.bxInEvent();

        // search the corresponding PSB in the emulated record using the 
        // BoardId and the BxInEvent  

        bool foundPSB = false;

        for (int iPsbF = 0; iPsbF < gtPsbVectorEmulSize; ++iPsbF) {

            const L1GtPsbWord& psbBlockEmul = gtPsbVectorEmul[iPsbF];
            const boost::uint16_t boardIdEmul = psbBlockEmul.boardId();
            const int bxInEventEmul = psbBlockEmul.bxInEvent();

            if ((boardIdEmul == boardIdData)
                    && (bxInEventData == bxInEventEmul)) {

                foundPSB = true;

                // compare the boards
                comparePSB(iEvent, evSetup, psbBlockData, psbBlockEmul);
            }

        }

        if (!foundPSB) {
            m_myCoutStream << "\nNo emulated PSB with boardId() = " << std::hex
                    << "0x" << std::setw(4) << std::setfill('0') << boardIdData
                    << std::setfill(' ') << std::dec << " and BxInEvent = "
                    << bxInEventData << " was found";

        }

    }

    edm::LogInfo("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

}

// L1 GT EVM record comparison
void L1GtHwValidation::compareEvmRecord(const edm::Event& iEvent,
        const edm::EventSetup& evSetup) {

    // formal index for EVM record
    int iRec = 1;

    // get the L1 GT hardware EVM record L1GlobalTriggerEvmReadoutRecord
    edm::Handle<L1GlobalTriggerEvmReadoutRecord> gtReadoutRecordData;
    iEvent.getByLabel(m_l1GtDataEvmInputTag, gtReadoutRecordData);

    bool validData = false;

    if (!gtReadoutRecordData.isValid()) {
        m_nrDataEventError++;
    } else {
        validData = true;
    }

    // get the L1 GT emulator EVM record L1GlobalTriggerEvmReadoutRecord
    edm::Handle<L1GlobalTriggerEvmReadoutRecord> gtReadoutRecordEmul;
    iEvent.getByLabel(m_l1GtEmulEvmInputTag, gtReadoutRecordEmul);

    bool validEmul = false;

    if (!gtReadoutRecordEmul.isValid()) {
        m_nrEmulEventError++;
    } else {
        validEmul = true;
    }

    if ((!validData) || (!validEmul)) {
        edm::LogWarning("L1GtHwValidation")
                << "\n No valid product found: EVM L1GlobalTriggerEvmReadoutRecord"
                << "\n     Data validity [1 = true; 0 = false]: " << validData
                << "\n     Emulator validity: [1 = true; 0 = false]: "
                << validEmul << "\n EVM histograms will not be filled.\n"
                << std::endl;

        return;
    }

    // compare GTFE
    const L1GtfeWord& gtfeBlockData = gtReadoutRecordData->gtfeWord();
    const L1GtfeWord& gtfeBlockEmul = gtReadoutRecordEmul->gtfeWord();

    compareGTFE(iEvent, evSetup, gtfeBlockData, gtfeBlockEmul, iRec);

    // FDL comparison
    const std::vector<L1GtFdlWord>& gtFdlVectorData =
            gtReadoutRecordData->gtFdlVector();
    const std::vector<L1GtFdlWord>& gtFdlVectorEmul =
            gtReadoutRecordEmul->gtFdlVector();

    int gtFdlVectorDataSize = gtFdlVectorData.size();
    int gtFdlVectorEmulSize = gtFdlVectorEmul.size();

    if (gtFdlVectorDataSize == gtFdlVectorEmulSize) {
        m_myCoutStream << "\nData and emulated FDL vector size: identical.\n";
        m_myCoutStream << "  Size: " << gtFdlVectorDataSize << std::endl;

        for (int iFdl = 0; iFdl < gtFdlVectorDataSize; ++iFdl) {

            const L1GtFdlWord& fdlBlockData = gtFdlVectorData[iFdl];
            const L1GtFdlWord& fdlBlockEmul = gtFdlVectorEmul[iFdl];

            compareFDL(iEvent, evSetup, fdlBlockData, fdlBlockEmul, iRec);
        }
    } else {
        m_myCoutStream << "\nData and emulated FDL vector size: different.\n";
        m_myCoutStream << "  Data: size = " << gtFdlVectorDataSize << std::endl;
        m_myCoutStream << "  Emul: size = " << gtFdlVectorEmulSize << std::endl;

    }

    // FIXME compare TCS

    LogDebug("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

    edm::LogInfo("L1GtHwValidation") << m_myCoutStream.str() << std::endl;

    m_myCoutStream.str("");
    m_myCoutStream.clear();

}

// compare the GCT collections obtained from L1 GT PSB with the input 
// GCT collections
void L1GtHwValidation::compareGt_Gct(const edm::Event& iEvent,
        const edm::EventSetup&) {

    // FIXME

}

// analyze each event: event loop
void L1GtHwValidation::analyze(const edm::Event& iEvent,
        const edm::EventSetup& evSetup) {

    ++m_nrEvJob;
    ++m_nrEvRun;

    // L1 GT DAQ record comparison
    compareDaqRecord(iEvent, evSetup);

    // L1 GT EVM record comparison
    compareEvmRecord(iEvent, evSetup);

    // GCT collections from L1 GT PSB versus unpacked GCT 
    compareGt_Gct(iEvent, evSetup);

}

// book all histograms for the module
void L1GtHwValidation::bookHistograms() {

    // histograms

    const unsigned int numberTechTriggers =
            L1GlobalTriggerReadoutSetup::NumberTechnicalTriggers;

    const unsigned int numberAlgoTriggers =
            L1GlobalTriggerReadoutSetup::NumberPhysTriggers;

    for (int iRec = 0; iRec < NumberOfGtRecords; ++iRec) {

        std::string recString;
        if (iRec == 0) {
            recString = "Daq_";
        } else {
            recString = "Evm_";
        }

        std::string hName;
        const char* histName;

        hName = recString + "gtfeDataEmul";
        histName = hName.c_str();

        // GTFE histograms
        m_gtfeDataEmul[iRec] = m_dbe->book1D(histName,
                "GTFE data vs emul mismatch", 8, 0., 7.);
        m_gtfeDataEmul[iRec]->setBinLabel(1, "BoardId", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(2, "RecordLength1", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(3, "RecordLength0", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(4, "BxNr", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(5, "SetupVersion", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(6, "DaqActiveBoards", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(7, "AltNrBxBoard", 1);
        m_gtfeDataEmul[iRec]->setBinLabel(8, "TotalTriggerNr", 1);

        // FDL histograms

        for (int iHist = 0; iHist < TotalBxInEvent; ++iHist) {

            // convert [0, TotalBxInEvent] to [-X, +X] and add to histogram name 
            int iIndex = iHist - ((TotalBxInEvent + 1) / 2 - 1);
            int hIndex = (iIndex + 16) % 16;

            std::stringstream ss;
            std::string str;
            ss << std::uppercase << std::hex << hIndex;
            ss >> str;

            hName = recString + "FdlDataEmul_" + str;
            histName = hName.c_str();

            std::string hTitle = "FDL data vs emul mismatch for BxInEvent = "
                    + str;
            const char* histTitle = hTitle.c_str();

            //

            m_fdlDataEmul[iHist][iRec] = m_dbe->book1D(histName, histTitle, 13,
                    0., 13.);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(1, "BoardId", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(2, "BxInEvent", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(3, "BxNr", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(4, "EventNr", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(5, "TechTrigger", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(6, "TechTriggerMask", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(7, "AlgoTrigger", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(8, "AlgoTriggerMask", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(9, "AlgoExtend", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(10, "NoAlgo", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(11, "FinalORAllParts", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(12, "FinalORPhysPart", 1);
            m_fdlDataEmul[iHist][iRec]->setBinLabel(13, "LocalBxNr", 1);

            // algorithm decision
            //   data
            hName = recString + "Data_AlgoDecision_" + str;
            histName = hName.c_str();

            hTitle = "Data: algorithm decision word for BxInEvent = " + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberAlgoTriggers, 0., numberAlgoTriggers);

            //   emul
            hName = recString + "Emul_AlgoDecision_" + str;
            histName = hName.c_str();

            hTitle = "Emul: algorithm decision word for BxInEvent = " + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberAlgoTriggers, 0., numberAlgoTriggers);

            // algorithm decision for prescaled algorithms
            //   data
            hName = recString + "Data_AlgoDecision_Prescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data: prescaled algorithms: algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecisionPrescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            //   emul
            hName = recString + "Emul_AlgoDecision_Prescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: prescaled algorithms: algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecisionPrescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            // algorithm decision for unprescaled algorithms
            //   data
            hName = recString + "Data_AlgoDecision_Unprescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data: unprescaled algorithms: algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecisionUnprescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            //   emul
            hName = recString + "Emul_AlgoDecision_Unprescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: unprescaled algorithms: algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecisionUnprescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            // algorithm decision after masking (partition physics)
            //   data
            hName = recString + "Data_AlgoDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data, physics partition: algorithm decision word after mask for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecisionMask[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberAlgoTriggers, 0., numberAlgoTriggers);

            //   emul
            hName = recString + "Emul_AlgoDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul, physics partition: algorithm decision word after mask for BxInEvent =  "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecisionMask[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberAlgoTriggers, 0., numberAlgoTriggers);

            //
            hName = recString + "DataEmul_AlgoDecision_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul: non-matching algorithm decision word for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulAlgoDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberAlgoTriggers, 0., numberAlgoTriggers);

            //
            hName = recString + "DataEmul_AlgoDecision_Prescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul: prescaled algorithms with non-matching decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulAlgoDecisionPrescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            //
            hName = recString + "DataEmul_AlgoDecision_Unprescaled_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul: unprescaled algorithms with non-matching decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulAlgoDecisionUnprescaled[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            //
            hName = recString + "Data_AlgoDecision_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data: algorithm decision for non-matching cases for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecision_NoMatch[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            //
            hName = recString + "Emul_AlgoDecision_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: algorithm decision for non-matching cases for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecision_NoMatch[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            // prescaled algorithms
            hName = recString + "Data_AlgoDecision_Prescaled_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data: prescaled algorithms: non-matching algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecisionPrescaled_NoMatch[iHist][iRec]
                    = m_dbe->book1D(histName, histTitle, numberAlgoTriggers,
                            0., numberAlgoTriggers);

            //
            hName = recString + "Emul_AlgoDecision_Prescaled_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: prescaled algorithms: non-matching algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecisionPrescaled_NoMatch[iHist][iRec]
                    = m_dbe->book1D(histName, histTitle, numberAlgoTriggers,
                            0., numberAlgoTriggers);


            // unprescaled algorithms - non-matching
            hName = recString + "Data_AlgoDecision_Unprescaled_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data: unprescaled algorithms: non-matching algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataAlgoDecisionUnprescaled_NoMatch[iHist][iRec]
                    = m_dbe->book1D(histName, histTitle, numberAlgoTriggers,
                            0., numberAlgoTriggers);

            //
            hName = recString + "Emul_AlgoDecision_Unprescaled_NoMatch_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: unprescaled algorithms: non-matching algorithm decision for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulAlgoDecisionUnprescaled_NoMatch[iHist][iRec]
                    = m_dbe->book1D(histName, histTitle, numberAlgoTriggers,
                            0., numberAlgoTriggers);


            // 
            hName = recString + "DataEmul_AlgoDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul, physics partition: non-matching algorithm decision word after mask for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulAlgoDecisionMask[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberAlgoTriggers, 0.,
                    numberAlgoTriggers);

            // technical trigger decision 
            //   data
            hName = recString + "Data_TechDecision_" + str;
            histName = hName.c_str();

            hTitle = "Data technical trigger decision word for BxInEvent = "
                    + str;
            histTitle = hTitle.c_str();

            m_fdlDataTechDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberTechTriggers, 0., numberTechTriggers);

            //   emul
            hName = recString + "Emul_TechDecision_" + str;
            histName = hName.c_str();

            hTitle = "Emul: technical trigger decision word for BxInEvent = "
                    + str;
            histTitle = hTitle.c_str();

            m_fdlEmulTechDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberTechTriggers, 0., numberTechTriggers);

            // technical trigger decision after masking (partition physics)
            hName = recString + "Data_TechDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data technical trigger decision word after mask for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataTechDecisionMask[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberTechTriggers, 0., numberTechTriggers);

            //
            hName = recString + "Emul_TechDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Emul: technical trigger decision word after mask for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlEmulTechDecisionMask[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberTechTriggers, 0., numberTechTriggers);

            //
            hName = recString + "DataEmul_TechDecision_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul: non-matching technical trigger decision word for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulTechDecision[iHist][iRec] = m_dbe->book1D(histName,
                    histTitle, numberTechTriggers, 0., numberTechTriggers);

            hName = recString + "DataEmul_TechDecisionAfterMask_" + str;
            histName = hName.c_str();

            hTitle
                    = "Data vs emul: non-matching technical trigger decision word after mask for BxInEvent = "
                            + str;
            histTitle = hTitle.c_str();

            m_fdlDataEmulTechDecisionMask[iHist][iRec] = m_dbe->book1D(
                    histName, histTitle, numberTechTriggers, 0.,
                    numberTechTriggers);

        }

        hName = recString + "FdlDataEmul_Err";
        histName = hName.c_str();

        m_fdlDataEmul_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "FDL data vs emul mismatch for non-matching BxInEvent in FDL payload",
                        13, 0., 13.);
        m_fdlDataEmul_Err[iRec]->setBinLabel(1, "BoardId", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(2, "BxInEvent", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(3, "BxNr", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(4, "EventNr", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(5, "TechTrigger", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(6, "TechTriggerMask", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(7, "AlgoTrigger", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(8, "AlgoTriggerMask", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(9, "AlgoExtend", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(10, "NoAlgo", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(11, "FinalORAllParts", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(12, "FinalORPhysPart", 1);
        m_fdlDataEmul_Err[iRec]->setBinLabel(13, "LocalBxNr", 1);

        hName = recString + "FdlDataAlgoDecision_Err";
        histName = hName.c_str();

        m_fdlDataAlgoDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Data: algorithm trigger decision word, non-matching BxInEvent",
                        numberAlgoTriggers, 0., numberAlgoTriggers);

        //
        hName = recString + "Emul_AlgoDecision_Err";
        histName = hName.c_str();

        m_fdlEmulAlgoDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Emul: algorithm trigger decision word, non-matching BxInEvent",
                        numberAlgoTriggers, 0., numberAlgoTriggers);

        hName = recString + "DataEmul_AlgoDecision_Err";
        histName = hName.c_str();

        m_fdlDataEmulAlgoDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Data vs emul: algorithm trigger decision word, non-matching BxInEvent",
                        numberAlgoTriggers, 0., numberAlgoTriggers);

        //
        hName = recString + "Data_TechDecision_Err";
        histName = hName.c_str();

        m_fdlDataTechDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Data: technical trigger decision word, non-matching BxInEvent",
                        numberTechTriggers, 0., numberTechTriggers);

        hName = recString + "Emul_TechDecision_Err";
        histName = hName.c_str();

        m_fdlEmulTechDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Emul: technical trigger decision word, non-matching BxInEvent",
                        numberTechTriggers, 0., numberTechTriggers);

        hName = recString + "DataEmul_TechDecision_Err";
        histName = hName.c_str();

        m_fdlDataEmulTechDecision_Err[iRec]
                = m_dbe->book1D(
                        histName,
                        "Data vs emul: technical trigger decision word, non-matching BxInEvent",
                        numberTechTriggers, 0., numberTechTriggers);

    }
}

void L1GtHwValidation::endRun(const edm::Run& run,
        const edm::EventSetup& evSetup) {

    LogDebug("L1GtHwValidation") << "\n\n endRun: " << run.id()
            << "\n  Number of events analyzed in this run:       " << m_nrEvRun
            << "\n  Total number of events analyzed in this job: " << m_nrEvJob
            << "\n" << std::endl;

}

// method called once each job just after ending the event loop
void L1GtHwValidation::endJob() {

    edm::LogInfo("L1GtHwValidation")
            << "\n\nTotal number of events analyzed in this job: " << m_nrEvJob
            << "\n" << std::endl;

    return;

}

//define this as a plug-in
DEFINE_FWK_MODULE( L1GtHwValidation);
