#ifndef DataFormats_BTauReco_SoftLeptonTagInfo_h
#define DataFormats_BTauReco_SoftLeptonTagInfo_h

#include <vector>

#include "DataFormats/BTauReco/interface/RefMacros.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h" 
#include "DataFormats/BTauReco/interface/JetTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"

namespace reco {
 
class SoftLeptonProperties {
public:
    float sip2d;                            // 2D signed impact parameter
    float sip3d;                            // 3D signed impact parameter
    float ptRel;                            // transverse momentum wrt. the jet axis
    float p0Par;                            // momentum along the jet direction, in the jet rest frame

    float etaRel;                           // (pseudo)rapidity along jet axis
    float deltaR;                           // (pseudo)angular distance to jet axis
    float ratio;                            // momentum over jet energy
    float ratioRel;                         // momentum parallel to jet axis over jet energy

    static const float undefQuality;

    struct quality {
        enum ElectronQuality {
            pfElectronId = 0,
            egammaElectronId,
            btagElectronId
        };

        enum MuonQuality {
            muonId = 0
        };
    };

    // check to see if quality has been set

    inline float hasQuality() const
    { return quality() != undefQuality; }
    inline float hasQuality(quality::MuonQuality qual) const
    { return quality((unsigned int)qual, false) != undefQuality; }
    inline float hasQuality(quality::ElectronQuality qual) const
    { return quality((unsigned int)qual, false) != undefQuality; }

    // retrieve lepton quality

    inline float quality(quality::MuonQuality qual,
                         bool throwIfUndefined = true) const
    { return quality((unsigned int)qual, throwIfUndefined); }
    inline float quality(quality::ElectronQuality qual,
                         bool throwIfUndefined = true) const
    { return quality((unsigned int)qual, throwIfUndefined); }

    // default value
    inline float quality() const { return quality(0, false); }

    // set lepton quality

    inline void setQuality(quality::MuonQuality qual, float value)
    { setQuality((unsigned int)qual, value); }
    inline void setQuality(quality::ElectronQuality qual, float value)
    { setQuality((unsigned int)qual, value); }

private:
    float quality(unsigned int index, bool throwIfUndefined) const;
    void setQuality(unsigned int index, float value);

    std::vector<float> qualities_;          // lepton qualities
};

class SoftLeptonTagInfo : public JetTagInfo {
public:
    typedef std::vector< std::pair< edm::RefToBase<reco::Track>, SoftLeptonProperties > > LeptonMap;
    
    SoftLeptonTagInfo(void) : m_leptons() {}

    virtual ~SoftLeptonTagInfo(void) {}
  
    virtual SoftLeptonTagInfo* clone(void) const { return new SoftLeptonTagInfo(*this); }

    unsigned int leptons(void) const { 
        return m_leptons.size(); 
    } 

    const edm::RefToBase<reco::Track> & lepton(size_t i) const {
        return m_leptons[i].first;
    }
    
    const SoftLeptonProperties & properties(size_t i) const {
        return m_leptons[i].second;
    }

    void insert(const edm::RefToBase<reco::Track> & lepton, const SoftLeptonProperties & properties) {
        m_leptons.push_back( std::pair< edm::RefToBase<reco::Track>, SoftLeptonProperties > (lepton, properties) );
    }

    /// returns a description of the extended informations in a TaggingVariableList
    virtual TaggingVariableList taggingVariables(void) const;

private:
    LeptonMap m_leptons;

};

DECLARE_EDM_REFS( SoftLeptonTagInfo )

}

#endif // DataFormats_BTauReco_SoftLeptonTagInfo_h
