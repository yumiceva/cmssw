#include "CalibTracker/SiStripCommon/interface/ShallowGainCalibration.h"
#include "CalibFormats/SiStripObjects/interface/SiStripGain.h" 
#include "CalibTracker/Records/interface/SiStripGainRcd.h"  

using namespace edm;
using namespace reco;
using namespace std;

ShallowGainCalibration::ShallowGainCalibration(const edm::ParameterSet& iConfig)
  :  theTracksLabel( iConfig.getParameter<edm::InputTag>("Tracks") ),
     Suffix       ( iConfig.getParameter<std::string>("Suffix")    ),
     Prefix       ( iConfig.getParameter<std::string>("Prefix") )
{
  produces <std::vector<int> >            ( Prefix + "trackindex"     + Suffix );
  produces <std::vector<unsigned int> >   ( Prefix + "rawid"          + Suffix );
  produces <std::vector<float> >          ( Prefix + "localdirx"      + Suffix );
  produces <std::vector<float> >          ( Prefix + "localdiry"      + Suffix );
  produces <std::vector<float> >          ( Prefix + "localdirz"      + Suffix );
  produces <std::vector<float> >          ( Prefix + "localposx"      + Suffix );
  produces <std::vector<float> >          ( Prefix + "localposy"      + Suffix );
  produces <std::vector<unsigned short> > ( Prefix + "firststrip"     + Suffix );
  produces <std::vector<unsigned short> > ( Prefix + "nstrips"        + Suffix );
  produces <std::vector<float> >          ( Prefix + "path"           + Suffix );
  produces <std::vector<unsigned char> >  ( Prefix + "amplitude"      + Suffix );
  produces <std::vector<double> >         ( Prefix + "gainused"       + Suffix );
}

void ShallowGainCalibration::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<int> >            trackindex    ( new std::vector<int>            );
  std::auto_ptr<std::vector<unsigned int> >   rawid         ( new std::vector<unsigned int>   );
  std::auto_ptr<std::vector<float>  >         localdirx     ( new std::vector<float>          );
  std::auto_ptr<std::vector<float>  >         localdiry     ( new std::vector<float>          );
  std::auto_ptr<std::vector<float>  >         localdirz     ( new std::vector<float>          );
  std::auto_ptr<std::vector<float>  >         localposx     ( new std::vector<float>          );
  std::auto_ptr<std::vector<float>  >         localposy     ( new std::vector<float>          );
  std::auto_ptr<std::vector<unsigned short> > firststrip    ( new std::vector<unsigned short> );
  std::auto_ptr<std::vector<unsigned short> > nstrips       ( new std::vector<unsigned short> );
  std::auto_ptr<std::vector<float>  >         path          ( new std::vector<float>          );
  std::auto_ptr<std::vector<unsigned char> >  amplitude     ( new std::vector<unsigned char>  );
  std::auto_ptr<std::vector<double>  >        gainused      ( new std::vector<double>          );

  edm::ESHandle<TrackerGeometry> theTrackerGeometry;         iSetup.get<TrackerDigiGeometryRecord>().get( theTrackerGeometry );  
  m_tracker=&(* theTrackerGeometry );
  edm::ESHandle<SiStripGain> gainHandle;                     iSetup.get<SiStripGainRcd>().get(gainHandle);
  edm::Handle<edm::View<reco::Track> > tracks;	             iEvent.getByLabel(theTracksLabel, tracks);	  
  edm::Handle<TrajTrackAssociationCollection> associations;  iEvent.getByLabel(theTracksLabel, associations);

  for( TrajTrackAssociationCollection::const_iterator association = associations->begin(); association != associations->end(); association++) {
       const Trajectory*  traj  = association->key.get();
       const reco::Track* track = association->val.get();

       vector<TrajectoryMeasurement> measurements = traj->measurements();
       for(vector<TrajectoryMeasurement>::const_iterator measurement_it = measurements.begin(); measurement_it!=measurements.end(); measurement_it++){
          TrajectoryStateOnSurface trajState = measurement_it->updatedState();
          if( !trajState.isValid() ) continue;     

          const TrackingRecHit*         hit                 = (*measurement_it->recHit()).hit();
          const SiStripRecHit1D*        sistripsimple1dhit  = dynamic_cast<const SiStripRecHit1D*>(hit);
          const SiStripRecHit2D*        sistripsimplehit    = dynamic_cast<const SiStripRecHit2D*>(hit);
          const SiStripMatchedRecHit2D* sistripmatchedhit   = dynamic_cast<const SiStripMatchedRecHit2D*>(hit);
          const SiPixelRecHit*          pixelHit            = dynamic_cast<const SiPixelRecHit*>(hit);

          LocalVector             trackDirection = trajState.localDirection();
          double                  cosine         = trackDirection.z()/trackDirection.mag();

          if(pixelHit){
               uint32_t                DetId = pixelHit->geographicalId();
               double                  Path  = (10.0*thickness(DetId))/fabs(cosine);

               trackindex    ->push_back( shallow::findTrackIndex(tracks, track) ); 
               rawid         ->push_back( DetId );         
               localdirx     ->push_back( trackDirection.x() );
               localdiry     ->push_back( trackDirection.y() );
               localdirz     ->push_back( trackDirection.z() );
               localposx     ->push_back( (trajState.localPosition()).x() );
               localposy     ->push_back( (trajState.localPosition()).y() );
               firststrip    ->push_back( -1 );
               nstrips       ->push_back( 1 );
               path          ->push_back( Path );
               gainused      ->push_back( 1.0 );  
               amplitude     ->push_back( pixelHit->cluster()->charge() );

          }else{
               const SiStripCluster*   Cluster = NULL;
               uint32_t                DetId = 0;

             for(unsigned int h=0;h<2;h++){
               if(!sistripmatchedhit && h==1){
                  continue;
               }else if(sistripmatchedhit  && h==0){
                  Cluster = &sistripmatchedhit->monoCluster();
                  DetId = sistripmatchedhit->monoId();
               }else if(sistripmatchedhit  && h==1){
                  Cluster = &sistripmatchedhit->stereoCluster();;
                  DetId = sistripmatchedhit->stereoId();
               }else if(sistripsimplehit){
                  Cluster = (sistripsimplehit->cluster()).get();
                  DetId = sistripsimplehit->geographicalId().rawId();
               }else if(sistripsimple1dhit){
                  Cluster = (sistripsimple1dhit->cluster()).get();
                  DetId = sistripsimple1dhit->geographicalId().rawId();
               }else{
                  continue;
               }

               const vector<uint8_t>&  Ampls          = Cluster->amplitudes();
               int                     FirstStrip     = Cluster->firstStrip();
               int                     APVId          = FirstStrip/128;
               double                  Path           = (10.0*thickness(DetId))/fabs(cosine);
               double                  PrevGain       = -1;

               if(gainHandle.isValid()){ 
                  SiStripApvGain::Range detGainRange = gainHandle->getRange(DetId);
                  PrevGain = *(detGainRange.first + APVId);
               }

               for(unsigned int a=0;a<Ampls.size();a++){               
                  amplitude->push_back( Ampls[a] );
               }

               trackindex    ->push_back( shallow::findTrackIndex(tracks, track) ); 
               rawid         ->push_back( DetId );         
               localdirx     ->push_back( trackDirection.x() );
               localdiry     ->push_back( trackDirection.y() );
               localdirz     ->push_back( trackDirection.z() );
               localposx     ->push_back( (trajState.localPosition()).x() );
               localposy     ->push_back( (trajState.localPosition()).y() );
               firststrip    ->push_back( FirstStrip );
               nstrips       ->push_back( Ampls.size() );
               path          ->push_back( Path );
               gainused      ->push_back( PrevGain );  
             }
          }
      }
  }

  iEvent.put(trackindex,    Prefix + "trackindex"    + Suffix );
  iEvent.put(rawid     ,    Prefix + "rawid"         + Suffix );
  iEvent.put(localdirx ,    Prefix + "localdirx"     + Suffix );
  iEvent.put(localdiry ,    Prefix + "localdiry"     + Suffix );
  iEvent.put(localdirz ,    Prefix + "localdirz"     + Suffix );
  iEvent.put(localposx ,    Prefix + "localposx"     + Suffix );
  iEvent.put(localposy ,    Prefix + "localposy"     + Suffix );
  iEvent.put(firststrip,    Prefix + "firststrip"    + Suffix );
  iEvent.put(nstrips,       Prefix + "nstrips"       + Suffix );
  iEvent.put(path,          Prefix + "path"          + Suffix );
  iEvent.put(amplitude,     Prefix + "amplitude"     + Suffix );
  iEvent.put(gainused,      Prefix + "gainused"      + Suffix );
}


