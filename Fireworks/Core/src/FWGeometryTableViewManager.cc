// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGeometryTableViewManager
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Fri Jul  8 00:40:37 CEST 2011
// $Id: FWGeometryTableViewManager.cc,v 1.5 2011/08/05 09:38:16 yana Exp $
//

#include <boost/bind.hpp>

#include "TFile.h"
#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"

#include "Fireworks/Core/interface/FWGeometryTableViewManager.h"
#include "Fireworks/Core/interface/FWGeometryTableView.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "Fireworks/Core/interface/FWGUIManager.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/interface/fwLog.h"

TGeoManager* FWGeometryTableViewManager::s_geoManager = 0;

FWGeometryTableViewManager::FWGeometryTableViewManager(FWGUIManager* iGUIMgr, std::string fileName):
   FWViewManagerBase(),
   m_fileName(fileName)
{
   FWGUIManager::ViewBuildFunctor f;
   f=boost::bind(&FWGeometryTableViewManager::buildView, this, _1, _2);                
   iGUIMgr->registerViewBuilder(FWViewType::idToName(FWViewType::kGeometryTable), f);
}

FWGeometryTableViewManager::~FWGeometryTableViewManager()
{ 
}


FWViewBase*
FWGeometryTableViewManager::buildView(TEveWindowSlot* iParent, const std::string& /*type*/)
{
   boost::shared_ptr<FWGeometryTableView> view;
   if (!s_geoManager) initGeoManager();
   view.reset( new FWGeometryTableView(iParent, &colorManager(), (!s_geoManager) ? 0 : s_geoManager->GetTopNode(),  (!s_geoManager) ? 0 : s_geoManager->GetListOfVolumes()));

   view->setBackgroundColor();
   m_views.push_back(boost::shared_ptr<FWGeometryTableView> (view));
   view->beingDestroyed_.connect(boost::bind(&FWGeometryTableViewManager::beingDestroyed, this,_1));
                                            
   return view.get();
}


void
FWGeometryTableViewManager::beingDestroyed(const FWViewBase* iView)
{
   for(std::vector<boost::shared_ptr<FWGeometryTableView> >::iterator it=m_views.begin(); it != m_views.end(); ++it) {
      if(it->get() == iView) {
         m_views.erase(it);
         return;
      }
   }
}

void
FWGeometryTableViewManager::colorsChanged()
{
  for(std::vector<boost::shared_ptr<FWGeometryTableView> >::iterator it=m_views.begin(); it != m_views.end(); ++it)
      (*it)->setBackgroundColor();
}


void
FWGeometryTableViewManager::initGeoManager()
{
   TGeoManager  *old    = gGeoManager;
   TGeoIdentity *old_id = gGeoIdentity;
   gGeoManager = 0;
   
   TFile* file = FWGeometry::findFile( m_fileName.c_str() );

   try 
   {
     if ( ! file )
       throw std::runtime_error("No root file.");
      
     file->ls();
      
     if ( !file->Get("cmsGeo;1"))
       throw std::runtime_error("Can't find TGeoManager object in selected file.");
     s_geoManager = (TGeoManager*) file->Get("cmsGeo;1");      
   }
   catch (std::runtime_error &e)
   {
     fwLog(fwlog::kError) << "Failed to find simulation geomtery file. Please set the file path with --sim-geom-file option.\n";
     exit(0);
   }
   gGeoManager  = old;
   gGeoIdentity = old_id;
}
