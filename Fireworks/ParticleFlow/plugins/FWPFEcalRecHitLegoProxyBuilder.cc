#include "FWPFEcalRecHitLegoProxyBuilder.h"

//______________________________________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::scaleProduct( TEveElementList *parent, FWViewType::EType type, const FWViewContext *vc )
{
   typedef std::vector<FWPFLegoRecHit*> rh;

   // printf("FWPFEcalRecHitLegoProxyBuilder::scaleProduct >> scale %f \n", caloScale->getValToHeight());
   for( rh::iterator i = m_recHits.begin(); i != m_recHits.end(); ++i )
      (*i)->updateScale( vc);
}

//______________________________________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::localModelChanges( const FWModelId &iId, TEveElement *iCompound,
                                                   FWViewType::EType viewType, const FWViewContext *vc )
{
   const FWDisplayProperties &p = item()->modelInfo(iId.index()).displayProperties();

   TEveStraightLineSet* line = static_cast<TEveStraightLineSet*>(iCompound->FirstChild());
   line->SetMarkerColor(p.color());

   TEveBox* box = static_cast<TEveBox*>(iCompound->LastChild());
   box->SetLineColor(kBlack);
}

//______________________________________________________________________________________________________
TEveVector
FWPFEcalRecHitLegoProxyBuilder::calculateCentre( const std::vector<TEveVector> &corners ) const
{
   TEveVector centre;

   for( size_t i = 0; i < corners.size(); ++i )
   {
      centre.fX += corners[i].fX;
      centre.fY += corners[i].fY;            // Get total for x,y,z values
      centre.fZ += corners[i].fZ;
   }
   centre *= 1.f / 8.f;

   return centre;   
}

//______________________________________________________________________________________________________
float
FWPFEcalRecHitLegoProxyBuilder::calculateEt( const TEveVector &centre, float E )
{
   TEveVector vec = centre;
   float et;

   vec.Normalize();
   vec *= E;
   et = vec.Perp();

   return et;
}

//______________________________________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::build( const FWEventItem *iItem, TEveElementList *product, const FWViewContext *vc )
{
   //cleanLocal called before the build, no need to free recHit vec

   float maxEnergy = 0.0f;
   float maxEt = 0.0f;

   for( int index = 0; index < static_cast<int>(iItem->size() ); ++index )
   {
      TEveCompound *itemHolder = createCompound();
      product->AddElement( itemHolder );

      const EcalRecHit &iData = modelData( index );
      const float *corners = item()->getGeom()->getCorners( iData.detid() );
      float energy, et;
      std::vector<TEveVector> etaphiCorners(8);
      TEveVector centre;

      if( corners == 0 )
         continue;

      for( int i = 0; i < 4; ++i )
      {
         int j = i * 3;
         TEveVector cv = TEveVector( corners[j], corners[j+1], corners[j+2] );
         etaphiCorners[i].fX = cv.Eta();                    // Conversion of rechit X/Y values for plotting in Eta/Phi
         cv = TEveVector( corners[j], corners[j+1], corners[j+2] );
         etaphiCorners[i].fY = cv.Phi();
         etaphiCorners[i].fZ = 0.0;                         // Small (floor) Z offset

         etaphiCorners[i+4].fX = etaphiCorners[i].fX;
         etaphiCorners[i+4].fY = etaphiCorners[i].fY;       // Top can simply be plotted exactly over the top of the bottom face
         etaphiCorners[i+4].fZ = 0.001;
         // printf("%f %f %d \n",  etaphiCorners[i].fX, etaphiCorners[i].fY, i);
      }
      centre = calculateCentre( etaphiCorners );
      energy = iData.energy();
      et = calculateEt( centre, energy );

      if( energy > maxEnergy )
         maxEnergy = energy;
      if( energy > maxEt )
         maxEt = et;

      m_maxEnergyLog = log(maxEnergy);
      m_maxEtLog = log(maxEt);

      FWPFLegoRecHit *recHit = new FWPFLegoRecHit( etaphiCorners, itemHolder, this, vc, energy, et );
      recHit->setSquareColor(item()->defaultDisplayProperties().color());
      m_recHits.push_back( recHit );
   }
}

//______________________________________________________________________________________________________
REGISTER_FWPROXYBUILDER( FWPFEcalRecHitLegoProxyBuilder, EcalRecHit, "Ecal RecHit PF", FWViewType::kLegoPFECALBit );
