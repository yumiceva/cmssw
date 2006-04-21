/** \file
 *
 * $Date: 2006/04/21 14:25:38 $
 * $Revision: 1.7 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 */

/* This Class Header */
#include "RecoLocalMuon/DTSegment/src/DTCombinatorialPatternReco.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTSuperLayer.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2D.h"
#include "RecoLocalMuon/DTSegment/src/DTSegmentUpdator.h"
#include "RecoLocalMuon/DTSegment/src/DTSegmentCleaner.h"
#include "RecoLocalMuon/DTSegment/src/DTHitPairForFit.h"
#include "RecoLocalMuon/DTSegment/src/DTSegmentCand.h"

/* C++ Headers */
#include <iterator>
using namespace std;
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/* ====================================================================== */

/// Constructor
DTCombinatorialPatternReco::DTCombinatorialPatternReco(const edm::ParameterSet& pset) : 
DTRecSegment2DBaseAlgo(pset), theAlgoName("DTCombinatorialPatternReco")
{
  theMaxAllowedHits = pset.getParameter<unsigned int>("MaxAllowedHits"); // 100
  theAlphaMaxTheta = pset.getParameter<double>("AlphaMaxTheta");// 0.1 ;
  theAlphaMaxPhi = pset.getParameter<double>("AlphaMaxPhi");// 1.0 ;
  debug = pset.getUntrackedParameter<bool>("debug"); //true;
  theUpdator = new DTSegmentUpdator(pset);
  theCleaner = new DTSegmentCleaner(pset);
}

/// Destructor
DTCombinatorialPatternReco::~DTCombinatorialPatternReco() {
}

/* Operations */ 
edm::OwnVector<DTRecSegment2D>
DTCombinatorialPatternReco::reconstruct(const DTSuperLayer* sl,
                                        const std::vector<DTRecHit1DPair>& pairs){

  edm::OwnVector<DTRecSegment2D> result;
  vector<DTHitPairForFit*> hitsForFit = initHits(sl, pairs);

  vector<DTSegmentCand*> candidates = buildSegments(sl, hitsForFit);

  vector<DTSegmentCand*>::const_iterator cand=candidates.begin();
  while (cand<candidates.end()) {

    //RB:TODO put an implict converter instead of the explicit conversion below
    DTRecSegment2D *segment = (*cand)->convert();

    theUpdator->update(segment);

    result.push_back(segment);
    delete *(cand++); // delete the candidate!
  }
  // vector<DTRecHit1D> hits;
  // for (vector<DTRecHit1DPair>::const_iterator pair=pairs.begin();
  //      pair!=pairs.end(); ++pair) {
  //   hits.push_back(*(*pair).componentRecHit(DTEnums::Right));
  // }
  // DTRecSegment2D*  seg = new DTRecSegment2D(sl->id(), hits);
  return result;
}

void DTCombinatorialPatternReco::setES(const edm::EventSetup& setup){
  // Get the DT Geometry
  setup.get<MuonGeometryRecord>().get(theDTGeometry);
  theUpdator->setES(setup);
}

vector<DTHitPairForFit*>
DTCombinatorialPatternReco::initHits(const DTSuperLayer* sl,
                                     const std::vector<DTRecHit1DPair>& hits){  
  
  vector<DTHitPairForFit*> result;
  for (vector<DTRecHit1DPair>::const_iterator hit=hits.begin();
       hit!=hits.end(); ++hit) {
    result.push_back(new DTHitPairForFit(*hit, *sl, theDTGeometry));
  }
  //TODO needed??
  //sort(theHits.begin(),theHits.end(), pHitSort());
  return result;
}

