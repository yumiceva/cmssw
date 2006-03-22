
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoTracker/RoadSearchHelixMaker/interface/RoadSearchHelixMaker.h"

using cms::RoadSearchHelixMaker;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(RoadSearchHelixMaker)
