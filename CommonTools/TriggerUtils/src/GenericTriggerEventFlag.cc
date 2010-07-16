//
// $Id$
//


#include "CommonTools/TriggerUtils/interface/GenericTriggerEventFlag.h"

#include "CondFormats/HLTObjects/interface/AlCaRecoTriggerBits.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtLogicParser.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <string>
#include <vector>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream> // DEBUG


/// To be called from the ED module's c'tor
GenericTriggerEventFlag::GenericTriggerEventFlag( const edm::ParameterSet & config )
  : watchDB_( 0 )
  , dbLabel_( "" )
  , verbose_( 0 )
  , gtDBKey_( "" )
  , l1BeforeMask_( true )
  , l1DBKey_( "" )
  , hltDBKey_( "" )
  , on_( true )
  , onDcs_( true )
  , onGt_( true )
  , onL1_( true )
  , onHlt_( true )
  , configError_( "CONFIG_ERROR" )
{

  // General switch(es)
  if ( config.exists( "andOr" ) ) {
    andOr_ = config.getParameter< bool >( "andOr" );
    if ( config.exists( "verbosityLevel" ) ) verbose_ = config.getParameter< unsigned >( "verbosityLevel" );
  } else {
    on_    = false;
    onDcs_ = false;
    onGt_  = false;
    onL1_  = false;
    onHlt_ = false;
  }

  if ( on_ ) {
    if ( config.exists( "andOrDcs" ) ) {
      andOrDcs_      = config.getParameter< bool >( "andOrDcs" );
      dcsInputTag_   = config.getParameter< edm::InputTag >( "dcsInputTag" );
      dcsPartitions_ = config.getParameter< std::vector< int > >( "dcsPartitions" );
      errorReplyDcs_ = config.getParameter< bool >( "errorReplyDcs" );
    } else {
      onDcs_ = false;
    }
    if ( config.exists( "andOrGt" ) ) {
      andOrGt_              = config.getParameter< bool >( "andOrGt" );
      gtInputTag_           = config.getParameter< edm::InputTag >( "gtInputTag" );
      gtLogicalExpressions_ = config.getParameter< std::vector< std::string > >( "gtStatusBits" );
      errorReplyGt_         = config.getParameter< bool >( "errorReplyGt" );
      if ( config.exists( "gtDBKey" ) ) gtDBKey_ = config.getParameter< std::string >( "gtDBKey" );
    } else {
      onGt_ = false;
    }
    if ( config.exists( "andOrL1" ) ) {
      andOrL1_              = config.getParameter< bool >( "andOrL1" );
      l1LogicalExpressions_ = config.getParameter< std::vector< std::string > >( "l1Algorithms" );
      errorReplyL1_         = config.getParameter< bool >( "errorReplyL1" );
      if ( config.exists( "l1DBKey" ) )      l1DBKey_      = config.getParameter< std::string >( "l1DBKey" );
      if ( config.exists( "l1BeforeMask" ) ) l1BeforeMask_ = config.getParameter< bool >( "l1BeforeMask" );
    } else {
      onL1_ = false;
    }
    if ( config.exists( "andOrHlt" ) ) {
      andOrHlt_              = config.getParameter< bool >( "andOrHlt" );
      hltInputTag_           = config.getParameter< edm::InputTag >( "hltInputTag" );
      hltLogicalExpressions_ = config.getParameter< std::vector< std::string > >( "hltPaths" );
      errorReplyHlt_         = config.getParameter< bool >( "errorReplyHlt" );
      if ( config.exists( "hltDBKey" ) ) hltDBKey_ = config.getParameter< std::string >( "hltDBKey" );
    } else {
      onHlt_ = false;
    }
    if ( ! onDcs_ && ! onGt_ && ! onL1_ && ! onHlt_ ) on_      = false;
    else {
      if ( config.exists( "dbLabel" ) ) dbLabel_ = config.getParameter< std::string >( "dbLabel" );
      watchDB_ = new edm::ESWatcher< AlCaRecoTriggerBitsRcd >;
    }
  }

}


/// To be called from d'tors by 'delete'
GenericTriggerEventFlag::~GenericTriggerEventFlag()
{

  if ( on_ ) delete watchDB_;

}


