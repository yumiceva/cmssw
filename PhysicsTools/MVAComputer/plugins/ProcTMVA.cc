// -*- C++ -*-
//
// Package:     MVAComputer
// Class  :     ProcTMVA
// 

// Implementation:
//     TMVA wrapper, needs n non-optional, non-multiple input variables
//     and outputs one result variable. All TMVA algorithms can be used,
//     calibration data is passed via stream and extracted from a zipped
//     buffer.
//
// Author:      Christophe Saout
// Created:     Sat Apr 24 15:18 CEST 2007
// $Id: ProcTMVA.cc,v 1.3 2011/02/22 18:40:31 kukartse Exp $
//

#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

// ROOT version magic to support TMVA interface changes in newer ROOT
#include <RVersion.h>

#include <TMVA/Types.h>
#include <TMVA/MethodBase.h>
#include "TMVA/Reader.h"

#include "PhysicsTools/MVAComputer/interface/memstream.h"
#include "PhysicsTools/MVAComputer/interface/zstream.h"

#include "PhysicsTools/MVAComputer/interface/VarProcessor.h"
#include "PhysicsTools/MVAComputer/interface/Calibration.h"

using namespace PhysicsTools;

namespace { // anonymous

class ProcTMVA : public VarProcessor {
    public:
	typedef VarProcessor::Registry::Registry<ProcTMVA,
					Calibration::ProcExternal> Registry;

	ProcTMVA(const char *name,
	         const Calibration::ProcExternal *calib,
	         const MVAComputer *computer);
	virtual ~ProcTMVA() {}

	virtual void configure(ConfIterator iter, unsigned int n);
	virtual void eval(ValueIterator iter, unsigned int n) const;

    private:
  std::auto_ptr<TMVA::Reader>     reader;
  std::auto_ptr<TMVA::MethodBase> method;
  std::string   methodName;
  unsigned int  nVars;

  // FIXME: Gena
  TString   methodName_t;
};

static ProcTMVA::Registry registry("ProcTMVA");

ProcTMVA::ProcTMVA(const char *name,
                   const Calibration::ProcExternal *calib,
                   const MVAComputer *computer) :
	VarProcessor(name, calib, computer)
{

  reader = std::auto_ptr<TMVA::Reader>(new TMVA::Reader( "!Color:!Silent" ));    
  
  ext::imemstream is(
		     reinterpret_cast<const char*>(&calib->store.front()),
		     calib->store.size());
  ext::izstream izs(&is);
  
  std::getline(izs, methodName);
  std::string tmp;
  std::getline(izs, tmp);
  std::istringstream iss(tmp);
  iss >> nVars;
  for(unsigned int i = 0; i < nVars; i++) {
    std::getline(izs, tmp);
    reader->DataInfo().AddVariable(tmp.c_str());
  }
  
  bool isXml = false; // weights in XML (TMVA 4) or plain text
  bool isFirstPass = true;
  TString weight_file_name(tmpnam(0));
  std:: ofstream weight_file;
  //

  std::string weights;
  while (izs.good()) {
    std::string tmp;

    std::getline(izs, tmp);
    if (isFirstPass){
      isFirstPass = false;
      if ( tmp.find("<?xml") != std::string::npos ){ //xml
	isXml = true;
	weights += tmp + " "; 
      }
      else{
	std::cout << std::endl;
	std::cout << "ProcTMVA::ProcTMVA(): *** WARNING! ***" << std::endl;
	std::cout << "ProcTMVA::ProcTMVA(): Old pre-TMVA 4 plain text weights are being loaded" << std::endl;
	std::cout << "ProcTMVA::ProcTMVA(): It may work but backwards compatibility is not guaranteed" << std::endl;
	std::cout << "ProcTMVA::ProcTMVA(): TMVA 4 weight file format is XML" << std::endl;
 	std::cout << "ProcTMVA::ProcTMVA(): Retrain your networks as soon as possible!" << std::endl;
	std::cout << "ProcTMVA::ProcTMVA(): Creating temporary weight file " << weight_file_name << std::endl;
	weight_file.open(weight_file_name.Data());
	weight_file << tmp << std::endl;
      }
    } // end first pass
    else{
      if (isXml){ // xml
	izs >> tmp;
	weights += tmp + " "; 
      }
      else{       // plain text
	weight_file << tmp << std::endl;
      }
    } // end not first pass
    
  }
  if (weight_file.is_open()){
    std::cout << "ProcTMVA::ProcTMVA(): Deleting temporary weight file " << weight_file_name << std::endl;
    weight_file.close();
  }

  TMVA::Types::EMVA methodType =
			  TMVA::Types::Instance().GetMethodType(methodName);

 if (isXml){
   method = std::auto_ptr<TMVA::MethodBase>
     ( dynamic_cast<TMVA::MethodBase*>
       ( reader->BookMVA( methodType, weights.c_str() ) ) ); 
 }
 else{
   methodName_t.Clear();
   methodName_t.Append(methodName.c_str());
   method = std::auto_ptr<TMVA::MethodBase>
     ( dynamic_cast<TMVA::MethodBase*>
       ( reader->BookMVA( methodName_t, weight_file_name ) ) );
 }

}

void ProcTMVA::configure(ConfIterator iter, unsigned int n)
{
	if (n != nVars)
		return;

	for(unsigned int i = 0; i < n; i++)
		iter++(Variable::FLAG_NONE);

	iter << Variable::FLAG_NONE;
}

void ProcTMVA::eval(ValueIterator iter, unsigned int n) const
{
	for(unsigned int i = 0; i < n; i++)
		reader->DataInfo().GetDataSet()->GetEvent()->SetVal(i, *iter++);

  std::cout << "\tMVA Disc: " << method->GetMvaValue() << std::endl; //for testing only
  iter( method->GetMvaValue() );
}

} // anonymous namespace

MVA_COMPUTER_DEFINE_PLUGIN(ProcTMVA);
