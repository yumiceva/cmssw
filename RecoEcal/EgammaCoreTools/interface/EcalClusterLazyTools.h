#ifndef RecoEcal_EgammaCoreTools_EcalClusterLazyTools_h
#define RecoEcal_EgammaCoreTools_EcalClusterLazyTools_h

/** \class EcalClusterLazyTools
 *  
 * various cluster tools (e.g. cluster shapes)
 *
 * \author Federico Ferri
 * 
 * \version $Id: 
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"


class CaloTopology;
class CaloGeometry;
class CaloSubdetectorTopology;

class EcalClusterLazyTools {
        public:
       EcalClusterLazyTools( const edm::Event &ev, const edm::EventSetup &es, edm::InputTag redEBRecHits, edm::InputTag redEERecHits,const edm::ParameterSet& config );
       EcalClusterLazyTools( const edm::Event &ev, const edm::EventSetup &es, edm::InputTag redEBRecHits, edm::InputTag redEERecHits);
       EcalClusterLazyTools( const edm::Event &ev, const edm::EventSetup &es, edm::InputTag redEBRecHits, edm::InputTag redEERecHits, edm::InputTag redESRecHits);

                ~EcalClusterLazyTools();

                // various energies in the matrix nxn surrounding the maximum energy crystal of the input cluster  
                //NOTE (29/10/08): we now use an eta/phi coordinate system rather than phi/eta
                //to minmise possible screwups, for now e5x1 isnt defined all the majority of people who call it actually want e1x5 and 
                //it is thought it is better that their code doesnt compile rather than pick up the wrong function
                //therefore in this version and later e1x5 = e5x1 in the old version 
                //so 1x5 is 1 crystal in eta and 5 crystals in phi
                //note e3x2 does not have a definate eta/phi geometry, it takes the maximum 3x2 block containing the 
                //seed regardless of whether that 3 in eta or phi
                float e1x3( const reco::BasicCluster &cluster );
                float e1x3( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e3x1( const reco::BasicCluster &cluster );
                float e3x1( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e1x5( const reco::BasicCluster &cluster );
                float e1x5( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e5x1( const reco::BasicCluster &cluster );
                float e5x1( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e2x2( const reco::BasicCluster &cluster );
                float e2x2( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e3x2( const reco::BasicCluster &cluster );
                float e3x2( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e3x3( const reco::BasicCluster &cluster );
                float e3x3( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e4x4( const reco::BasicCluster &cluster );
                float e4x4( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float e5x5( const reco::BasicCluster &cluster );
                float e5x5( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // energy in the 2x5 strip right of the max crystal (does not contain max crystal)
                // 2 crystals wide in eta, 5 wide in phi.
                float e2x5Right( const reco::BasicCluster &cluster );
                float e2x5Right( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // energy in the 2x5 strip left of the max crystal (does not contain max crystal)
                float e2x5Left( const reco::BasicCluster &cluster );
                float e2x5Left( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // energy in the 5x2 strip above the max crystal (does not contain max crystal)
                // 5 crystals wide in eta, 2 wide in phi.
                float e2x5Top( const reco::BasicCluster &cluster );
		float e2x5Top( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                // energy in the 5x2 strip below the max crystal (does not contain max crystal)
                float e2x5Bottom( const reco::BasicCluster &cluster );
		float e2x5Bottom( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // energy in a 2x5 strip containing the seed (max) crystal.
                // 2 crystals wide in eta, 5 wide in phi.
                // it is the maximum of either (1x5left + 1x5center) or (1x5right + 1x5center)
                float e2x5Max( const reco::BasicCluster &cluster );
		float e2x5Max( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // energies in the crystal left, right, top, bottom w.r.t. to the most energetic crystal
                float eLeft( const reco::BasicCluster &cluster );
		float eLeft( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float eRight( const reco::BasicCluster &cluster );
		float eRight( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float eTop( const reco::BasicCluster &cluster );
		float eTop( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                float eBottom( const reco::BasicCluster &cluster );
		float eBottom( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // the energy of the most energetic crystal in the cluster
                float eMax( const reco::BasicCluster &cluster );
		float eMax( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );
                // the energy of the second most energetic crystal in the cluster
                float e2nd( const reco::BasicCluster &cluster );
		float e2nd( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                // get the DetId and the energy of the maximum energy crystal of the input cluster
                std::pair<DetId, float> getMaximum( const reco::BasicCluster &cluster );
		std::pair<DetId, float> getMaximum( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                std::vector<float> energyBasketFractionEta( const reco::BasicCluster &cluster );
                std::vector<float> energyBasketFractionEta( const reco::BasicCluster &cluster,std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv  );

                std::vector<float> energyBasketFractionPhi( const reco::BasicCluster &cluster );
                std::vector<float> energyBasketFractionPhi( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

                // return a vector v with v[0] = etaLat, v[1] = phiLat, v[2] = lat
                std::vector<float> lat( const reco::BasicCluster &cluster, bool logW = true, float w0 = 4.7 );
                std::vector<float> lat( const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv, bool logW = true, float w0 = 4.7 );

                // return a vector v with v[0] = covEtaEta, v[1] = covEtaPhi, v[2] = covPhiPhi
                std::vector<float> covariances(const reco::BasicCluster &cluster, float w0 = 4.7 );
                std::vector<float> covariances(const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv, float w0 = 4.7 );

                // return a vector v with v[0] = covIEtaIEta, v[1] = covIEtaIPhi, v[2] = covIPhiIPhi
                //this function calculates differences in eta/phi in units of crystals not global eta/phi
                //this is gives better performance in the crack regions of the calorimeter but gives otherwise identical results to covariances function
                //this is only defined for the barrel, it returns covariances when the cluster is in the endcap
                //Warning: covIEtaIEta has been studied by egamma, but so far covIPhiIPhi hasnt been studied extensively so there could be a bug in 
                //         the covIPhiIEta or covIPhiIPhi calculations. I dont think there is but as it hasnt been heavily used, there might be one
                std::vector<float> localCovariances(const reco::BasicCluster &cluster, float w0 = 4.7);
                std::vector<float> localCovariances(const reco::BasicCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv, float w0 = 4.7);

                std::vector<float> scLocalCovariances(const reco::SuperCluster &cluster, float w0 = 4.7);
                std::vector<float> scLocalCovariances(const reco::SuperCluster &cluster, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv, float w0 = 4.7);

                double zernike20( const reco::BasicCluster &cluster, double R0 = 6.6, bool logW = true, float w0 = 4.7 );
                double zernike42( const reco::BasicCluster &cluster, double R0 = 6.6, bool logW = true, float w0 = 4.7 );

                // get the detId's of a matrix centered in the maximum energy crystal = (0,0)
                // the size is specified by ixMin, ixMax, iyMin, iyMax in unit of crystals
                std::vector<DetId> matrixDetId( DetId id, int ixMin, int ixMax, int iyMin, int iyMax );
                // get the energy deposited in a matrix centered in the maximum energy crystal = (0,0)
                // the size is specified by ixMin, ixMax, iyMin, iyMax in unit of crystals
                float matrixEnergy( const reco::BasicCluster &cluster, DetId id, int ixMin, int ixMax, int iyMin, int iyMax );
                float matrixEnergy( const reco::BasicCluster &cluster, DetId id, int ixMin, int ixMax, int iyMin, int iyMax, std::vector<int> flagsexcl, std::vector<int> severitiesexcl, const EcalSeverityLevelAlgo *sevLv );

		// get time of basic cluster seed crystal 
		float BasicClusterSeedTime(const reco::BasicCluster &cluster);
		// error-weighted average of time from constituents of basic cluster 
		float BasicClusterTime(const reco::BasicCluster &cluster, const edm::Event &ev);
		// get BasicClusterSeedTime of the seed basic cluser of the supercluster
		float SuperClusterSeedTime(const reco::SuperCluster &cluster);
		// get BasicClusterTime of the seed basic cluser of the supercluster
		float SuperClusterTime(const reco::SuperCluster &cluster, const edm::Event &ev);

		// mapping for preshower rechits
		std::map<DetId, EcalRecHit> rechits_map_;
		// get Preshower hit array
		std::vector<float> getESHits(double X, double Y, double Z, std::map<DetId, EcalRecHit> rechits_map, const CaloGeometry* geometry, CaloSubdetectorTopology *topology_p, int row=0, int plane=1);
		// get Preshower hit shape
		float getESShape(std::vector<float> ESHits0);
		// get Preshower effective sigmaRR
		float eseffsirir( const reco::SuperCluster &cluster );
		float eseffsixix( const reco::SuperCluster &cluster );
		float eseffsiyiy( const reco::SuperCluster &cluster );

//  std::vector<int> flagsexcl_;
  //std::vector<int> severitiesexcl_;
 // const EcalSeverityLevelAlgo *sevLv;

        private:
                void getGeometry( const edm::EventSetup &es );
                void getTopology( const edm::EventSetup &es );
                void getEBRecHits( const edm::Event &ev, edm::InputTag redEBRecHits );
                void getEERecHits( const edm::Event &ev, edm::InputTag redEERecHits );
                void getESRecHits( const edm::Event &ev, edm::InputTag redESRecHits );
                const EcalRecHitCollection * getEcalRecHitCollection( const reco::BasicCluster &cluster );

                const CaloGeometry *geometry_;
                const CaloTopology *topology_;
                const EcalRecHitCollection *ebRecHits_;
                const EcalRecHitCollection *eeRecHits_;
                const EcalRecHitCollection *esRecHits_;
		
		//const EcalIntercalibConstantMap& icalMap;
		edm::ESHandle<EcalIntercalibConstants> ical;
		EcalIntercalibConstantMap        icalMap;
		edm::ESHandle<EcalADCToGeVConstant>    agc;
		edm::ESHandle<EcalLaserDbService>      laser;
                void getIntercalibConstants( const edm::EventSetup &es );
                void getADCToGeV           ( const edm::EventSetup &es );
                void getLaserDbService     ( const edm::EventSetup &es );

//  std::vector<int> flagsexcl_;
//  std::vector<int> severitiesexcl_;
		
};

#endif
