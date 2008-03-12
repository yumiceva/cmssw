#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/Hcal/interface/HcalChannelQualityHandler.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//typedef popcon::PopConAnalyzer<HcalChannelQualityHandler> HcalChannelQualityPopConAnalyzer;

class HcalChannelQualityPopConAnalyzer: public popcon::PopConAnalyzer<HcalChannelQualityHandler>
{
public:
  typedef HcalChannelQualityHandler SourceHandler;

  HcalChannelQualityPopConAnalyzer(const edm::ParameterSet& pset): 
    popcon::PopConAnalyzer<HcalChannelQualityHandler>(pset),
    m_populator(pset),
    m_source(pset.getParameter<edm::ParameterSet>("Source")) {}

private:
  virtual void endJob() 
  {
    m_source.initObject(myDBObject);
    write();
  }

  virtual void analyze(const edm::Event& ev, const edm::EventSetup& esetup)
  {
    //Using ES to get the data:

    edm::ESHandle<HcalChannelQuality> objecthandle;
    esetup.get<HcalChannelQualityRcd>().get(objecthandle);
    myDBObject = new HcalChannelQuality(*objecthandle.product() );
  }

  void write() { m_populator.write(m_source); }

private:
  popcon::PopCon m_populator;
  SourceHandler m_source;

  HcalChannelQuality* myDBObject;
};

DEFINE_FWK_MODULE(HcalChannelQualityPopConAnalyzer);
