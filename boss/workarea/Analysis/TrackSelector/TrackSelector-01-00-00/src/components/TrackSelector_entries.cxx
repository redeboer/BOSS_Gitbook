#include "GaudiKernel/DeclareFactoryEntries.h"
#include "TrackSelector/TrackSelector.h"
#include "TrackSelector/D0phi_KpiKK.h"

DECLARE_ALGORITHM_FACTORY(TrackSelector)

DECLARE_FACTORY_ENTRIES(TrackSelector) {
  DECLARE_ALGORITHM(TrackSelector);
  DECLARE_ALGORITHM(D0phi_KpiKK);
  DECLARE_ALGORITHM(rhopi_pipigg);
}

