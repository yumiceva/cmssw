#ifndef SimG4Core_GeometryProducer_H
#define SimG4Core_GeometryProducer_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/EDProduct.h"
 
#include "SimG4Core/SensitiveDetector/interface/AttachSD.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveDetector.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveTkDetector.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveCaloDetector.h"
#include "SimG4Core/Notification/interface/SimActivityRegistry.h"

#include <memory>
#include "boost/shared_ptr.hpp"

namespace sim { class FieldBuilder; }
class SimWatcher;
class SimProducer;
class DDDWorld;
class G4RunManagerKernel;
class SimTrackManager;

class GeometryProducer : public edm::EDProducer
{
public:
    explicit GeometryProducer(edm::ParameterSet const & p);
    virtual ~GeometryProducer();
    virtual void beginJob(const edm::EventSetup & c);
    virtual void endJob();
    virtual void produce(edm::Event & e, const edm::EventSetup & c);
    std::vector<SensitiveTkDetector*>& sensTkDetectors() { return m_sensTkDets; }
    std::vector<SensitiveCaloDetector*>& sensCaloDetectors() { return m_sensCaloDets; }
    std::vector<boost::shared_ptr<SimProducer> > producers() const
    { return m_producers; }
private:
    G4RunManagerKernel * m_kernel;
    bool m_pUseMagneticField;
    edm::ParameterSet m_pField;
    edm::ParameterSet m_p;    
    AttachSD * m_attach;
    std::vector<SensitiveTkDetector*> m_sensTkDets;
    std::vector<SensitiveCaloDetector*> m_sensCaloDets;
    SimActivityRegistry m_registry;
    std::vector<boost::shared_ptr<SimWatcher> > m_watchers;
    std::vector<boost::shared_ptr<SimProducer> > m_producers;    
    std::auto_ptr<SimTrackManager> m_trackManager;
    std::auto_ptr<sim::FieldBuilder> m_fieldBuilder;
};

#endif
