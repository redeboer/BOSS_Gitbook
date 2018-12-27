#include "GaudiKernel/DeclareFactoryEntries.h"
#include "TrackSelector/D0phi_KpiKK.h"
#include "TrackSelector/rhopi_pipigg.h"

DECLARE_ALGORITHM_FACTORY(D0phi_KpiKK)
DECLARE_ALGORITHM_FACTORY(rhopi_pipigg)

DECLARE_FACTORY_ENTRIES(D0phi_KpiKK) {
  DECLARE_ALGORITHM(D0phi_KpiKK);
}

DECLARE_FACTORY_ENTRIES(rhopi_pipigg) {
  DECLARE_ALGORITHM(rhopi_pipigg);
}
