#ifndef ECAL_FENIX_LINEARIZER_H
#define ECAL_FENIX_LINEARIZER_H

#include <SimCalorimetry/EcalTrigPrimAlgos/interface/EcalVLinearizer.h>
#include <DataFormats/EcalDigi/interface/EBDataFrame.h>

class EBDetId;
class EcalBarrelTopology;

namespace tpg {

// global type definitions for header defined by Tag entries in ArgoUML
// Result: typedef <typedef_global_header> <tag_value>;


  /** 
   \class EcalFenixLinearizer
   \brief Linearisation for Fenix strip
   *  input: 16 bits  corresponding to input EBDataFrame
   *  output: 18 bits 
   *  
   *  Question:
   *  - est-ce que ca correspond a la transformation en Et?
   *           -----> oui (avec possibilite de disable) 
   *  - faut-il chercher qqpart des constantes de calibration?
   *           -----> oui (une methode set est envisageable au debut remplir les valeurs a la main puis plus tard aller les chercher dans une database, ou eventuellement dans un .orcarc)
   *           -----> de meme pour le theta et pour les autres valeurs needed.
   *   
   *  
   *  Attention: 
   *  La multplication par un float est equivalente a la multiplication par un int suivie de n decalage (division par 2^n)
   *   
   *  Cette classe doit etre configuree cristal par cristal avec 9 parametres entiers chacun.
   */

  class EcalFenixLinearizer : public EcalVLinearizer {


  private:
    /** maximum number of samples per frame */
    //temporary, waiting for changes in EBDataFrame
    enum { SIZEMAX = 10}; 
    enum {numberOfCrystalsInStrip = 5};

    bool off;
    int uncorrectedSample_;
    int gainID_;
    int base_;
    int mult_;
    int shift_;
    int baseLine_[5][5][3]; //to be changed
    int strip_;
    EcalBarrelTopology *ebTopol_;

    //    int setInput(int RawSam, int stripNum, int XtalNumberInStrip);
   int setInput(EcalMGPASample RawSam, int stripNum, int XtalNumberInStrip);
    int getBase(int XtalNumberInTower) const;
    int getMult() const;
    int getShift() const;
    int process() ;
    //UB??    int findCrystalNumber(const CellID&);
   int findCrystalNumber(const EBDetId&);



  public:
    EcalFenixLinearizer(EcalBarrelTopology *);
    virtual ~EcalFenixLinearizer();
    // future version:
    virtual EBDataFrame process(EBDataFrame) {EBDataFrame df;return df;}
    virtual EBDataFrame process(EBDataFrame,int stripnr); 

  };

} /* End of namespace tpg */

#endif
