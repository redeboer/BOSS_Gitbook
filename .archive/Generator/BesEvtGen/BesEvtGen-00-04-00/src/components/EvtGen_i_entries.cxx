//====================================================================
//  EVTDECAY_entries.cxx
//--------------------------------------------------------------------
//
//  Package    : Generators/EvtDecay
//
//  Description: Declaration of the components (factory entries)
//               specified by this component library.
//
//====================================================================

#include "EvtDecay.h"
#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_ALGORITHM_FACTORY( EvtDecay )

DECLARE_FACTORY_ENTRIES( BesEvtGen ) {
    DECLARE_ALGORITHM( EvtDecay );
}