/// To be called from beginedm::Run() methods
void GenericTriggerEventFlag::initRun( const edm::Run & run, const edm::EventSetup & setup )
{

  if ( watchDB_->check( setup ) ) {
    std::cout << "  GenericTriggerEventFlag::initRun(): GT DB tag  : " << gtDBKey_.data() << std::endl; // DEBUG
    if ( onGt_ && gtDBKey_.size() > 0 ) {
      const std::vector< std::string > exprs( expressionsFromDB( gtDBKey_, setup ) );
      if ( exprs.empty() || exprs.at( 0 ) != configError_ ) gtLogicalExpressions_ = exprs;
      std::cout << "  GenericTriggerEventFlag::initRun(): GT DB size : " << gtLogicalExpressions_.size() << std::endl; // DEBUG
      for ( size_t iE( 0 ); iE < gtLogicalExpressions_.size(); ++iE ) { // DEBUG
        std::cout << "                                     " << gtLogicalExpressions_.at( iE ) << std::endl; // DEBUG
      } // DEBUG
    }
    std::cout << "  GenericTriggerEventFlag::initRun(): L1 DB tag  : " << l1DBKey_.data() << std::endl; // DEBUG
    if ( onL1_ && l1DBKey_.size() > 0 ) {
      const std::vector< std::string > exprs( expressionsFromDB( l1DBKey_, setup ) );
      if ( exprs.empty() || exprs.at( 0 ) != configError_ ) l1LogicalExpressions_ = exprs;
      std::cout << "  GenericTriggerEventFlag::initRun(): L1 DB size : " << l1LogicalExpressions_.size() << std::endl; // DEBUG
      for ( size_t iE( 0 ); iE < l1LogicalExpressions_.size(); ++iE ) { // DEBUG
        std::cout << "                                     " << l1LogicalExpressions_.at( iE ) << std::endl; // DEBUG
      } // DEBUG
    }
    std::cout << "  GenericTriggerEventFlag::initRun(): HLT DB tag : " << hltDBKey_.data() << std::endl; // DEBUG
    if ( onHlt_ && hltDBKey_.size() > 0 ) {
      const std::vector< std::string > exprs( expressionsFromDB( hltDBKey_, setup ) );
      if ( exprs.empty() || exprs.at( 0 ) != configError_ ) hltLogicalExpressions_ = exprs;
      std::cout << "  GenericTriggerEventFlag::initRun(): HLT DB size: " << hltLogicalExpressions_.size() << std::endl; // DEBUG
      for ( size_t iE( 0 ); iE < hltLogicalExpressions_.size(); ++iE ) { // DEBUG
        std::cout << "                                     " << hltLogicalExpressions_.at( iE ) << std::endl; // DEBUG
      } // DEBUG
    }
  }

  hltConfigInit_ = false;
  if ( onHlt_ ) {
    if ( hltInputTag_.process().size() == 0 ) {
      if ( verbose_ > 0 ) edm::LogError( "GenericTriggerEventFlag" ) << "HLT TriggerResults InputTag \"" << hltInputTag_.encode() << "\" specifies no process";
    } else {
      bool hltChanged( false );
      if ( ! hltConfig_.init( run, setup, hltInputTag_.process(), hltChanged ) ) {
        if ( verbose_ > 0 ) edm::LogError( "GenericTriggerEventFlag" ) << "HLT config initialization error with process name \"" << hltInputTag_.process() << "\"";
      } else if ( hltConfig_.size() <= 0 ) {
        if ( verbose_ > 0 ) edm::LogError( "GenericTriggerEventFlag" ) << "HLT config size error";
      } else hltConfigInit_ = true;
      if ( hltChanged ) std::cout << "  GenericTriggerEventFlag::initRun(): HLT changed"<< std::endl; // DEBUG
    }
  }

}


/// To be called from analyze/filter() methods
bool GenericTriggerEventFlag::accept( const edm::Event & event, const edm::EventSetup & setup )
{

  if ( ! on_ ) return true;

  // Determine decision
  if ( andOr_ ) return ( acceptDcs( event ) || acceptGt( event ) || acceptL1( event, setup ) || acceptHlt( event ) );
  return ( acceptDcs( event ) && acceptGt( event ) && acceptL1( event, setup ) && acceptHlt( event ) );

}