vector<DTSegmentCand*>
DTCombinatorialPatternReco::buildSegments(const DTSuperLayer* sl,
                                          const std::vector<DTHitPairForFit*>& hits){

  typedef vector<DTHitPairForFit*> hitCont;
  typedef hitCont::const_iterator  hitIter;
  vector<DTSegmentCand*> result;

  if(debug) {
    cout << "buildSegments: " << sl->id() << " nHits " << hits.size() << endl;
    for (vector<DTHitPairForFit*>::const_iterator hit=hits.begin();
         hit!=hits.end(); ++hit) cout << **hit<< endl;
  }

  // 10-Mar-2004 SL
  // put a protection against heavily populated chambers, for which the segment
  // building could lead to infinite memory usage...
  if (hits.size() > theMaxAllowedHits ) {
    if(debug) {
      cout << "Warning: this chamber " << sl->id() << " has too many hits : "
        << hits.size() << " max allowed is " << theMaxAllowedHits << endl;
      cout << "Skipping segment reconstruction... " << endl;
    }
    return result;
  }

  /// get two hits in different layers and see if there are other / hits
  //  compatible with them
  for (hitCont::const_iterator firstHit=hits.begin(); firstHit!=hits.end();
       ++firstHit) {
    for (hitCont::const_reverse_iterator lastHit=hits.rbegin(); 
         (*lastHit)!=(*firstHit); ++lastHit) {
      if ((*lastHit)->id()==(*firstHit)->id()) continue; // hits in different layers!
      if(debug) {
        cout << "Selected these two hits pair " << endl;
        cout << "First " << *(*firstHit) << endl;
        cout << "Last " << *(*lastHit) << endl;
      }

      GlobalPoint IP;
      float DAlphaMax;
      if ((sl->id()).superlayer()==2)  // Theta SL
        DAlphaMax=theAlphaMaxTheta;
      else // Phi SL
        DAlphaMax=theAlphaMaxPhi;

      DTEnums::DTCellSide codes[2]={DTEnums::Right, DTEnums::Left};
      for (int firstLR=0; firstLR<2; ++firstLR) {
        for (int lastLR=0; lastLR<2; ++lastLR) {
	  // TODO move the global transformation in the DTHitPairForFit class
	  // when it will be moved I will able to remove the sl from the input parameter and
	  // use this function for the super-Phi segments.
	  GlobalPoint gposFirst=sl->toGlobal( (*firstHit)->localPosition(codes[firstLR]) );
	  GlobalPoint gposLast= sl->toGlobal( (*lastHit)->localPosition(codes[lastLR]) );
	  
	  // was:
          // const GeomDet* firstDet=dtGeom.idToDet((*firstHit)->id());
	  // const GeomDet* lastDet=dtGeom.idToDet((*lastHit)->id());
	  // GlobalPoint gposFirst=firstDet->globalPosition(codes[firstLR]);
	  // GlobalPoint gposLast=lastDet->globalPosition(codes[lastLR]);
	  // RB @ SL: is it right my substitution?

          GlobalVector gvec=gposLast-gposFirst;
          GlobalVector gvecIP=gposLast-IP;

          // difference in angle measured
          float DAlpha=fabs(gvec.theta()-gvecIP.theta());

          // cout << "DAlpha " << DAlpha << endl;
          if (DAlpha<DAlphaMax) {

            // create a segment hypotesis
            // I don't need a true segment, just direction and position
            LocalPoint posIni = (*firstHit)->localPosition(codes[firstLR]);
            LocalVector dirIni = 
              ((*lastHit)->localPosition(codes[lastLR])-posIni).unit();

            // search for other compatible hits, with or without the L/R solved
            vector<AssPoint> assHits = findCompatibleHits(posIni, dirIni, hits);
            if(debug) 
              cout << "compatible hits " << assHits.size() << endl;

            // get the best segment with these hits: it's just one! 
            // (is it correct?)
            DTSegmentCand* seg = buildBestSegment(assHits, sl);

            if (seg) {
              if(debug) 
                cout << "segment " << *seg<< endl;

              // check if the chi2 and #hits are ok
              if (!seg->good()) {
                delete seg;
              } else { 

                // remove duplicated segments (I know, would be better to do it before the
                // fit...)
                if (checkDoubleCandidates(result,seg)) {
                  // add to the vector of hypotesis
                  result.push_back(seg);
                  if(debug) 
                    cout << "result is now " << result.size() << endl;
                } else { // delete it!
                  delete seg;
                  if(debug) 
                    cout << "already existing" << endl;
                }
              }
            }
          }
        }
      }
    }
  }
  if (debug) {
    for (vector<DTSegmentCand*>::const_iterator seg=result.begin();
         seg!=result.end(); ++seg) 
      cout << *(*seg) << endl;
  }

  // now I have a couple of segment hypotesis, should check for ghost
  result = theCleaner->clean(result);
  if (debug) {
    cout << "result no ghost  " << result.size() << endl;
    for (vector<DTSegmentCand*>::const_iterator seg=result.begin();
         seg!=result.end(); ++seg) 
      cout << *(*seg) << endl;
  }

  return result;
}


vector<DTCombinatorialPatternReco::AssPoint>
DTCombinatorialPatternReco::findCompatibleHits(const LocalPoint& posIni,
                                               const LocalVector& dirIni,
                                               const vector<DTHitPairForFit*>& hits) {
  if (debug) cout << "Pos: " << posIni << " Dir: "<< dirIni << endl;
  vector<AssPoint> result;

  typedef vector<DTHitPairForFit*> hitCont;
  typedef hitCont::const_iterator  hitIter;
  for (hitIter hit=hits.begin(); hit!=hits.end(); ++hit) {
    pair<bool,bool> isCompatible = (*hit)->isCompatible(posIni, dirIni);
    if (debug) 
      cout << "isCompatible " << isCompatible.first << " " <<
        isCompatible.second << endl;

    // if only one of the two is compatible, then the LR is assigned,
    // otherwise is undefined

    DTEnums::DTCellSide lrcode;
    if (isCompatible.first && isCompatible.second) 
      lrcode=DTEnums::undefLR;
    else if (isCompatible.first) 
      lrcode=DTEnums::Left;
    else if (isCompatible.second) 
      lrcode=DTEnums::Right;
    else 
      continue; // neither is compatible
    result.push_back(AssPoint(*hit, lrcode));
  }
  return result;
}

