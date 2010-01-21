// $Id: Normal.cc,v 1.6.4.2 2009/10/13 14:13:55 mommsen Exp $
/// @file: Normal.cc

#include "EventFilter/StorageManager/interface/StateMachine.h"
#include "EventFilter/StorageManager/interface/TransitionRecord.h"

#include <iostream>

using namespace std;
using namespace stor;

void Normal::do_entryActionWork()
{
  TransitionRecord tr( stateName(), true );
  outermost_context().updateHistory( tr );
}

Normal::Normal( my_context c ): my_base(c)
{
  safeEntryAction();
}

void Normal::do_exitActionWork()
{
  TransitionRecord tr( stateName(), false );
  outermost_context().updateHistory( tr );
}

Normal::~Normal()
{
  safeExitAction();
}

string Normal::do_stateName() const
{
  return string( "Normal" );
}

void Normal::do_moveToFailedState( xcept::Exception& exception ) const
{
  outermost_context().getSharedResources()->moveToFailedState( exception );
}

/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
