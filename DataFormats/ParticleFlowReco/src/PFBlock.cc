#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"

#include <iomanip>
#include <sstream>

using namespace std;
using namespace reco;


void PFBlock::addElement(  PFBlockElement* element ) {
  element->setIndex( elements_.size() );
  element->lock();
  elements_.push_back( element->clone() ); 

}


void PFBlock::bookLinkData() {

}



void PFBlock::setLink(unsigned i1, unsigned i2, 
		      double Chi2, double Dist,
                      LinkData& linkData, 
		      LinkTest test) const {
  
  assert( test<LINKTEST_ALL );
  
  unsigned index = 0;
  bool ok =  matrix2vector(i1,i2, index);

  if(ok) {
    //ignore the  -1, -1 pair
    if ( Chi2 > -0.5 || Dist > -0.5 ) {
        Link & l = linkData[index];
        l.distance=Dist;
        l.chi2=Chi2;
        l.test |= (1 << test);
    }     else  //delete if existing
    {
     LinkData::iterator it = linkData.find(index);
     if(it!=linkData.end()) linkData.erase(it);
    }

  } else {
    assert(0);
  }
  
}


// void PFBlock::lock(unsigned i, LinkData& linkData ) const {
  
//   assert( linkData.size() == linkDataSize() );
  
//   for(unsigned j=0; j<elements_.size(); j++) {
    
//     if(i==j) continue;
    
//     unsigned index = 0;
//     bool ok =  matrix2vector(i,j, index);
//     if(ok)
//       linkData[index] = -1;
//     else 
//       assert(0);
//   }
// }



void PFBlock::associatedElements( unsigned i, 
                                  const LinkData& linkData, 
                                  multimap<double, unsigned>& sortedAssociates,
                                  PFBlockElement::Type type,
				  LinkTest test ) 
  const {
  


  sortedAssociates.clear();
  
  // i is too large
  if( i > elements_.size() ) return;
  assert(i>=0);
  
  for(unsigned ie=0; ie<elements_.size(); ie++) {
    
    // considered element itself
    if( ie == i ) {
      continue;
    }
    // not the right type
    if(type !=  PFBlockElement::NONE && 
       elements_[ie].type() != type ) {
      continue;
    }

    // double c2 = chi2(i, ie,  linkData );
    // PJ the real distance is better to decide what cluster is closest 
    // pJ -> Order the elements by increasing distance !

    unsigned index = 0;
    if( !matrix2vector(i, ie, index) ) continue;

    double c2=-1;
    LinkData::const_iterator it =  linkData.find(index);
    if(it!=linkData.end() && ( ( (1 << test ) & it->second.test) !=0 || (test == LINKTEST_ALL) ) ) c2= it->second.distance;

    // not associated
    if( c2 < 0 ) { 
      continue;
    }
//    std::cout << "ardebug " << c2 << " " <<  ie << " " <<  i << " " << type << std::endl;
    sortedAssociates.insert( pair<double,unsigned>(c2, ie) );
  }
} 

bool PFBlock::matrix2vector( unsigned iindex, unsigned jindex, 
                             unsigned& index ) const {

  unsigned size = elements_.size();
  if( iindex == jindex || 
      iindex >=  size ||
      jindex >=  size ) {
    return false;
  }
  
  if( iindex > jindex ) 
    std::swap( iindex, jindex);

  
  index = jindex-iindex-1;

  if(iindex>0) {
    index += iindex*size;
    unsigned missing = iindex*(iindex+1)/2;
    index -= missing;
  }
  
  return true;
}


double PFBlock::chi2( unsigned ie1, unsigned ie2,
                      const LinkData& linkData) const {
  
  double Chi2 = -1;

  unsigned index = 0;
  if( !matrix2vector(ie1, ie2, index) ) return Chi2;
  LinkData::const_iterator it =  linkData.find(index);
  if(it!=linkData.end() ) Chi2= it->second.chi2;

  return Chi2;

}

double PFBlock::dist( unsigned ie1, unsigned ie2,
                      const LinkData& linkData) const {

  double Dist = -1;

  unsigned index = 0;
  if( !matrix2vector(ie1, ie2, index) ) return Dist;
  LinkData::const_iterator it =  linkData.find(index);
  if(it!=linkData.end() ) Dist= it->second.distance;

  return Dist;

}


