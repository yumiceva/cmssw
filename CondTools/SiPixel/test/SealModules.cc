#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondTools/SiPixel/test/SiPixelCondObjBuilder.h"
#include "CondTools/SiPixel/test/SiPixelCondObjReader.h"

using cms::SiPixelCondObjBuilder;
using cms::SiPixelCondObjReader;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SiPixelCondObjBuilder);
DEFINE_ANOTHER_FWK_MODULE(SiPixelCondObjReader);