bool GenericTriggerEventFlag::acceptDcs( const edm::Event & event )
{

  // An empty DCS partitions list acts as switch.
  if ( ! onDcs_ || dcsPartitions_.empty() ) return ( ! andOr_ ); // logically neutral, depending on base logical connective

  // Accessing the DcsStatusCollection
  edm::Handle< DcsStatusCollection > dcsStatus;
  event.getByLabel( dcsInputTag_, dcsStatus );
  if ( ! dcsStatus.isValid() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "DcsStatusCollection product with InputTag \"" << dcsInputTag_.encode() << "\" not in event ==> decision: " << errorReplyDcs_;
    return errorReplyDcs_;
  }
  if ( ( *dcsStatus ).size() == 0 ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "DcsStatusCollection product with InputTag \"" << dcsInputTag_.encode() << "\" empty ==> decision: " << errorReplyDcs_;
    return errorReplyDcs_;
  }

  // Determine decision of DCS partition combination and return
  if ( andOrDcs_ ) { // OR combination
    for ( std::vector< int >::const_iterator partitionNumber = dcsPartitions_.begin(); partitionNumber != dcsPartitions_.end(); ++partitionNumber ) {
      if ( acceptDcsPartition( dcsStatus, *partitionNumber ) ) return true;
    }
    return false;
  }
  for ( std::vector< int >::const_iterator partitionNumber = dcsPartitions_.begin(); partitionNumber != dcsPartitions_.end(); ++partitionNumber ) {
    if ( ! acceptDcsPartition( dcsStatus, *partitionNumber ) ) return false;
  }
  return true;

}


bool GenericTriggerEventFlag::acceptDcsPartition( const edm::Handle< DcsStatusCollection > & dcsStatus, int dcsPartition ) const
{

  // Error checks
  switch( dcsPartition ) {
    case DcsStatus::EBp   :
    case DcsStatus::EBm   :
    case DcsStatus::EEp   :
    case DcsStatus::EEm   :
    case DcsStatus::HBHEa :
    case DcsStatus::HBHEb :
    case DcsStatus::HBHEc :
    case DcsStatus::HF    :
    case DcsStatus::HO    :
    case DcsStatus::RPC   :
    case DcsStatus::DT0   :
    case DcsStatus::DTp   :
    case DcsStatus::DTm   :
    case DcsStatus::CSCp  :
    case DcsStatus::CSCm  :
    case DcsStatus::CASTOR:
    case DcsStatus::TIBTID:
    case DcsStatus::TOB   :
    case DcsStatus::TECp  :
    case DcsStatus::TECm  :
    case DcsStatus::BPIX  :
    case DcsStatus::FPIX  :
    case DcsStatus::ESp   :
    case DcsStatus::ESm   :
      break;
    default:
      if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "DCS partition number \"" << dcsPartition << "\" does not exist ==> decision: " << errorReplyDcs_;
      return errorReplyDcs_;
  }

  // Determine decision
  std::cout << "  GenericTriggerEventFlag: partition " << dcsPartition << " -> " << dcsStatus->at( 0 ).ready( dcsPartition ) << std::endl; // DEBUG
  return dcsStatus->at( 0 ).ready( dcsPartition );

}


/// Does this event fulfill the configured GT status logical expression combination?
bool GenericTriggerEventFlag::acceptGt( const edm::Event & event )
{

  // An empty GT status bits logical expressions list acts as switch.
  if ( ! onGt_ || gtLogicalExpressions_.empty() ) return ( ! andOr_ ); // logically neutral, depending on base logical connective

  // Accessing the L1GlobalTriggerReadoutRecord
  edm::Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
  event.getByLabel( gtInputTag_, gtReadoutRecord );
  if ( ! gtReadoutRecord.isValid() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "L1GlobalTriggerReadoutRecord product with InputTag \"" << gtInputTag_.encode() << "\" not in event ==> decision: " << errorReplyGt_;
    return errorReplyGt_;
  }

  // Determine decision of GT status bits logical expression combination and return
  if ( andOrGt_ ) { // OR combination
    for ( std::vector< std::string >::const_iterator gtLogicalExpression = gtLogicalExpressions_.begin(); gtLogicalExpression != gtLogicalExpressions_.end(); ++gtLogicalExpression ) {
      if ( acceptGtLogicalExpression( gtReadoutRecord, *gtLogicalExpression ) ) return true;
    }
    return false;
  }
  for ( std::vector< std::string >::const_iterator gtLogicalExpression = gtLogicalExpressions_.begin(); gtLogicalExpression != gtLogicalExpressions_.end(); ++gtLogicalExpression ) {
    if ( ! acceptGtLogicalExpression( gtReadoutRecord, *gtLogicalExpression ) ) return false;
  }
  return true;

}


