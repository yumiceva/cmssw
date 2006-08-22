// -*- C++ -*-
//
// Package:     SiStripChannelChargeFilter
// Class  :     ClusterMTCCFilter
// 
//
// Original Author:  dkcira


#include "EventFilter/SiStripChannelChargeFilter/interface/ClusterMTCCFilter.h"
#include "FWCore/Framework/interface/Handle.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace cms
{

ClusterMTCCFilter::ClusterMTCCFilter(const edm::ParameterSet& ps){
   //
   ModulesToBeExcluded.clear();
   ModulesToBeExcluded = ps.getParameter< std::vector<unsigned> >("ModulesToBeExcluded");
//   edm::LogInfo("ClusterMTCCFilter")<<"Clusters from "<<ModulesToBeExcluded.size()<<" modules will be ignored in the filter:";
   std::cout<<"ClusterMTCCFilter"<<" Clusters from "<<ModulesToBeExcluded.size()<<" modules will be ignored in the filter:"<<std::endl;
   for( std::vector<uint32_t>::const_iterator imod = ModulesToBeExcluded.begin(); imod != ModulesToBeExcluded.end(); imod++){
//     edm::LogInfo("ClusterMTCCFilter")<< *imod;
     std::cout<< *imod<<std::endl;
   }
   //
   ChargeThresholdTIB=ps.getParameter<int>("ChargeThresholdTIB");
   ChargeThresholdTOB=ps.getParameter<int>("ChargeThresholdTOB");
   ChargeThresholdTEC=ps.getParameter<int>("ChargeThresholdTEC");
   MinClustersDiffComponents=ps.getParameter<int>("MinClustersDiffComponents");
   clusterProducer = ps.getParameter<string>("ClusterProducer");
   //
   produces <int>();
   produces <unsigned int >();
   produces < map<uint,vector<SiStripCluster> > >();
}

bool ClusterMTCCFilter::filter(edm::Event & e, edm::EventSetup const& c) {
  //get SiStripCluster
  edm::Handle< edm::DetSetVector<SiStripCluster> > h;
  e.getByLabel(clusterProducer,h);


  //
  unsigned int sum_of_cluster_charges=0;
  clusters_in_subcomponents.clear();
  // first find all clusters that are over the threshold
  for (edm::DetSetVector<SiStripCluster>::const_iterator it=h->begin();it!=h->end();it++) {
    for(vector<SiStripCluster>::const_iterator vit=(it->data).begin(); vit!=(it->data).end(); vit++){
      // calculate sum of amplitudes
      unsigned int amplclus=0;
      for(vector<short>::const_iterator ia=vit->amplitudes().begin(); ia!=vit->amplitudes().end(); ia++) {
        if ((*ia)>0) amplclus+=(*ia); // why should this be negative?
      }
      sum_of_cluster_charges += amplclus;
      DetId thedetId = DetId(it->detId());
      uint generalized_layer = 0;
      bool exclude_this_detid = false;
      for( std::vector<uint32_t>::const_iterator imod = ModulesToBeExcluded.begin(); imod != ModulesToBeExcluded.end(); imod++ ){
          if(*imod == thedetId.rawId()) exclude_this_detid = true; // found in exclusion list
      }
      // apply different thresholds for TIB/TOB/TEC
      if( ! exclude_this_detid ){ // only consider if not in exclusion list
        if ( ( thedetId.subdetId()==StripSubdetector::TIB && amplclus>ChargeThresholdTIB )
          || ( thedetId.subdetId()==StripSubdetector::TOB && amplclus>ChargeThresholdTOB )
          || ( thedetId.subdetId()==StripSubdetector::TEC && amplclus>ChargeThresholdTEC )
          ){
          // calculate generalized_layer:  31 = TIB1, 32 = TIB2, 33 = TIB3, 50 = TOB, 60 = TEC
          if(thedetId.subdetId()==StripSubdetector::TIB){
             TIBDetId ptib = TIBDetId(thedetId.rawId());
             generalized_layer = 10*thedetId.subdetId() + ptib.layer() + ptib.stereo();
  	   if (ptib.layer()==2){
  	     generalized_layer++;
  	     if (ptib.glued()) cout<<"WRONGGGG"<<endl;
  	   }
          }else{
            generalized_layer = 10*thedetId.subdetId();
  	  if(thedetId.subdetId()==StripSubdetector::TOB){
  	    TOBDetId ptob = TOBDetId(thedetId.rawId());
  	    generalized_layer += ptob.layer();
  	  }
          }
          // fill clusters_in_subcomponents
          map<uint,vector<SiStripCluster> >::iterator layer_it = clusters_in_subcomponents.find(generalized_layer);
          if(layer_it==clusters_in_subcomponents.end()){ // if layer not found yet, create DATA vector and generate map KEY + DATA
            vector<SiStripCluster> local_vector;
            local_vector.push_back(*vit);
            clusters_in_subcomponents.insert( std::make_pair( generalized_layer, local_vector) );
          }else{ // push into already existing vector
             (layer_it->second).push_back(*vit);
          }
        }
      }
    }
  }

  bool decision=false; // default value, only accept if set true in this loop
  uint nr_of_subcomps_with_clusters=0;
  if( clusters_in_subcomponents[31].size()>0 ) nr_of_subcomps_with_clusters++; // TIB1
  if( clusters_in_subcomponents[32].size()>0 ) nr_of_subcomps_with_clusters++; // TIB2
  if( clusters_in_subcomponents[33].size()>0 ) nr_of_subcomps_with_clusters++; // TIB3
  if( clusters_in_subcomponents[51].size()>0 ) nr_of_subcomps_with_clusters++; // TOB1
  if( clusters_in_subcomponents[52].size()>0 ) nr_of_subcomps_with_clusters++; // TOB2
  if( clusters_in_subcomponents[60].size()>0 ) nr_of_subcomps_with_clusters++; // TEC
  if(
     nr_of_subcomps_with_clusters >= MinClustersDiffComponents // more than 'MinClustersDiffComponents' components have at least 1 cluster
     ) {
      decision = true; // accept event
  }

  std::auto_ptr< int > output_decision( new int(decision) );
  e.put(output_decision);

  std::auto_ptr< unsigned int > output_sumofcharges( new unsigned int(sum_of_cluster_charges) );
  e.put(output_sumofcharges);

  std::auto_ptr< map<uint,vector<SiStripCluster> > > output_clusters(new map<uint,vector<SiStripCluster> > (clusters_in_subcomponents));
  e.put(output_clusters);

  return decision;
}
}
