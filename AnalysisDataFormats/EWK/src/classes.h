#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "AnalysisDataFormats/EWK/interface/WMuNuCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/Common/interface/RefHolder.h"
#include "DataFormats/Common/interface/Holder.h"
#include "DataFormats/Common/interface/RefVectorHolder.h"
#include "DataFormats/Common/interface/VectorHolder.h"
#include "DataFormats/Common/interface/BaseVectorHolder.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

#include <vector>
#include <map>


namespace {
  struct dictionary {
     std::vector<reco::WMuNuCandidate> v1;
     edm::Wrapper<std::vector<reco::WMuNuCandidate> > c1;
     edm::Ref<std::vector<reco::WMuNuCandidate> > r1;
     edm::RefProd<std::vector<reco::WMuNuCandidate> > rp1;
     edm::Wrapper<edm::RefVector<std::vector<reco::WMuNuCandidate> > > wrv1;
     edm::helpers::Key<edm::RefProd<std::vector<reco::WMuNuCandidate> > > hkrv1;


     edm::RefToBase<reco::WMuNuCandidate> rtbm;
     edm::reftobase::IndirectHolder<reco::WMuNuCandidate> ihm;
     edm::RefToBaseProd<reco::WMuNuCandidate> rtbpm;
     edm::RefToBaseVector<reco::WMuNuCandidate> rtbvm;
     edm::Wrapper<edm::RefToBaseVector<reco::WMuNuCandidate> > rtbvm_w;
     edm::reftobase::BaseVectorHolder<reco::WMuNuCandidate> *bvhm_p;
 
     
 
  };
}  
