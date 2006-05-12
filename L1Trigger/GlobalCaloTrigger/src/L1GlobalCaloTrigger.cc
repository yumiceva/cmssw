#include "L1Trigger/GlobalCaloTrigger/interface/L1GlobalCaloTrigger.h"

#include "L1Trigger/GlobalCaloTrigger/interface/L1GctSourceCard.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJetLeafCard.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctEmLeafCard.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctWheelJetFpga.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctWheelEnergyFpga.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJetFinalStage.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctGlobalEnergyAlgos.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctElectronFinalSort.h"


L1GlobalCaloTrigger* L1GlobalCaloTrigger::instance = 0;

// constructor
L1GlobalCaloTrigger::L1GlobalCaloTrigger() :
  theSourceCards(54),
	theJetLeafCards(6),
  theEmLeafCards(2),
	theWheelJetFpgas(2),
	theWheelEnergyFpgas(2)
{
  
  build();
  setup();
  
}

L1GlobalCaloTrigger::~L1GlobalCaloTrigger()
{
  theSourceCards.clear();
}

L1GlobalCaloTrigger* L1GlobalCaloTrigger::theGct() {

  if (L1GlobalCaloTrigger::instance==0) {
    L1GlobalCaloTrigger::instance = new L1GlobalCaloTrigger();
  }
  return L1GlobalCaloTrigger::instance;

}

// instantiate hardware/algorithms
void L1GlobalCaloTrigger::build() {

  for (int i=0; i<18; i++) {
    theSourceCards[3*i] = new L1GctSourceCard(3*i, L1GctSourceCard::cardType1);
    theSourceCards[3*i+1] = new L1GctSourceCard(3*i+1, L1GctSourceCard::cardType2);
    theSourceCards[3*i+2] = new L1GctSourceCard(3*i+2, L1GctSourceCard::cardType3);
  }

  for (int i=0; i<6; i++) {
    theJetLeafCards[i] = new L1GctJetLeafCard(i, i % 3);
  }
  
  for (int i=0; i<2; i++) {
    theEmLeafCards[i] = new L1GctEmLeafCard(i);
  }
    
  for (int i=0; i<2; i++) {
    theWheelJetFpgas[i] = new L1GctWheelJetFpga(i);
    theWheelEnergyFpgas[i] = new L1GctWheelEnergyFpga(i);
  }
  
  theJetFinalStage = new L1GctJetFinalStage();
  theEnergyFinalStage = new L1GctGlobalEnergyAlgos();

  theIsoEmFinalStage = new L1GctElectronFinalSort(true);
  theNonIsoEmFinalStage = new L1GctElectronFinalSort(false);

}

// wire up the hardware/algos
void L1GlobalCaloTrigger::setup() {

  // EM leaf cards
  for (int i=0; i<2; i++) {
    for (int j=0; j<9; j++) {
      theEmLeafCards[i]->setInputSourceCard(j, theSourceCards[i*9+j]);
    }
  }

  // Jet Leaf cards
  for (int i=0; i<6; i++) {
    for (int j=0; j<3; j++) {
      theJetLeafCards[i]->setInputSourceCard(j, theSourceCards[i*6+j]);
    }
  }
  
  // Wheel Fpgas
  for (int i=0; i<2; i++) {
    for (int j=0; j<3; j++) {
      theWheelJetFpgas[i]->setInputLeafCard(j, theJetLeafCards[i*2+j]);
      theWheelEnergyFpgas[i]->setInputLeafCard(j, theJetLeafCards[i*2+j]);
    }
  }

  // Electron Final Sort
  for (int i=0; i<2; i++) {
    theIsoEmFinalStage->setInputLeafCard(i, theEmLeafCards[i]);
    theNonIsoEmFinalStage->setInputLeafCard(i, theEmLeafCards[i]);
  }

  // Jet Final Stage
  for (int i=0; i<2; i++) {
    theJetFinalStage->setInputWheelJetFpga(i, theWheelJetFpgas[i]);
  }

  // Global Energy Algos
  theEnergyFinalStage->setMinusWheelEnergyFpga(theWheelEnergyFpgas[0]);
  theEnergyFinalStage->setPlusWheelEnergyFpga(theWheelEnergyFpgas[1]);
  theEnergyFinalStage->setMinusWheelJetFpga(theWheelJetFpgas[0]);
  theEnergyFinalStage->setPlusWheelJetFpga(theWheelJetFpgas[1]);
  theEnergyFinalStage->setJetFinalStage(theJetFinalStage);

  //  for (int i=0; i<2; i++) {
  //    theEnergyFinalStage->setInputWheelEnergyFpga(i, theWheelEnergyFpgas[i]);
  //    theEnergyFinalStage->setInputJetFinalStage(i, thejetFinalStage);
  //  }

}

void L1GlobalCaloTrigger::process() {
		
  // Source cards
  for (int i=0; i<54; i++) {
    theSourceCards[i]->fetchInput();
  }

  // EM Leaf Card
  for (int i=0; i<4; i++) {
    theEmLeafCards[i]->fetchInput();
    theEmLeafCards[i]->process();
  }

  // Jet Leaf cards
  for (int i=0; i<6; i++) {
    theJetLeafCards[i]->fetchInput();
    theJetLeafCards[i]->process();
  }

  // Wheel Cards
  for (int i=0; i<2; i++) {
    theWheelJetFpgas[i]->fetchInput();
    theWheelJetFpgas[i]->process();
  }

  for (int i=0; i<2; i++) {
    theWheelEnergyFpgas[i]->fetchInput();
    theWheelEnergyFpgas[i]->process();
  }

  // Electron Final Stage
  theIsoEmFinalStage->fetchInput();
  theIsoEmFinalStage->process();

  theNonIsoEmFinalStage->fetchInput();
  theNonIsoEmFinalStage->process();


  // Jet Final Stage
  theJetFinalStage->fetchInput();
  theJetFinalStage->process();

  // Energy Final Stage
  theEnergyFinalStage->fetchInput();
  theEnergyFinalStage->process();
  
	
}

void L1GlobalCaloTrigger::print() {

	cout << "===Global Calo Trigger===" << endl;
	cout << "------Debug Output-------" << endl;
	cout << endl;
	cout << "N Source Cards " << theSourceCards.size() << endl;
	cout << "N Jet Leaf Cards " << theJetLeafCards.size() << endl;
	cout << "N Wheel Jet Fpgas " << theWheelJetFpgas.size() << endl;
	cout << "N Wheel Energy Fpgas " << theWheelEnergyFpgas.size() << endl;
	cout << "N Em Leaf Cards" << theEmLeafCards.size() << endl;
	cout << endl;
	for (int i=0; i<theSourceCards.size(); i++) {
		cout << theSourceCards[i];
	}
	for (int i=0; i<theJetLeafCards.size(); i++) {
		cout << theJetLeafCards[i];
	}
	for (int i=0; i<theWheelJetFpgas.size(); i++) {
		cout << theWheelJetFpgas[i];
	}
	for (int i=0; i<theWheelEnergyFpgas.size(); i++) {
		cout << theWheelEnergyFpgas[i];
	}
	cout << theJetFinalStage;
	cout << theEnergyFinalStage;
	cout << theIsoEmFinalStage;
	cout << theNonIsoEmFinalStage;
	cout << "===Global Calo Trigger===" << endl;
	cout << "-----End Debug Output----" << endl;

}