/// Does this event fulfill this particular GT status bits' logical expression?
bool GenericTriggerEventFlag::acceptGtLogicalExpression( const edm::Handle< L1GlobalTriggerReadoutRecord > & gtReadoutRecord, std::string gtLogicalExpression )
{

  // Check empty std::strings
  if ( gtLogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty logical expression ==> decision: " << errorReplyGt_;
    return errorReplyGt_;
  }

  // Negated paths
  bool negExpr( negate( gtLogicalExpression ) );
  if ( negExpr && gtLogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty (negated) logical expression ==> decision: " << errorReplyGt_;
    return errorReplyGt_;
  }

  // Parse logical expression and determine GT status bit decision
  L1GtLogicParser gtAlgoLogicParser( gtLogicalExpression );
  // Loop over status bits
  for ( size_t iStatusBit = 0; iStatusBit < gtAlgoLogicParser.operandTokenVector().size(); ++iStatusBit ) {
    const std::string gtStatusBit( gtAlgoLogicParser.operandTokenVector().at( iStatusBit ).tokenName );
    // Manipulate status bit decision as stored in the parser
    bool decision;
    // Hard-coded status bits!!!
    if ( gtStatusBit == "PhysDecl" || gtStatusBit == "PhysicsDeclared" ) {
      decision = ( gtReadoutRecord->gtFdlWord().physicsDeclared() == 1 );
    } else {
      if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "GT status bit \"" << gtStatusBit << "\" is not defined ==> decision: " << errorReplyGt_;
      decision = errorReplyDcs_;
    }
    std::cout << "  GenericTriggerEventFlag: status bit " << gtStatusBit << " -> " << decision << std::endl; // DEBUG
    gtAlgoLogicParser.operandTokenVector().at( iStatusBit ).tokenResult = decision;
  }

  // Determine decision
  const bool gtDecision( gtAlgoLogicParser.expressionResult() );
  std::cout << "  GenericTriggerEventFlag: GT status logical expression "; // DEBUG
  if ( negExpr ) std::cout << "~\"" << gtLogicalExpression << "\" -> " << ( ! gtDecision ) << std::endl; // DEBUG
  else           std::cout <<  "\"" << gtLogicalExpression << "\" -> " <<     gtDecision   << std::endl; // DEBUG
  return negExpr ? ( ! gtDecision ) : gtDecision;

}


/// Was this event accepted by the configured L1 logical expression combination?
bool GenericTriggerEventFlag::acceptL1( const edm::Event & event, const edm::EventSetup & setup )
{

  // An empty L1 logical expressions list acts as switch.
  if ( ! onL1_ || l1LogicalExpressions_.empty() ) return ( ! andOr_ ); // logically neutral, depending on base logical connective

  // Getting the L1 event setup
  l1Gt_.retrieveL1EventSetup( setup ); // FIXME This can possibly go to initRun()

  // Determine decision of L1 logical expression combination and return
  if ( andOrL1_ ) { // OR combination
    for ( std::vector< std::string >::const_iterator l1LogicalExpression = l1LogicalExpressions_.begin(); l1LogicalExpression != l1LogicalExpressions_.end(); ++l1LogicalExpression ) {
      if ( acceptL1LogicalExpression( event, *l1LogicalExpression ) ) return true;
    }
    return false;
  }
  for ( std::vector< std::string >::const_iterator l1LogicalExpression = l1LogicalExpressions_.begin(); l1LogicalExpression != l1LogicalExpressions_.end(); ++l1LogicalExpression ) {
    if ( ! acceptL1LogicalExpression( event, *l1LogicalExpression ) ) return false;
  }
  return true;

}


