#ifndef LOCAL_EPTOPIPIETA3PI_SELECTOR_H
#define LOCAL_EPTOPIPIETA3PI_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalEptoPiPiEta3PiSelector : public DCSelectionFunction<CDDecay>
{
 public :

  LocalEptoPiPiEta3PiSelector();

  bool operator() (CDDecay& aEp);


 private :

  LocalEptoPiPiEta3PiSelector( const LocalEptoPiPiEta3PiSelector& );
  const LocalEptoPiPiEta3PiSelector& operator= ( const LocalEptoPiPiEta3PiSelector& );

  double m_minMass;
  double m_maxMass;

};

extern LocalEptoPiPiEta3PiSelector eptoPiPiEta3PiSelector;

#endif
