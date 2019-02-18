#include "GaudiKernel/DeclareFactoryEntries.h"
#include "DTagAlg/DTag.h"
#include "DTagAlg/NeutralDReconstruction.h"
#include "DTagAlg/ChargedDReconstruction.h"
#include "DTagAlg/DsReconstruction.h"


DECLARE_ALGORITHM_FACTORY( DTag )
DECLARE_ALGORITHM_FACTORY( NeutralDReconstruction )
DECLARE_ALGORITHM_FACTORY( ChargedDReconstruction )
DECLARE_ALGORITHM_FACTORY( DsReconstruction )
  
DECLARE_FACTORY_ENTRIES( DTagAlg ) {
  DECLARE_ALGORITHM(DTag);
}