/// Was this event accepted by this particular L1 algorithms' logical expression?
bool GenericTriggerEventFlag::acceptL1LogicalExpression( const edm::Event & event, std::string l1LogicalExpression )
{

  // Check empty std::strings
  if ( l1LogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty logical expression ==> decision: " << errorReplyL1_;
    return errorReplyL1_;
  }

  // Negated logical expression
  bool negExpr( negate( l1LogicalExpression ) );
  if ( negExpr && l1LogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty (negated) logical expression ==> decision: " << errorReplyL1_;
    return errorReplyL1_;
  }

  // Parse logical expression and determine L1 decision
  L1GtLogicParser l1AlgoLogicParser( l1LogicalExpression );
  // Loop over algorithms
  for ( size_t iAlgorithm = 0; iAlgorithm < l1AlgoLogicParser.operandTokenVector().size(); ++iAlgorithm ) {
    const std::string l1AlgoName( l1AlgoLogicParser.operandTokenVector().at( iAlgorithm ).tokenName );
    std::cout << "  GenericTriggerEventFlag: algo name " << l1AlgoName << std::endl; // DEBUG
    int error( -1 );
    const bool decision( l1BeforeMask_ ? l1Gt_.decisionBeforeMask( event, l1AlgoName, error ) : l1Gt_.decisionAfterMask( event, l1AlgoName, error ) );
    const std::string choice( l1BeforeMask_ ? "before mask" : "after mask" ); // DEBUG
    std::cout << "  GenericTriggerEventFlag: " << choice << std::endl; // DEBUG
    if ( l1Gt_.decisionBeforeMask( event, l1AlgoName, error ) != l1Gt_.decisionAfterMask( event, l1AlgoName, error ) ) { // DEBUG
      std::cout << "  GenericTriggerEventFlag: before/after " << l1Gt_.decisionBeforeMask( event, l1AlgoName, error ) << "/" << l1Gt_.decisionAfterMask( event, l1AlgoName, error ) << " differ" << std::endl; // DEBUG
    } // DEBUG
    // Error checks
    if ( error != 0 ) {
      if ( verbose_ > 1 ) {
        if ( error == 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "L1 algorithm \"" << l1AlgoName << "\" does not exist in the L1 menu ==> decision: "                                          << errorReplyL1_;
        else              edm::LogWarning( "GenericTriggerEventFlag" ) << "L1 algorithm \"" << l1AlgoName << "\" received error code " << error << " from L1GtUtils::decisionBeforeMask ==> decision: " << errorReplyL1_;
      }
      l1AlgoLogicParser.operandTokenVector().at( iAlgorithm ).tokenResult = errorReplyL1_;
      continue;
    }
    // Manipulate algo decision as stored in the parser
    std::cout << "  GenericTriggerEventFlag: decision " << decision << std::endl; // DEBUG
    l1AlgoLogicParser.operandTokenVector().at( iAlgorithm ).tokenResult = decision;
  }

  // Return decision
  const bool l1Decision( l1AlgoLogicParser.expressionResult() );
  std::cout << "  GenericTriggerEventFlag: L1 logical expression "; // DEBUG
  if ( negExpr ) std::cout << "~\"" << l1LogicalExpression << "\" -> " << ( ! l1Decision ) << std::endl; // DEBUG
  else           std::cout <<  "\"" << l1LogicalExpression << "\" -> " <<     l1Decision   << std::endl; // DEBUG
  return negExpr ? ( ! l1Decision ) : l1Decision;

}


/// Was this event accepted by the configured HLT logical expression combination?
bool GenericTriggerEventFlag::acceptHlt( const edm::Event & event )
{

  // An empty HLT logical expressions list acts as switch.
  if ( ! onHlt_ || hltLogicalExpressions_.empty() ) return ( ! andOr_ ); // logically neutral, depending on base logical connective

  // Checking the HLT configuration,
  if ( ! hltConfigInit_ ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "HLT config error ==> decision: " << errorReplyHlt_;
    return errorReplyHlt_;
  }

  // Accessing the TriggerResults
  edm::Handle< edm::TriggerResults > hltTriggerResults;
  event.getByLabel( hltInputTag_, hltTriggerResults );
  if ( ! hltTriggerResults.isValid() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "TriggerResults product with InputTag \"" << hltInputTag_.encode() << "\" not in event ==> decision: " << errorReplyHlt_;
    return errorReplyHlt_;
  }
  if ( ( *hltTriggerResults ).size() == 0 ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "TriggerResults product with InputTag \"" << hltInputTag_.encode() << "\" empty ==> decision: " << errorReplyHlt_;
    return errorReplyDcs_;
  }

  // Determine decision of HLT logical expression combination and return
  if ( andOrHlt_ ) { // OR combination
    for ( std::vector< std::string >::const_iterator hltLogicalExpression = hltLogicalExpressions_.begin(); hltLogicalExpression != hltLogicalExpressions_.end(); ++hltLogicalExpression ) {
      if ( acceptHltLogicalExpression( hltTriggerResults, *hltLogicalExpression ) ) return true;
    }
    return false;
  }
  for ( std::vector< std::string >::const_iterator hltLogicalExpression = hltLogicalExpressions_.begin(); hltLogicalExpression != hltLogicalExpressions_.end(); ++hltLogicalExpression ) {
    if ( ! acceptHltLogicalExpression( hltTriggerResults, *hltLogicalExpression ) ) return false;
  }
  return true;

}


/// Was this event accepted by this particular HLT paths' logical expression?
bool GenericTriggerEventFlag::acceptHltLogicalExpression( const edm::Handle< edm::TriggerResults > & hltTriggerResults, std::string hltLogicalExpression ) const
{

  // Check empty std::strings
  if ( hltLogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty logical expression ==> decision: " << errorReplyHlt_;
    return errorReplyHlt_;
  }

  // Negated paths
  bool negExpr( negate( hltLogicalExpression ) );
  if ( negExpr && hltLogicalExpression.empty() ) {
    if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "Empty (negated) logical expression ==> decision: " << errorReplyHlt_;
    return errorReplyHlt_;
  }

  // Parse logical expression and determine HLT decision
  L1GtLogicParser hltAlgoLogicParser( hltLogicalExpression );
  // Loop over paths
  for ( size_t iPath = 0; iPath < hltAlgoLogicParser.operandTokenVector().size(); ++iPath ) {
    const std::string hltPathName( hltAlgoLogicParser.operandTokenVector().at( iPath ).tokenName );
    const unsigned indexPath( hltConfig_.triggerIndex( hltPathName ) );
    // Further error checks
    if ( indexPath == hltConfig_.size() ) {
      if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "HLT path \"" << hltPathName << "\" is not found in process " << hltInputTag_.process() << " ==> decision: " << errorReplyHlt_;
      hltAlgoLogicParser.operandTokenVector().at( iPath ).tokenResult = errorReplyHlt_;
      continue;
    }
    if ( hltTriggerResults->error( indexPath ) ) {
      if ( verbose_ > 1 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "HLT path \"" << hltPathName << "\" in error ==> decision: " << errorReplyHlt_;
      hltAlgoLogicParser.operandTokenVector().at( iPath ).tokenResult = errorReplyHlt_;
      continue;
    }
    // Manipulate algo decision as stored in the parser
    const bool decision( hltTriggerResults->accept( indexPath ) );
    std::cout << "  GenericTriggerEventFlag: path name " << hltPathName << " -> " << decision << std::endl; // DEBUG
    hltAlgoLogicParser.operandTokenVector().at( iPath ).tokenResult = decision;
  }

  // Determine decision
  const bool hltDecision( hltAlgoLogicParser.expressionResult() );
  std::cout << "  GenericTriggerEventFlag: HLT logical expression "; // DEBUG
  if ( negExpr ) std::cout << "~\"" << hltLogicalExpression << "\" -> " << ( ! hltDecision ) << std::endl; // DEBUG
  else           std::cout <<  "\"" << hltLogicalExpression << "\" -> " <<     hltDecision   << std::endl; // DEBUG
  return negExpr ? ( ! hltDecision ) : hltDecision;

}



/// Reads and returns logical expressions from DB
std::vector< std::string > GenericTriggerEventFlag::expressionsFromDB( const std::string & key, const edm::EventSetup & setup )
{

  edm::ESHandle< AlCaRecoTriggerBits > logicalExpressions;
  setup.get< AlCaRecoTriggerBitsRcd >().get( dbLabel_, logicalExpressions );
  const std::map< std::string, std::string > & expressionMap = logicalExpressions->m_alcarecoToTrig;
  std::map< std::string, std::string >::const_iterator listIter = expressionMap.find( key );
  if ( listIter == expressionMap.end() ) {
    if ( verbose_ > 0 ) edm::LogWarning( "GenericTriggerEventFlag" ) << "No logical expressions found under key " << key << " in 'AlCaRecoTriggerBitsRcd'";
    return std::vector< std::string >( 1, configError_ );
  }
  return logicalExpressions->decompose( listIter->second );

}



/// Checks for negated words
bool GenericTriggerEventFlag::negate( std::string & word ) const
{

  bool negate( false );
  if ( word.at( 0 ) == '~' ) {
    negate = true;
    word.erase( 0, 1 );
  }
  return negate;

}
