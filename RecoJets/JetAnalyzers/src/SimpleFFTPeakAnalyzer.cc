// -*- C++ -*-
//
// Package:    SimpleFFTPeakAnalyzer
// Class:      SimpleFFTPeakAnalyzer
// 
/**\class SimpleFFTPeakAnalyzer SimpleFFTPeakAnalyzer.cc RecoJets/JetAnalyzers/src/SimpleFFTPeakAnalyzer.cc

 Description: dumps the info created by FFTJetProducer into a root ntuple

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Igor Volobouev
//         Created:  Mon Jul 16 22:09:23 CDT 2012
// $Id: SimpleFFTPeakAnalyzer.cc,v 1.1 2012/07/16 17:40:54 igv Exp $
//
//

#include <cmath>
#include <cfloat>
#include <cassert>
#include <algorithm>

#include "fftjet/EquidistantSequence.hh"
#include "fftjet/ProximityClusteringTree.hh"
#include "fftjet/PeakEtaPhiDistance.hh"
#include "fftjet/SparseClusteringTree.hh"

#include "TNtuple.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Histograms/interface/MEtoEDMFormat.h"
#include "RecoJets/FFTJetAlgorithms/interface/fftjetTypedefs.h"
#include "RecoJets/FFTJetAlgorithms/interface/clusteringTreeConverters.h"
#include "RecoJets/FFTJetAlgorithms/interface/jetConverters.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"


#define init_param(type, varname) varname (ps.getParameter< type >( #varname ))


static std::auto_ptr<std::vector<double> >
scale_set_parser(const edm::ParameterSet& ps)
{
    typedef std::auto_ptr<std::vector<double> > return_type;

    const std::string className = ps.getParameter<std::string>("Class");

    if (!className.compare("EquidistantInLinearSpace") ||
        !className.compare("EquidistantInLogSpace"))
    {
        const double minScale = ps.getParameter<double>("minScale");
        const double maxScale = ps.getParameter<double>("maxScale");
        const unsigned nScales = ps.getParameter<unsigned>("nScales");

        if (minScale <= 0.0 || maxScale <= 0.0 ||
            nScales == 0 || minScale == maxScale)
            return return_type(NULL);

        // Can't return pointers to EquidistantInLinearSpace
        // or EquidistantInLogSpace directly because std::vector
        // destructor is not virtual.
        if (!className.compare("EquidistantInLinearSpace"))
            return return_type(new std::vector<double>(
                                   fftjet::EquidistantInLinearSpace(
                                       minScale, maxScale, nScales)));
        else
            return return_type(new std::vector<double>(
                                   fftjet::EquidistantInLogSpace(
                                       minScale, maxScale, nScales)));
    }

    if (!className.compare("UserSet"))
    {
        return_type scales(new std::vector<double>(
            ps.getParameter<std::vector<double> >("scales")));

        // Verify that all scales are positive and unique
        const unsigned nscales = scales->size();
        for (unsigned i=0; i<nscales; ++i)
            if ((*scales)[i] <= 0.0)
                return return_type(NULL);

        for (unsigned i=1; i<nscales; ++i)
            for (unsigned j=0; j<i; ++j)
                if ((*scales)[i] == (*scales)[j])
                    return return_type(NULL);

        return scales;
    }

    return return_type(NULL);
}


//
// class declaration
//
class SimpleFFTPeakAnalyzer : public edm::EDAnalyzer
{
public:
    typedef fftjet::SparseClusteringTree<fftjet::Peak,long> SparseTree;
    typedef fftjet::ProximityClusteringTree<fftjet::Peak,long> ClusteringTree;
    typedef reco::PattRecoTree<float,reco::PattRecoPeak<float> > StoredTree;

    explicit SimpleFFTPeakAnalyzer(const edm::ParameterSet&);
    ~SimpleFFTPeakAnalyzer();

private:
    SimpleFFTPeakAnalyzer();
    SimpleFFTPeakAnalyzer(const SimpleFFTPeakAnalyzer&);
    SimpleFFTPeakAnalyzer& operator=(const SimpleFFTPeakAnalyzer&);

    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    inline double peakPt(const fftjet::Peak& peak) const
    {
        const double s = peak.scale();
        return ptConversionFactor*s*s*peak.magnitude();
    }

    edm::InputTag treeLabel;
    std::string ntupleTitle;
    double ptConversionFactor;
    double minNtupleScale;
    double maxNtupleScale;

    // The bandwidth ratio for the distance calculator
    double etaToPhiBandwidthRatio;

    // Scales used
    std::auto_ptr<std::vector<double> > iniScales;

    // The clustering tree variables
    fftjet::PeakEtaPhiDistance distanceCalc;
    ClusteringTree clusTree;    
    SparseTree sparseTree;

    // Output ntuple
    TNtuple* nt;

    unsigned long counter;
    unsigned numEventVariables;

    std::vector<float> ntupleData;

    // Vector for storing clusters on the given tree level
    std::vector<fftjet::Peak> peaks;
    std::vector<unsigned> sparNodes;

    template<class Tree>
    void processClusteringTree(const Tree& tree);

    void fillPeakVector(const SparseTree& tree, unsigned level);
    void fillPeakVector(const ClusteringTree& tree, unsigned level);
};

//
// constructors and destructor
//
SimpleFFTPeakAnalyzer::SimpleFFTPeakAnalyzer(const edm::ParameterSet& ps)
    : init_param(edm::InputTag, treeLabel),
      init_param(std::string, ntupleTitle),
      init_param(double, ptConversionFactor),
      init_param(double, minNtupleScale),
      init_param(double, maxNtupleScale),
      init_param(double, etaToPhiBandwidthRatio),
      distanceCalc(etaToPhiBandwidthRatio),
      clusTree(&distanceCalc),
      nt(0),
      counter(0),
      numEventVariables(0)
{
    iniScales = scale_set_parser(
        ps.getParameter<edm::ParameterSet>("InitialScales"));
    if (iniScales.get() == 0)
        throw cms::Exception("FFTJetBadConfig") 
            << "Invalid configuration of the clustering tree scales"
            << std::endl;
    std::sort(iniScales->begin(), iniScales->end(), std::greater<double>());
}


SimpleFFTPeakAnalyzer::~SimpleFFTPeakAnalyzer()
{
}


// ------------ method called once each job just before starting event loop
void SimpleFFTPeakAnalyzer::beginJob()
{
    // Generic variables which identify the event
    std::string vars = "cnt:run:event";

    numEventVariables = std::count(vars.begin(), vars.end(), ':') + 1U;

    // Peak-related variables
    vars += ":level:peakNumber:nPeaks:peakEta:peakPhi:peakMagnitude:peakPt:peakDriftSpeed:peakMagSpeed:peakLifetime:peakScale:peakNearestNeighborDistance:peakClusterRadius:peakClusterSeparation:hessDet:hessLaplacian:hessEtaPhiRatio:hessEigenRatio:peakNearestNeighborDPhi:peakNearestNeighborPt";

    ntupleData.reserve(std::count(vars.begin(), vars.end(), ':') + 1U);

    // Book the ntuple
    edm::Service<TFileService> fs;
    nt = fs->make<TNtuple>(
        ntupleTitle.c_str(), ntupleTitle.c_str(), vars.c_str());
}


void SimpleFFTPeakAnalyzer::fillPeakVector(const SparseTree& tree, unsigned level)
{
    tree.getLevelNodes(level, &sparNodes);
    const unsigned len = sparNodes.size();
    peaks.clear();
    peaks.reserve(len);
    for (unsigned i=0; i<len; ++i)
        peaks.push_back(tree.uncheckedNode(sparNodes[i]).getCluster());
}


void SimpleFFTPeakAnalyzer::fillPeakVector(const ClusteringTree& tree, unsigned ilev)
{
    double actualScale = 0.0;
    long dummyInfo;
    tree.getLevelData(ilev, &actualScale, &peaks, &dummyInfo);
}


template<class Tree>
void SimpleFFTPeakAnalyzer::processClusteringTree(
    const Tree& clusteringTree)
{
    // Cycle over all tree levels except level 0
    const unsigned nLevels = clusteringTree.nLevels();
    for (unsigned ilev=1; ilev<nLevels; ++ilev)
    {
        const double levelScale = clusteringTree.getScale(ilev);
        if (levelScale >= minNtupleScale && levelScale <= maxNtupleScale)
        {
            // Process this level
            fillPeakVector(clusteringTree, ilev);
            std::sort(peaks.begin(), peaks.end(), std::greater<fftjet::Peak>());
            const unsigned nPeaks = peaks.size();
            for (unsigned ipeak=0; ipeak<nPeaks; ++ipeak)
            {
                // Process this peak
                ntupleData.erase(ntupleData.begin()+numEventVariables, ntupleData.end());

                const fftjet::Peak& peak(peaks[ipeak]);
                const double peakScale = peak.scale();
                const double hessDet = peak.hessianDeterminant();

                ntupleData.push_back(ilev);
                ntupleData.push_back(ipeak);
                ntupleData.push_back(nPeaks);
                ntupleData.push_back(peak.eta());
                ntupleData.push_back(peak.phi());
                ntupleData.push_back(peak.magnitude());
                ntupleData.push_back(peakPt(peak));
                ntupleData.push_back(peak.driftSpeed());
                ntupleData.push_back(peak.magSpeed());
                ntupleData.push_back(peak.lifetime());
                ntupleData.push_back(peakScale);
                ntupleData.push_back(peak.nearestNeighborDistance());
                ntupleData.push_back(peak.clusterRadius());
                ntupleData.push_back(peak.clusterSeparation());
                ntupleData.push_back(hessDet);
                ntupleData.push_back(-peak.laplacian());

                // Use hessian to determine some peak shape characteristics
                double hess[3];
                peak.hessian(hess);
                if (hess[0])
                    // The following quantity approximates sigma_eta/sigma_phi squared
                    ntupleData.push_back(hess[2]/hess[0]);
                else if (hess[2])
                    ntupleData.push_back(FLT_MAX);
                else
                    ntupleData.push_back(-1.f);

                // min/max ratio of hessian eigenvalues
                const double hDelta = hess[2] - hess[0];
                const double eigen1 = (hess[0] + hess[2] - 
                                       sqrt(4.0*hess[1]*hess[1] + hDelta*hDelta))/2.0;
                if (eigen1)
                {
                    const double eigen2 = hessDet/eigen1;
                    const double emin = std::min(fabs(eigen1), fabs(eigen2));
                    const double emax = std::max(fabs(eigen1), fabs(eigen2));
                    ntupleData.push_back(emin/emax);
                }
                else if (hessDet)
                    ntupleData.push_back(0.f);
                else
                    ntupleData.push_back(-1.f);

                double nearestNeighborDPhi = 4.0;
                double nearestNeighborPt = -10.0;
                if (nPeaks > 1U)
                {
                    double nearestNeighborDist = DBL_MAX;
                    for (unsigned i=0; i<nPeaks; ++i)
                        if (i != ipeak)
                        {
                            const fftjet::Peak& neighbor(peaks[i]);
                            const double nScale = neighbor.scale();
                            const double d = distanceCalc(
                                peakScale, peak, nScale, neighbor);
                            if (d < nearestNeighborDist)
                            {
                                nearestNeighborDist = d;
                                nearestNeighborDPhi = reco::deltaPhi(
                                    peak.phi(), neighbor.phi());
                                nearestNeighborPt = peakPt(neighbor);
                            }
                        }
                }
                ntupleData.push_back(nearestNeighborDPhi);
                ntupleData.push_back(nearestNeighborPt);

                assert(ntupleData.size() == static_cast<unsigned>(nt->GetNvar()));
                nt->Fill(&ntupleData[0]);
            }
        }
    }
}


// ------------ method called to for each event  ------------
void SimpleFFTPeakAnalyzer::analyze(const edm::Event& iEvent,
                                    const edm::EventSetup& iSetup)
{
    ntupleData.clear();
    ntupleData.push_back(counter);

    const long runnumber = iEvent.id().run();
    const long eventnumber = iEvent.id().event();
    ntupleData.push_back(runnumber);
    ntupleData.push_back(eventnumber);

    // Load the clustering tree. We assume that
    // the complete event was not appended at the
    // lowest level.
    edm::Handle<StoredTree> input;
    iEvent.getByLabel(treeLabel, input);

    // Convert into normal FFTJet clustering tree
    if (input->isSparse())
    {
        fftjetcms::sparsePeakTreeFromStorable(
            *input, iniScales.get(), 0.0, &sparseTree);
        sparseTree.sortNodes();
        processClusteringTree(sparseTree);
    }
    else
    {
        fftjetcms::densePeakTreeFromStorable(
            *input, iniScales.get(), 0.0, &clusTree);
        processClusteringTree(clusTree);
    }

    ++counter;
}


// ------------ method called once each job just after ending the event loop
void SimpleFFTPeakAnalyzer::endJob()
{
}


//define this as a plug-in
DEFINE_FWK_MODULE(SimpleFFTPeakAnalyzer);