ostream& reco::operator<<(  ostream& out, 
                            const reco::PFBlock& block ) {

  if(! out) return out;
  const edm::OwnVector< reco::PFBlockElement >& elements = block.elements();
  out<<"\t--- PFBlock ---  "<<endl;
  out<<"\tnumber of elements: "<<elements.size()<<endl;
  
  // Build element label (string) : elid from type, layer and occurence number
  // use stringstream instead of sprintf to concatenate string and integer into string
 
  vector <string> elid;
  string s;
  stringstream ss;
  int iel = 0;
  int iTK =0;
  int iGSF =0;
  int iBREM=0;
  int iPS1 = 0;
  int iPS2 = 0;
  int iEE = 0;
  int iEB = 0;
  int iHE = 0;
  int iHB = 0;
  int iHFEM = 0;
  int iHFHAD = 0;

  // for each element in turn
  std::vector<bool> toPrint(elements.size(),static_cast<bool>(true));
  for(unsigned ie=0; ie<elements.size(); ie++) {
    
    PFBlockElement::Type type = elements[ie].type();
    std::multimap<double, unsigned> ecalElems;
    switch(type){
    case PFBlockElement::TRACK:
      iTK++;
      ss << "TK" << iTK;
      break;
    case PFBlockElement::GSF:
      iGSF++;
      ss << "GSF" << iGSF;
      break;
    case PFBlockElement::BREM:
      block.associatedElements( ie,  block.linkData(),
				ecalElems ,
				reco::PFBlockElement::ECAL,
				reco::PFBlock::LINKTEST_ALL );
      iBREM++;
      if ( ecalElems.size() ) { 
	ss << "BR" << iBREM;
      } else {
	toPrint[ie] = false;
      }
      break;    
    default:{
      PFClusterRef clusterref = elements[ie].clusterRef();
      int layer = clusterref->layer();
      switch (layer){
      case PFLayer::PS1:
        iPS1++;
        ss << "PV" << iPS1;
        break;
      case PFLayer::PS2:
        iPS2++;
        ss << "PH" << iPS2;
        break;
      case PFLayer::ECAL_ENDCAP:
        iEE++;
        ss << "EE" << iEE;
        break;
      case PFLayer::ECAL_BARREL:
        iEB++;
        ss << "EB" << iEB;
        break;
      case PFLayer::HCAL_ENDCAP:
        iHE++;
        ss << "HE" << iHE;
        break;
      case PFLayer::HCAL_BARREL1:
        iHB++;
        ss << "HB" << iHB;
        break;
      case PFLayer::HF_EM:
        iHFEM++;
        ss << "FE" << iHFEM;
        break;
      case PFLayer::HF_HAD:
        iHFHAD++;
        ss << "FH" << iHFHAD;
	break;
      default:
        iel++;   
        ss << "??" << iel;
        break;
      }
      break;
    }
    }
    s = ss.str();
    elid.push_back( s );
    // clear stringstream
    ss.str("");

    if ( toPrint[ie] ) out<<"\t"<< s <<" "<<elements[ie] <<endl;
  }
  
  out<<endl;
  int width = 6;
  if( !block.linkData().empty() ) {
    out<<"\tlink data (chi squared): "<<endl;
    out<<setiosflags(ios::right);
    out<<"\t" << setw(width) << " ";
    for(unsigned ie=0; ie<elid.size(); ie++) 
      if ( toPrint[ie] ) out <<setw(width)<< elid[ie];
    out<<endl;  
    out<<setiosflags(ios::fixed);
    out<<setprecision(1);      
  
    for(unsigned i=0; i<block.elements_.size(); i++) {
      if ( !toPrint[i] ) continue;
      out<<"\t";
      out <<setw(width) << elid[i];
      for(unsigned j=0; j<block.elements_.size(); j++) {
	if ( !toPrint[j] ) continue;
        double chi2 = block.chi2(i,j, block.linkData() );

	//Note Alex: we might want to print out all the
	//linkdata matrix obtained from chi2, rechit, tangent tests?
	
	//if not linked, try to see if linked by rechit
	if( chi2<0 ) chi2 = block.chi2(i,j, block.linkData());
//				       PFBlock::LINKTEST_RECHIT );

	if (chi2 > -0.5) out<<setw(width)<< chi2; 
        else  out <<setw(width)<< " ";
      }
      out<<endl;
    }
    out<<setprecision(3);  
    out<<resetiosflags(ios::right|ios::fixed);
  }
  else {
    out<<"\tno links."<<endl;
  }
      
  width = 6;
  if( !block.linkData().empty() ) {
    out<<endl<<"\tlink data (distance x 1000): "<<endl;
    out<<setiosflags(ios::right);
    out<<"\t" << setw(width) << " ";
    for(unsigned ie=0; ie<elid.size(); ie++) 
      if ( toPrint[ie] ) out <<setw(width)<< elid[ie];
    out<<endl;  
    out<<setiosflags(ios::fixed);
    out<<setprecision(1);      
  
    for(unsigned i=0; i<block.elements_.size(); i++) {
      if ( !toPrint[i] ) continue;
      out<<"\t";
      out <<setw(width) << elid[i];
      for(unsigned j=0; j<block.elements_.size(); j++) {
	if ( !toPrint[j] ) continue;
        double Dist = block.dist(i,j, block.linkData());//,PFBlock::LINKTEST_ALL);

	// out<<setw(width)<< Dist*1000.;
	if (Dist > -0.5) out<<setw(width)<< Dist*1000.; 
        else  out <<setw(width)<< " ";
      }
      out<<endl;
    }

    out<<setprecision(3);  
    out<<resetiosflags(ios::right|ios::fixed);
  }
  else {
    out<<"\tno links."<<endl;
  }
  
  return out;
}
 
 
unsigned PFBlock::linkDataSize() const {
  unsigned n = elements_.size();
  
  // number of possible undirected links between n elements.
  // reflective links impossible.
 
  return n*(n-1)/2; 
}
