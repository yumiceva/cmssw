#ifndef L1Trigger_RPCTBMuon_h
#define L1Trigger_RPCTBMuon_h
#include "L1Trigger/RPCTrigger/interface/RPCMuon.h"
#include "L1Trigger/RPCTrigger/interface/RPCPacMuon.h"
//---------------------------------------------------------------------------
//output of the Pac (one LogCone),
//used in m_TBGhostBuster
//input and output of the m_TBGhostBuster

/** \class RPCTBMuon
  * Used in Ghoust Busters and sorters.
  * Has additionall filds: m_Killed, m_GBData, m_EtaAddress, m_PhiAddress need in these algorithms.
  * \author Karol Bunkowski (Warsaw)
  */


class RPCTBMuon: public RPCMuon {
public:
  ///Empty muon.
  RPCTBMuon();

  RPCTBMuon(int ptCode, int quality, int sign, int patternNum, unsigned short firedPlanes);

  RPCTBMuon(const RPCPacMuon& pacMuon);

  int getCode() const;

  void setCode(int code);

  void setPhiAddr(int phiAddr);

  void setSectorAddr(int sectorAddr);

  void setEtaAddr(int etaAddr);
  
  void setAddress(int etaAddr, int phiAddr);

  void setAddress(int tbNumber, int tbTower, int phiAddr);

  int getEtaAddr() const;

  int getPhiAddr() const;

  int getSegmentAddr() const;

  int getSectorAddr() const;

  int getContinSegmAddr() const;

  void setCodeAndPhiAddr(int code, int phiAddr);

  void setCodeAndEtaAddr(int code, int etaAddr);

  int getGBData() const;

  std::string getGBDataBitStr() const;
  
  std::string printDebugInfo(int debugFormat) const;

  std::string printExtDebugInfo(int, int, int) const;

  void setGBDataKilledFirst();

  void setGBDataKilledLast();

  bool gBDataKilledFirst() const;

  bool gBDataKilledLast() const;

  /** @param where - to bits meaning is differnt on diffrent places
   * values:
   * 	fsbOut - outputo of the Final Sorter Board - input of GMT	
   * 
   * */ 		
  unsigned int toBits(std::string where) const;
  
  void fromBits(std::string where, unsigned int value);
  
  std::string bitsToString() const;

//------need to perform ghost - busting ------------
  void kill();

  /** @return true = was non-empty muon and was killed
    * false = was not killed or is zero */
  bool wasKilled() const;

  /** @return true = was no-zero muon and was not killed
    * false = is killed or is zero */
  bool isLive() const;
//aaa
  ///Used in sorting.
  struct TMuonMore : public std::less<RPCTBMuon> {
    bool operator()(const RPCTBMuon& muonL,
                     const RPCTBMuon& muonR) const {
      return muonL.getCode() > muonR.getCode();
    }
  };


  class FSBOut {
  private:
  	static const int m_phiBitsCnt  = 8;  static const unsigned int m_phiBitsMask  = 0xff;
  	static const int m_ptBitsCnt   = 5;  static const unsigned int m_ptBitsMask   = 0x1f;
  	static const int m_qualBitsCnt = 3;  static const unsigned int m_qualBitsMask = 0x7;
  	static const int m_etaBitsCnt  = 6;  static const unsigned int m_etaBitsMask  = 0x3f;
  	static const int m_signBitsCnt = 1;  static const unsigned int m_signBitsMask = 0x1;
  public:	
  	static unsigned int toBits(const RPCTBMuon& muon);
  	static void fromBits(RPCTBMuon& muon, unsigned int value);
  };

private:
//------ hardware signals------------------------
  unsigned int m_EtaAddress;

  unsigned int m_PhiAddress;

  /** 2 bits,
    * 0 00 - this muon did not kill nothing on the sector edge
    * 1 01 - this muon killed muon from segment 0 (the "right" sector edge), or is in segment 0
    * 2 10 - this muon killed muon from segment 11 (the "left" sector edge), or is in segment 11
    * 3 11 - this muon killed muon from segment 0 and 11 */
  unsigned int m_GBData; 

//------- need to perform ghost - busting ---------
  bool m_Killed; //!< true means that muon was killed during GB
	
  friend class FSBOut;

  class FSBIn {
  private:
  	static const int m_signBitsCnt = 1;  static const unsigned int m_signBitsMask = 0x1;
  	static const int m_ptBitsCnt   = 5;  static const unsigned int m_ptBitsMask   = 0x1f;
  	static const int m_qualBitsCnt = 3;  static const unsigned int m_qualBitsMask = 0x7;  	
  	static const int m_phiBitsCnt  = 7;  static const unsigned int m_phiBitsMask  = 0x7f;  	  	
  	static const int m_etaBitsCnt  = 6;  static const unsigned int m_etaBitsMask  = 0x3f;
  public:	  	
  	static unsigned int toBits(const RPCTBMuon& muon);
  	static void fromBits(RPCTBMuon& muon, unsigned int value);
  };
  friend class FSBIn;
  
};

typedef std::vector<RPCTBMuon> L1RpcTBMuonsVec;
typedef std::vector<L1RpcTBMuonsVec> L1RpcTBMuonsVec2;

#endif

