/* \class PixelCountFilter
 *
 * Filters events if at least N pixel clusters
 *
 * \author: Vincenzo Chiochia, Uni-ZH
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountFilter.h"

 typedef ObjectCountFilter<
           SiPixelClusterCollectionNew
         > PixelCountFilter;

DEFINE_FWK_MODULE( PixelCountFilter );
