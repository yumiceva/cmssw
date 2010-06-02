#ifndef Fireworks_Calo_FWCaloTowerSliceSelector_h
#define Fireworks_Calo_FWCaloTowerSliceSelector_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWCaloTowerSliceSelector
// 
/**\class FWCaloTowerSliceSelector FWCaloTowerSliceSelector.h Fireworks/Calo/interface/FWCaloTowerSliceSelector.h

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Wed Jun  2 19:21:19 CEST 2010
// $Id$
//

// system include files

// user include files

#include "Fireworks/Calo/src/FWFromSliceSelector.h"

// forward declarations

class FWCaloTowerSliceSelector : public FWFromSliceSelector
{
public:
   FWCaloTowerSliceSelector(TH2F* h, const FWEventItem* i) : FWFromSliceSelector(h, i) {}
   virtual ~FWCaloTowerSliceSelector() {}

   virtual void doSelect(const TEveCaloData::CellId_t&);
   virtual void doUnselect(const TEveCaloData::CellId_t&);
};

#endif