DTSegmentCand*
DTCombinatorialPatternReco::buildBestSegment(std::vector<AssPoint>& hits,
                                             const DTSuperLayer* sl) {
  if (hits.size()<3) {
    //cout << "buildBestSegment: hits " << hits.size()<< endl;
    return 0; // a least 3 point
  }

  // hits with defined LR
  vector<AssPoint> points;

  // without: I store both L and R, a deque since I need front insertion and
  // deletion
  deque<DTHitPairForFit* > pointsNoLR; 

  // first add only the hits with LR assigned
  for (vector<AssPoint>::const_iterator hit=hits.begin();
       hit!=hits.end(); ++hit) {
    if ((*hit).second != DTEnums::undefLR) {
      points.push_back(*hit);
    } else { // then also for the undef'd one
      pointsNoLR.push_back((*hit).first);
    }
  }

  if(debug) {
    cout << "points " << points.size() << endl;
    cout << "pointsNoLR " << pointsNoLR.size() << endl;
  }

  // build all possible candidates using L/R ambiguity
  vector<DTSegmentCand*> candidates ;

  buildPointsCollection(points, pointsNoLR, candidates, sl);

  if(debug)
    cout << "candidates " << candidates.size() << endl;

  // so now I have build a given number of segments, I should find the best one,
  // by #hits and chi2.
  vector<DTSegmentCand*>::const_iterator bestCandIter = candidates.end();
  double minChi2=999999.;
  unsigned int maxNumHits=0;
  for (vector<DTSegmentCand*>::const_iterator iter=candidates.begin();
       iter!=candidates.end(); ++iter) {
    if ((*iter)->nHits()==maxNumHits && (*iter)->chi2()<minChi2) {
      minChi2=(*iter)->chi2();
      bestCandIter=iter;
    } else if ((*iter)->nHits()>maxNumHits) {
      maxNumHits=(*iter)->nHits();
      minChi2=(*iter)->chi2();
      bestCandIter=iter;
    }
  }

  // delete all candidates but the best one!
  for (vector<DTSegmentCand*>::iterator iter=candidates.begin();
       iter!=candidates.end(); ++iter) if (iter!=bestCandIter) delete *iter;

       // return the best candate if any
       if (bestCandIter != candidates.end()) {
         return (*bestCandIter);
       }
       return 0;
}

void
DTCombinatorialPatternReco::buildPointsCollection(vector<AssPoint>& points, 
                                                  deque<DTHitPairForFit*>& pointsNoLR, 
                                                  vector<DTSegmentCand*>& candidates,
                                                  const DTSuperLayer* sl) {

  if(debug) {
    cout << "buildPointsCollection " << endl;
    cout << "points: " << points.size() << " NOLR: " << pointsNoLR.size()<< endl;
  }
  if (pointsNoLR.size()>0) { // still unassociated points!
    DTHitPairForFit* unassHit = pointsNoLR.front();
    // try with the right
    if(debug)
      cout << "Right hit" << endl;
    points.push_back(AssPoint(unassHit, DTEnums::Right));
    pointsNoLR.pop_front();
    buildPointsCollection(points, pointsNoLR, candidates, sl);
    pointsNoLR.push_front((unassHit));
    points.pop_back();

    // try with the left
    if(debug)
      cout << "Left hit" << endl;
    points.push_back(AssPoint(unassHit, DTEnums::Left));
    pointsNoLR.pop_front();
    buildPointsCollection(points, pointsNoLR, candidates, sl);
    pointsNoLR.push_front((unassHit));
    points.pop_back();
  } else { // all associated

    if(debug) {
      cout << "The Hits were" << endl;
      copy(points.begin(), points.end(),
           ostream_iterator<DTSegmentCand::AssPoint>(std::cout));
      cout << "----" << endl;
      cout << "All associated " << endl;
    }
    DTSegmentCand::AssPointCont pointsSet;

    // for (vector<AssPoint>::const_iterator point=points.begin();
    //      point!=points.end(); ++point) 
    pointsSet.insert(points.begin(),points.end());

    if(debug) {
      cout << "The Hits are" << endl;
      copy(pointsSet.begin(), pointsSet.end(),
           ostream_iterator<DTSegmentCand::AssPoint>(std::cout));
      cout << "----" << endl;
    }

    DTSegmentCand* newCand = new DTSegmentCand(pointsSet,sl);
    if (theUpdator->fit(newCand)) candidates.push_back(newCand);
    else delete newCand; // bad seg, too few hits
  }
}

bool
DTCombinatorialPatternReco::checkDoubleCandidates(vector<DTSegmentCand*>& cands,
                                                  DTSegmentCand* seg) {
  for (vector<DTSegmentCand*>::iterator cand=cands.begin();
       cand!=cands.end(); ++cand) 
    if (*(*cand)==*seg) return false;
  return true;
}