/*
bool ShallowGainCalibration::IsFarFromBorder(TrajectoryStateOnSurface* trajState, const uint32_t detid, const edm::EventSetup* iSetup)
{ 
  edm::ESHandle<TrackerGeometry> tkGeom; iSetup->get<TrackerDigiGeometryRecord>().get( tkGeom );

  LocalPoint  HitLocalPos   = trajState->localPosition();
  LocalError  HitLocalError = trajState->localError().positionError() ;

  const GeomDetUnit* it = tkGeom->idToDetUnit(DetId(detid));
  if (dynamic_cast<const StripGeomDetUnit*>(it)==0 && dynamic_cast<const PixelGeomDetUnit*>(it)==0) {
     std::cout << "this detID doesn't seem to belong to the Tracker" << std::endl;
     return false;
  }

  const BoundPlane plane = it->surface();
  const TrapezoidalPlaneBounds* trapezoidalBounds( dynamic_cast<const TrapezoidalPlaneBounds*>(&(plane.bounds())));
  const RectangularPlaneBounds* rectangularBounds( dynamic_cast<const RectangularPlaneBounds*>(&(plane.bounds())));

  double DistFromBorder = 1.0;    
  double HalfLength     = it->surface().bounds().length() /2.0;

  if(trapezoidalBounds)
  {
     std::vector<float> const & parameters = (*trapezoidalBounds).parameters();
     HalfLength     = parameters[3];
  }else if(rectangularBounds){
     HalfLength     = it->surface().bounds().length() /2.0;
  }else{return false;}

  if (fabs(HitLocalPos.y())+HitLocalError.yy() >= (HalfLength - DistFromBorder) ) return false;

  return true;
}*/


double ShallowGainCalibration::thickness(DetId id)
{
 map<DetId,double>::iterator th=m_thicknessMap.find(id);
 if(th!=m_thicknessMap.end())
   return (*th).second;
 else {
   double detThickness=1.;
   //compute thickness normalization
   const GeomDetUnit* it = m_tracker->idToDetUnit(DetId(id));
   bool isPixel = dynamic_cast<const PixelGeomDetUnit*>(it)!=0;
   bool isStrip = dynamic_cast<const StripGeomDetUnit*>(it)!=0;
   if (!isPixel && ! isStrip) {
   //FIXME throw exception
      edm::LogWarning("DeDxHitsProducer") << "\t\t this detID doesn't seem to belong to the Tracker";
      detThickness = 1.;
  }else{
      detThickness = it->surface().bounds().thickness();
  }

   m_thicknessMap[id]=detThickness;//computed value
   return detThickness;
 }
}

